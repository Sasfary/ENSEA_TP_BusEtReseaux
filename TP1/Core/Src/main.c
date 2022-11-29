/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADD_BMP280 				0x77 << 1

#define BMP_MODE_NORMAL 		0b11
#define BMP_SAMPLING_SKIPPED	0b000
#define BMP_SAMPLING_1			0b001
#define BMP_SAMPLING_2			0b010
#define BMP_SAMPLING_4			0b011
#define BMP_SAMPLING_8			0b100
#define BMP_SAMPLING_16			0b111

#define BMP_CONFIG_T			5
#define BMP_CONFIG_P			2

#define RPI_BUFFER_SIZE 			20
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char Buff_Rpi_Rx[RPI_BUFFER_SIZE];
char Buff_Rpi_Tx[RPI_BUFFER_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){
		if(Buff_Rpi_Rx[1] == 'E' && Buff_Rpi_Rx[2] == 'T' && Buff_Rpi_Rx[3] == '_'){
			uint8_t Tx_Size;
			if(Buff_Rpi_Rx[0] == 'G'){

				switch(Buff_Rpi_Rx[4]){
				case 'T' :
					Tx_Size = sprintf(Buff_Rpi_Tx,"T=+%4.2f_C\r\n",((float)(rand()%50))/(rand()%10));//BMPget_Temp()
					break;
				case 'P' :
					Tx_Size = sprintf(Buff_Rpi_Tx,"P=%uPa\r\n",rand()%200000);//BMPget_Press();
					break;
				case 'K' :
					Tx_Size = sprintf(Buff_Rpi_Tx,"T=%10.5f\r\n",((float)(rand()%50))/(rand()%10));//BMPget_CoefK();
					break;
				case 'A' :
					Tx_Size = sprintf(Buff_Rpi_Tx,"T=%10.6f\r\n",((float)(rand()%2024))/(rand()%10));//BMPget_Angle();
					break;
				}
				HAL_UART_Transmit_DMA(&huart1, Buff_Rpi_Tx, Tx_Size);
			}else if(Buff_Rpi_Rx[0] == 'S' && Buff_Rpi_Rx[4] == 'K' && Buff_Rpi_Rx[5] == '='){
				Tx_Size = sprintf(Buff_Rpi_Tx,"SET_K=OK\r\n");
				HAL_UART_Transmit_DMA(&huart1, Buff_Rpi_Tx, Tx_Size);
			}
		}
		HAL_UART_Receive_DMA(&huart1, Buff_Rpi_Rx, 5);
	}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	/*
  uint8_t * Registre =0xD0;
  uint8_t * msg;*/


    /*
	uint8_t Registre[1] = {0xD0};
	uint8_t data[3];
	HAL_I2C_Master_Transmit(&hi2c1, ADD_BMP280, Registre, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, ADD_BMP280, data, 1, HAL_MAX_DELAY);
	printf("\r\n     %x\n\r",data[0]);

	HAL_Delay(100);

	uint8_t Config[2] = {0xF5,
			BMP_MODE_NORMAL | BMP_SAMPLING_16 << BMP_CONFIG_P | BMP_SAMPLING_2 << BMP_CONFIG_T};
	HAL_I2C_Master_Transmit(&hi2c1, ADD_BMP280, Config, 2, HAL_MAX_DELAY);


	HAL_I2C_Master_Transmit(&hi2c1, ADD_BMP280, Config, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, ADD_BMP280, data, 1, HAL_MAX_DELAY);
	printf("\r\n   %x",data[0]);
	HAL_Delay(10);*/


	HAL_UART_Receive_DMA(&huart1, Buff_Rpi_Rx, 5);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		//printf("HELLO");
/*
		HAL_I2C_Master_Transmit(&hi2c1, ADD_BMP280, Config, 2, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, ADD_BMP280, Config, 1, HAL_MAX_DELAY);
		HAL_I2C_Master_Receive(&hi2c1, ADD_BMP280, data, 1, HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,Buff_Rpi_Rx,1,HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1,Buff_Rpi_Rx,1,HAL_MAX_DELAY);
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
