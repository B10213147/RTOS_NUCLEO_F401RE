/*
 * rtos_sched.h
 *
 *  Created on: Jan 24, 2017
 *      Author: Harvard Tseng
**/

#ifndef __RTOS_SCHED_H
#define __RTOS_SCHED_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "rtos.h"
	 
typedef enum{
	task_completed,
	task_running
}sch_statusType;

void rtos_sched(void);
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RTOS_SCHED_H */
