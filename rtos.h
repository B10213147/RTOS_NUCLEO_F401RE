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
	 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "rtos_sched.h"
#include "rtos_task.h"
#include "rtos_mail.h"

/* rtos kernel */
void rtos_init(uint32_t slice);
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RTOS_H */
