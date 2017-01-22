/*
 * rtos_task.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/
 
#include "rtos.h"

void empty(void){
}

voidfuncptr priv_task = empty;
voidfuncptr sch_tab[] = {empty, empty};
int sch_tab_length = sizeof(sch_tab) / sizeof(sch_tab[0]);
