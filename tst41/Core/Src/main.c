/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "gpio.h"


#define I2C_ADDRESS 0x68 << 1  // Адрес I2C устройства
uint8_t dataToSend[2] = {0x00, 0x07}; 
uint8_t receivedData[2];

void SystemClock_Config(void);

int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
	
	//вариант 2
	//старт передачи
		while (HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS, dataToSend, 2, HAL_MAX_DELAY) != HAL_OK)
	{
		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{
			/* Acknowledge failure occurs.
			 * Slave don't acknowledge its address. */
			Error_Handler();
		}
	}
	//ожидание конца передачи
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{};
	//старт приема
		while(HAL_I2C_Master_Receive(&hi2c1, I2C_ADDRESS, receivedData, 2, HAL_MAX_DELAY) != HAL_OK)
	{
		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{
			/* Acknowledge failure occurs.
			 * Slave don't acknowledge its address. */
			Error_Handler();
		}
	}

	
  while (1)
  {
////// Отправка данных
////        if (HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS, dataToSend, 2, HAL_MAX_DELAY) != HAL_OK) {
////            //printf("Error during transmission\r\n");
////        }

////        HAL_Delay(1000);

////        // Чтение данных
////        if (HAL_I2C_Master_Receive(&hi2c1, I2C_ADDRESS, receivedData, 2, HAL_MAX_DELAY) != HAL_OK) {
////            //printf("Error during reception\r\n");
////        } else {
////            //printf("Received Data: 0x%02X 0x%02X\r\n", receivedData[0], receivedData[1]);
////        }

////        HAL_Delay(1000);
  }

}

uint8_t BufferCmp(uint8_t* pBuff1, uint8_t* pBuff2, uint16_t len)
{
	while (len--)
	{
		if((*pBuff1) != *pBuff2)
		{
			return 1;
		}
		pBuff1++;
		pBuff2++;
	}
	return 0;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c1)
{
	HAL_Delay(10);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	HAL_Delay(10);

	/* Compare the sent and received buffer */
	if(BufferCmp(dataToSend, receivedData, 2))
	{
		HAL_Delay(10);
	}
	else
	{
		HAL_Delay(10);
	}
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
//версия 1
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = 8;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }

//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//  {
//    Error_Handler();
//  }

//версия 2
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM       = 8;
	RCC_OscInitStruct.PLL.PLLN       = 336;
	RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ       = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK |
			RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
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




//#define I2C_ADDRESS 0x68 << 1  // Адрес I2C устройства
//    uint8_t dataToSend[2] = {0x00, 0x07}; // Пример данных для отправки
//    uint8_t receivedData[2];
//  while (1)
//  {
//      // Отправка данных
////        if (HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS, dataToSend, 2, HAL_MAX_DELAY) != HAL_OK)
////				{
////            //printf("Error during transmission\r\n");
////        }

////        HAL_Delay(1000);

//        // Чтение данных
//        if (HAL_I2C_Master_Receive(&hi2c1, I2C_ADDRESS, receivedData, 2, HAL_MAX_DELAY) != HAL_OK) 
//				{
//            //printf("Error during reception\r\n");
//						HAL_Delay(1);
//        } 
//				else 
//				{
//            //printf("Received Data: 0x%02X 0x%02X\r\n", receivedData[0], receivedData[1]);
//						HAL_Delay(1);
//        }

//        HAL_Delay(1000);
//  }
