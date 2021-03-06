/*
 * rtos_task.h
 *
 *  Created on: Jan 24, 2017
 *      Author: Harvard Tseng
**/

#ifndef __RTOS_TASK_H
#define __RTOS_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "rtos.h"
	 
typedef void (*voidfuncptr)();	
	 
struct rtos_task{
	struct rtos_task *next;
	voidfuncptr function;
	void *arg;	
};

void rtos_task_create(voidfuncptr func, void *arg);
void rtos_task_delete_self(struct rtos_task *task);
void rtos_task_delete(voidfuncptr func);
void rtos_task_insert(struct rtos_task **list, struct rtos_task *task);
void rtos_task_remove(struct rtos_task **list, struct rtos_task *task);

extern voidfuncptr priv_task;
extern struct rtos_task *rtos_running_task;
extern struct rtos_task *rtos_ready_tasks;
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RTOS_TASK_H */
