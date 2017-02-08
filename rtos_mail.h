/*
 * rtos_mail.h
 *
 *  Created on: Jan 24, 2017
 *      Author: Harvard Tseng
**/

#ifndef __RTOS_MAIL_H
#define __RTOS_MAIL_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
#include "rtos.h"

struct rtos_mail{
	unsigned int begin, end, length;
	char *data;
};

unsigned int rtos_mail_read(struct rtos_mail *mail, char *data, unsigned int num_bytes);
unsigned int rtos_mail_write(struct rtos_mail *mail, char *data, unsigned int num_bytes);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RTOS_MAIL_H */
