/*
 * rtos_sched.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/

#include "rtos_sched.h"

void rtos_sched_run(struct rtos_task *task){
	rtos_task_remove(&rtos_ready_tasks, task);	
	rtos_running_task = task;
	
	rtos_running_task->function(rtos_running_task->arg);	
	
	// Put task at the back of the ready list.
	struct rtos_task *cur;
	for(cur = rtos_ready_tasks;
		cur->next != NULL;
		cur = cur->next);
	cur->next = rtos_running_task;
}

void rtos_sched(void){
	//
	// Task operate over time_slice will stuck the OS.
	//
	if(rtos_running_task != NULL) while(1);
	
	//priv_task();
	// Run first task on the ready list.
	rtos_sched_run(rtos_ready_tasks);
	
	rtos_running_task = NULL;
}
