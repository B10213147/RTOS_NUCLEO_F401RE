/*
 * keys_driver.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/

#include <stdint.h>
#include "keys_driver.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

extern int led_f;

void keys_driver_init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

uint8_t last_keys_state;
void keys_driver(void){
	uint8_t keys_state = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
	if(last_keys_state == 0 && keys_state == 1){
		led_f /= 2;
		if(led_f < 160000) led_f = 1600000;
	}
	last_keys_state = keys_state;
}
