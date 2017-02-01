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
	 
void i2c_driver_init(void);
void i2c_Master_Transmitter(void);
void i2c_Master_Receiver(void);
void i2c_Slave_Transmitter(void);
void i2c_Slave_Receiver(void);
void test1(void);
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __I2C_DRIVER_H */
