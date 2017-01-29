#include "stm32f4xx.h"                  // Device header
#include "rtos.h"
#include "keys_driver.h"
#include "led_driver.h"
#include "usart_driver.h"

int main(void){
	keys_driver_init();
	led_driver_init();
	usart_driver_init();
	rtos_task_create(led_driver, 0);
	rtos_task_create(usart_driver, 0);
	rtos_task_create(keys_driver, 0);	
	rtos_task_create(led_driver, 0);	
	rtos_task_create(led_driver, 0);
	rtos_init(1000);

	while(1){
	}
	return 0;
}
