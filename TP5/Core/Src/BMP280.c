/*
 * BMP280.c
 *
 *  Created on: Nov 17, 2022
 *      Author: Sami Asfary Ensea
 */

#include "BMP280.h"


HAL_StatusTypeDef config_BMP280(void){
	uint8_t Config[2] = {BMP_REG_CONFIG,
				BMP_MODE_NORMAL | BMP_SAMPLING_16 << BMP_CONFIG_P | BMP_SAMPLING_16 << BMP_CONFIG_T};
	return HAL_I2C_Master_Transmit(&hi2c2, ADD_BMP280, Config, 2, HAL_MAX_DELAY);
}


uint8_t getID_BMP280(void){
	uint8_t Buffer[1] = {BMP_REG_ID};
	HAL_I2C_Master_Transmit(&hi2c2, ADD_BMP280, Buffer, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c2, ADD_BMP280, Buffer, 1, HAL_MAX_DELAY);
	return Buffer[0];
}

uint32_t getTemp_BMP280(void){
	uint8_t Buffer[3];
	Buffer[0] = BMP_REG_T_MSB;
	HAL_I2C_Master_Transmit(&hi2c2, ADD_BMP280, Buffer, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c2, ADD_BMP280, Buffer, 3, HAL_MAX_DELAY);
	return (((int32_t)Buffer[0])<<12) | (((int32_t)Buffer[1])<<4) | (((int32_t)Buffer[2])>>4);
}

uint32_t getPres_BMP280(void){
	uint8_t Buffer[3];
	Buffer[0] = BMP_REG_P_MSB;
	HAL_I2C_Master_Transmit(&hi2c2, ADD_BMP280, Buffer, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c2, ADD_BMP280, Buffer, 3, HAL_MAX_DELAY);
	return (((int32_t)Buffer[0])<<12) | (((int32_t)Buffer[1])<<4) | (((int32_t)Buffer[2])>>4);
}
























