/*
 * rtos_sched.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/

#include "rtos_sched.h"

int sch_tst = task_completed;
int sch_idx = 0;

void rtos_sched(void){
	//
	// Task operate over time_slice will stuck the OS.
	//
	if(sch_tst == task_running) while(1);
	
	sch_tst = task_running;
	
	//priv_task();
	sch_tab[sch_idx]();

	sch_idx++;
	if(sch_idx >= sch_tab_length) sch_idx = 0;
	sch_tst = task_completed;
}
