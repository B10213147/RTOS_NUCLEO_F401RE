/*
 * rtos_mail.c
 *
 *  Created on: Jan 20, 2017
 *      Author: Harvard Tseng
**/
 
#include "rtos_mail.h"

unsigned int rtos_mail_read(struct rtos_mail *mail, char *data, unsigned int num_bytes){
	unsigned int i;
	
	for(i = 0; i < num_bytes; i++){
		if(mail->begin == mail->end) break;
		data[i] = mail->data[mail->begin];
		mail->begin = (mail->begin + 1) % mail->length;
	}
	
	return i;
}

unsigned int rtos_mail_write(struct rtos_mail *mail, char *data, unsigned int num_bytes){
	unsigned int i, idx;
	
	for(i = 0; i < num_bytes; i++){
		idx = (mail->end + 1) % mail->length;
		if(idx == mail->begin) break;
		mail->data[mail->end] = data[i];
		mail->end = idx;
	}
	
	return i;
}
