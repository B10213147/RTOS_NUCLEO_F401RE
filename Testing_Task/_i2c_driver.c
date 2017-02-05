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

uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);

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
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0xA0;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStruct);
	
	I2C_InitStruct.I2C_OwnAddress1 = 0x40;
	I2C_Init(I2C2, &I2C_InitStruct);
	
	I2C_ITConfig(I2C2, I2C_IT_BUF | I2C_IT_EVT | I2C_IT_ERR, ENABLE);
	NVIC_SetPriority(I2C2_EV_IRQn, 0);
	NVIC_SetPriority(I2C2_ER_IRQn, 0);
	NVIC_EnableIRQ(I2C2_EV_IRQn);
	NVIC_EnableIRQ(I2C2_ER_IRQn);
	
	I2C_Cmd(I2C1, ENABLE);
	I2C_Cmd(I2C2, ENABLE);
}

void i2c_Master_Transmitter(void){
	I2C_GenerateSTART(I2C1, ENABLE);
	//I2C_AcknowledgeConfig(I2C1, DISABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, 0x40, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, 'M');
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

int n = 2;
char master_Rx[3];
void i2c_Master_Receiver(void){
	master_Rx[0] = 0; master_Rx[1] = 0;
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, 0x40, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	//while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR));
	
	if(n > 1){
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		master_Rx[n] = I2C_ReceiveData(I2C1);
		n--;
	}
	else{
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		I2C_GenerateSTOP(I2C1, ENABLE);
		master_Rx[1] = I2C_ReceiveData(I2C1);	
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));	
		master_Rx[0] = I2C_ReceiveData(I2C1);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_STOP_DETECTED));
		I2C_AcknowledgeConfig(I2C1, ENABLE);
		n = 2;
	}
}

void i2c_Slave_Transmitter(void){
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED));
	I2C_SendData(I2C2, 'S');
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_TRANSMITTED));
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_ACK_FAILURE));
}

char slave_Rx[2];
void i2c_Slave_Receiver(void){
	if(I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED)){
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_RECEIVED));
		slave_Rx[1] = I2C_ReceiveData(I2C2);
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_STOP_DETECTED));
	}
}


void I2C2_EV_IRQHandler(void){
	if(I2C_GetITStatus(I2C2, I2C_IT_ADDR)){
		I2C_ClearITPendingBit(I2C2, I2C_IT_ADDR);
		I2C2->SR1;
		I2C2->SR2;
	}
	/* Slave Receiver */
	if(I2C_GetITStatus(I2C2, I2C_IT_RXNE)){		
		I2C_ClearITPendingBit(I2C2, I2C_IT_RXNE);
		slave_Rx[1] = I2C_ReceiveData(I2C2);
	}
	if(I2C_GetITStatus(I2C2, I2C_IT_STOPF)){
		I2C_ClearITPendingBit(I2C2, I2C_IT_STOPF);
		I2C_GenerateSTOP(I2C2, ENABLE);
	}
	
	/* Slave Transmitter */
	if(I2C_GetITStatus(I2C2, I2C_IT_TXE)){
		I2C_ClearITPendingBit(I2C2, I2C_IT_TXE);
		switch(n){
			case 0: I2C_SendData(I2C2, '0'); break;
			case 1: I2C_SendData(I2C2, '1'); break;
			case 2: I2C_SendData(I2C2, '2'); break;
			default: I2C_SendData(I2C2, 'E'); break;
		}
	}		
}

void I2C2_ER_IRQHandler(void){
	if(I2C_GetITStatus(I2C2, I2C_IT_AF)){
		I2C_ClearITPendingBit(I2C2, I2C_IT_AF);
	}	
}

void test1(void){
	slave_Rx[0] = 0; slave_Rx[1] = 0;
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, 0x30, I2C_Direction_Transmitter);
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED));
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, 'A');	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_RECEIVED));
	slave_Rx[0] = I2C_ReceiveData(I2C2);

	I2C_SendData(I2C1, 'B');	
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_RECEIVED));
	slave_Rx[1] = I2C_ReceiveData(I2C2);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_STOP_DETECTED));
}

void test2(void){
	master_Rx[0] = 0; master_Rx[1] = 0;
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, 0x30, I2C_Direction_Receiver);
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED));
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	I2C_SendData(I2C2, '0');
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_TRANSMITTING));
	
	master_Rx[0] = I2C_read_ack(I2C1);
/*	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	master_Rx[0] = I2C_ReceiveData(I2C1);	
*/	
	I2C_SendData(I2C2, '1');
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_TRANSMITTED));
	
	master_Rx[1] = I2C_read_nack(I2C1);
/*	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);	
	master_Rx[1] = I2C_ReceiveData(I2C1);
	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
*/	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	//I2C_GenerateSTOP(I2C1, ENABLE);	
	//while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_ACK_FAILURE));
}

/* This function reads one byte from the slave device 
* and acknowledges the byte (requests another byte)
*/
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx){
    uint8_t data;
    // enable acknowledge of recieved data
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    // wait until one byte has been received
    while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    // read data from I2C data register and return data byte
    data = I2C_ReceiveData(I2Cx);
    return data;
}

/* This function reads one byte from the slave device
* and doesn't acknowledge the recieved data 
*/
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx){
    uint8_t data;
    // disabe acknowledge of received data
    // nack also generates stop condition after last byte received
    // see reference manual for more info
	
    while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_GenerateSTOP(I2Cx, ENABLE);
    // wait until one byte has been received
    // read data from I2C data register and return data byte
    data = I2C_ReceiveData(I2Cx);
    return data;
}