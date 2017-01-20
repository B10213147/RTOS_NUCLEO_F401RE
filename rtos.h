/*
 * rtos.h
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/

#ifndef __RTOS_H
#define __RTOS_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
#include <stdint.h>
	 
typedef void (*voidfuncptr)(void);
	 
typedef enum sch_status{
	task_completed,
	task_running
}sch_statusType;

/* rtos kernel API */
void rtos_init(uint32_t slice);

/* rtos scheduler API */	 
void rtos_sched(void);
extern voidfuncptr priv_task;
extern voidfuncptr sch_tab[];
extern int sch_tab_length;
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RTOS_H */
