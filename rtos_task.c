/*
 * rtos_task.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/

#include "rtos_task.h"

void empty(void){
}
	
voidfuncptr priv_task = empty;
struct rtos_task *rtos_running_task;
struct rtos_task *rtos_ready_tasks = NULL;

void rtos_task_create(voidfuncptr func, void *arg){
	struct rtos_task *task = (struct rtos_task *)malloc(sizeof(struct rtos_task));
	rtos_task_insert(&rtos_ready_tasks, task);
	task->function = func;
	task->arg = arg;
}

void rtos_task_delete_self(struct rtos_task *task){
	free(task);
}

void rtos_task_delete(voidfuncptr func){
	struct rtos_task *prev, *cur, *del_list = NULL;
	struct rtos_task **list = &rtos_ready_tasks;
	
	prev = *list, cur = *list;
	while(cur != NULL){	
		if(cur->function == func){
			rtos_task_remove(list, cur);
			rtos_task_insert(&del_list, cur);
			cur = *list;
		}
		else{
			prev = cur;
			cur = cur->next;
		}
	}
		
	while(del_list != NULL){
		struct rtos_task *del = del_list;
		rtos_task_remove(&del_list, del);
		free(del);
	}
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
