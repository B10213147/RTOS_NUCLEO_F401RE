/*
 * usart_driver.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/

#include <stdint.h>
#include "usart_driver.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "led_driver.h"
#include "rtos.h"

void usart_driver_init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//
	// Enable the GPIO pin for the USART2(PA2, PA3). 
	// Set the mode as Alternate function.
	//	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//
	// Configure the USART2.
	//
	USART_InitTypeDef USART_InitStruct;
	USART_StructInit(&USART_InitStruct);
	USART_Init(USART2, &USART_InitStruct);

	USART_Cmd(USART2, ENABLE);
}

void usart_driver(void){	
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET){
		uint16_t temp = USART_ReceiveData(USART2);
		if(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == SET){
			USART_SendData(USART2, temp);
		}
		switch(temp){
			case '1': rtos_task_create(task1, 0); break;
			case '2': rtos_task_create(task2, 0); break;
			case '3': rtos_task_create(task3, 0); break;
			case '4': rtos_task_create(task4, 0); break;
			case 'a': rtos_task_delete(task1); break;
			case 'b': rtos_task_delete(task2); break;
			case 'c': rtos_task_delete(task3); break;
			case 'd': rtos_task_delete(task4); break;
			default: break;
		}
	}
}
