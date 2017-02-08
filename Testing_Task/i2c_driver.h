/*
 * i2c_driver.h
 *
 *  Created on: Jan 30, 2017
 *      Author: Harvard Tseng
**/
 
#ifndef __I2C_DRIVER_H
#define __I2C_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include "stm32f4xx_i2c.h"
	 
void i2c_driver_init(void);
uint8_t TM_I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address);
void TM_I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);
void TM_I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data);
void TM_I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __I2C_DRIVER_H */
