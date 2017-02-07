/*
 * i2c_driver.c
 *
 *  Created on: Jan 30, 2017
 *      Author: Harvard Tseng
**/

#include "i2c_driver.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"

#define TM_I2C_TIMEOUT 20000
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

int16_t TM_I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack);
void TM_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t TM_I2C_ReadAck(I2C_TypeDef* I2Cx);
uint8_t TM_I2C_ReadNack(I2C_TypeDef* I2Cx);
uint8_t TM_I2C_Stop(I2C_TypeDef* I2Cx);

uint32_t TM_I2C_Timeout;

void i2c_driver_init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	
	
	//
	// Enable the GPIO pin for the I2C1(PB8, PB9), I2C2(PB10, PB3). 
	// Set the mode as Alternate function.
	//	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_ClockSpeed = 50000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0xA0;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Cmd(I2C1, DISABLE);
	I2C_Init(I2C1, &I2C_InitStruct);	
	I2C_Cmd(I2C1, ENABLE);
}

uint8_t TM_I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address) {
	uint8_t data;
	TM_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	/* Also stop condition happens */
	data = TM_I2C_ReadNack(I2Cx);
	return data;
}

void TM_I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count) {
	TM_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	while (count--) {
		if (!count) {
			/* Last byte */
			data[count] = TM_I2C_ReadNack(I2Cx);
		} else {
			data[count] = TM_I2C_ReadAck(I2Cx);
		}
	}
}

void TM_I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data) {
	TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	TM_I2C_WriteData(I2Cx, data);
	TM_I2C_Stop(I2Cx);
}

void TM_I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count) {
	TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	while (count--) {
		TM_I2C_WriteData(I2Cx, *data++);
	}
	TM_I2C_Stop(I2Cx);
}

/* Private functions */
int16_t TM_I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack) {
	/* Generate I2C start pulse */
	I2Cx->CR1 |= I2C_CR1_START;
	
	/* Wait till I2C is busy */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_SB)) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Enable ack if we select it */
	if (ack) {
		I2Cx->CR1 |= I2C_CR1_ACK;
	}

	/* Send write/read bit */
	if (direction == I2C_TRANSMITTER_MODE) {
		/* Send address with zero last bit */
		I2Cx->DR = address & ~I2C_OAR1_ADD0;
		
		/* Wait till finished */
		TM_I2C_Timeout = TM_I2C_TIMEOUT;
		while (!(I2Cx->SR1 & I2C_SR1_ADDR)) {
			if (--TM_I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	if (direction == I2C_RECEIVER_MODE) {
		/* Send address with 1 last bit */
		I2Cx->DR = address | I2C_OAR1_ADD0;
		
		/* Wait till finished */
		TM_I2C_Timeout = TM_I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
			if (--TM_I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	
	/* Read status register to clear ADDR flag */
	I2Cx->SR2;
	
	/* Return 0, everything ok */
	return 0;
}

void TM_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data) {
	/* Wait till I2C is not busy anymore */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_TXE) && TM_I2C_Timeout) {
		TM_I2C_Timeout--;
	}
	
	/* Send I2C data */
	I2Cx->DR = data;
}

uint8_t TM_I2C_ReadAck(I2C_TypeDef* I2Cx) {
	uint8_t data;
	
	/* Enable ACK */
	I2Cx->CR1 |= I2C_CR1_ACK;
	
	/* Wait till not received */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}
	
	/* Read data */
	data = I2Cx->DR;
	
	/* Return data */
	return data;
}

uint8_t TM_I2C_ReadNack(I2C_TypeDef* I2Cx) {
	uint8_t data;
	
	/* Disable ACK */
	I2Cx->CR1 &= ~I2C_CR1_ACK;
	
	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;
	
	/* Wait till received */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Read data */
	data = I2Cx->DR;
	
	/* Return data */
	return data;
}

uint8_t TM_I2C_Stop(I2C_TypeDef* I2Cx) {
	/* Wait till transmitter not empty */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF)))) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}
	
	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;
	
	/* Return 0, everything ok */
	return 0;
}
