/*
 * accelerometer_config.h
 *
 *  Created on: Jan 31, 2023
 *      Author: kovvalinio
 */

#ifndef INC_ACCELEROMETER_CONFIG_H_
#define INC_ACCELEROMETER_CONFIG_H_

#include <main.h>
#include <ism330dhcx.h>

//Accelerometer parameters configuration begin
#define ACC_ADDRESS ISM330DHCX_I2C_ADD_H
#define	ACC_AXES_ADDRESS 0x28U
#define ACC_ODR ISM330DHCX_XL_ODR_6667Hz
#define ACC_SCALE ISM330DHCX_2g
#define ACC_SENS ISM330DHCX_ACC_SENSITIVITY_FS_2G
//Accelerometer parameters configuration//


extern ism330dhcx_axis3bit16_t data_raw;

typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;

}acceleration;

void measure_and_add(ism330dhcx_axis3bit16_t *data);

#endif /* INC_ACCELEROMETER_CONFIG_H_ */
