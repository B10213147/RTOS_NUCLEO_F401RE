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

/* rtos kernel API */
void rtos_init(uint32_t slice);

/* rtos scheduler API */	 
void rtos_sched(void);
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RTOS_H */
