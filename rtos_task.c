/*
 * rtos_task.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/
 
#include <stdlib.h>
#include "rtos_task.h"

void empty(void){
}
	
voidfuncptr priv_task = empty;
voidfuncptr sch_tab[] = {empty};
int sch_tab_length = sizeof(sch_tab) / sizeof(sch_tab[0]);

struct rtos_task *rtos_running_task;
struct rtos_task *rtos_ready_tasks = NULL;

void rtos_task_create(voidfuncptr func, void *arg){
	struct rtos_task *task = (struct rtos_task *)malloc(sizeof(struct rtos_task));
	rtos_task_insert(&rtos_ready_tasks, task);
	task->function = func;
	task->arg = arg;
}

void rtos_task_delete(struct rtos_task *task){
	free(task);
}

void rtos_task_insert(struct rtos_task **list, struct rtos_task *task){
	task->next = *list;
	*list = task;
}

void rtos_task_remove(struct rtos_task **list, struct rtos_task *task){
	struct rtos_task *prev, *cur;
	
	if(*list == task){
		*list = (*list)->next;
	}
	else{
		for(prev = *list, cur = (*list)->next; 
			cur != task; 
			prev = cur, cur = cur->next);
		prev->next = cur->next;
	}
	task->next = 0;
}
