/*
 * BMP280.h
 *
 *  Created on: Nov 17, 2022
 *      Author: Sami Asfary Ensea
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include "main.h"

#define ADD_BMP280 				0x77 << 1

#define BMP_MODE_NORMAL 		0b11
#define BMP_SAMPLING_SKIPPED	0b000
#define BMP_SAMPLING_1			0b001
#define BMP_SAMPLING_2			0b010
#define BMP_SAMPLING_4			0b011
#define BMP_SAMPLING_8			0b100
#define BMP_SAMPLING_16			0b101

#define BMP_CONFIG_T			5
#define BMP_CONFIG_P			2

#define BMP_REG_CONFIG			0xF5
#define BMP_REG_ID				0xD0

#define BMP_REG_T_MSB			0xFA
#define BMP_REG_P_MSB			0xF7

extern I2C_HandleTypeDef hi2c2;

HAL_StatusTypeDef config_BMP280(void);
uint8_t getID_BMP280(void);
uint32_t getTemp_BMP280(void);
uint32_t getPres_BMP280(void);


#endif /* INC_BMP280_H_ */
