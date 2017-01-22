/*
 * rtos_task.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/
 
#include "rtos.h"
#include "led_driver.h"
#include "keys_driver.h"
#include "usart_driver.h"

void empty(void){
}

voidfuncptr priv_task = empty;
voidfuncptr sch_tab[] = {led_driver, keys_driver, usart_driver, empty};
int sch_tab_length = sizeof(sch_tab) / sizeof(sch_tab[0]);
