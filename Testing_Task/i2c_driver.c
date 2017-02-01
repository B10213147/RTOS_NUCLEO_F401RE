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

void i2c_driver_init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2, ENABLE);

	//
	// Enable the GPIO pin for the I2C1(PB8, PB9), I2C2(PB3, PB10).
	// Set the mode as Alternate function.
	//
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF9_I2C2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_ClockSpeed = 5000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStruct);

	I2C_InitStruct.I2C_OwnAddress1 = 0x10 << 1;
	I2C_Init(I2C2, &I2C_InitStruct);

	I2C_Cmd(I2C1, ENABLE);
	I2C_Cmd(I2C2, ENABLE);
}

void i2c_Master_Transmitter(void){
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, 0x10, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, 'M');
	while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF));
	I2C_GenerateSTOP(I2C1, ENABLE);
}

char master_Rx;
void i2C_Master_Receiver(void){
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, 0x10, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	master_Rx = I2C_ReceiveData(I2C1);
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void i2c_Slave_Transmitter(void){
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED));
	I2C_SendData(I2C2, 'S');
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_TRANSMITTED));
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_ACK_FAILURE));
}

char slave_Rx;
void i2c_Slave_Receiver(void){
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED));
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_RECEIVED));
	slave_Rx = I2C_ReceiveData(I2C2);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_STOP_DETECTED));
}
