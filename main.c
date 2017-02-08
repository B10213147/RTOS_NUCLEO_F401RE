#include "stm32f4xx.h"                  // Device header
#include "rtos.h"
#include "led_driver.h"
#include "usart_driver.h"

extern struct rtos_mail led_mail;
extern struct rtos_mail usart_mail;

int main(void){
	led_driver_init();
	usart_driver_init();
	rtos_task_create(led_driver, 0);
	rtos_task_create(usart_driver, 0);
	rtos_init(1000);

	while(1){
		char temp;
		if(rtos_mail_read(&usart_mail, &temp, 1) == 1){
			rtos_mail_write(&led_mail, &temp, 1);
		}
	}
	return 0;
}
