/*
 * led_driver.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/

#include "led_driver.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void led_driver_init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int led_f = 1;
int period = 1000;	// 1000 * time_slice = 1s
int remain = 0;
void led_driver(void){
	remain--;
	if(remain <= 0){
		GPIO_ToggleBits(GPIOA, GPIO_Pin_5);	
		int time_intervel = period / 2 / led_f;
		remain = time_intervel;
	}
}
