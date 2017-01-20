#include "stm32f4xx.h"                  // Device header
#include "rtos.h"

int main(void){
	rtos_init();
	while(1){
		for(int i = 0; i < 16000; i++){
			i++;
		}
	}
	return 0;
}
