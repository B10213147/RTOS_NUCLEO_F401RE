#include "stm32f4xx.h"                  // Device header
#include "rtos.h"
#include "led_driver.h"
#include "keys_driver.h"
#include "usart_driver.h"

int main(void){
	led_driver_init();
	keys_driver_init();
	usart_driver_init();
	rtos_init(1000);
	while(1);
	return 0;
}
