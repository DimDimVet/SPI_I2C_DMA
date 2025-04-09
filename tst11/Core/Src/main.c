
#include "main.h"
#include "i2c.h"
#include "gpio.h"


uint8_t dataToSend[2] = {0xAA, 0xAF}; // Пример данных для отправки
uint8_t receivedData[2];

void SystemClock_Config(void);


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

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	HAL_Delay(10);
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	HAL_Delay(10);

	if(BufferCmp(dataToSend, receivedData, 2))
	{
		HAL_Delay(10);
	}
	else
	{
		HAL_Delay(10);
	}
}



int main(void)
{

  //HAL_Init();
	

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();

//		while(HAL_I2C_Slave_Receive(&hi2c1, receivedData, 2, HAL_MAX_DELAY)!= HAL_OK)
//	{
//		Error_Handler();
//	}
//	//
//	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
//	{};
//	//
//		if(HAL_I2C_Slave_Transmit(&hi2c1, dataToSend, 2, HAL_MAX_DELAY)
//			!= HAL_OK)
//	{
//		Error_Handler();
//	}

  while (1)
  {

		while(HAL_I2C_Slave_Receive(&hi2c1, receivedData, 2, HAL_MAX_DELAY)!= HAL_OK)
	{
		Error_Handler();
	}
	//
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{};
	//
		if(HAL_I2C_Slave_Transmit(&hi2c1, dataToSend, 2, HAL_MAX_DELAY)
			!= HAL_OK)
	{
		Error_Handler();
	}

   HAL_Delay(1000);
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
//вариант 1
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

}


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




//Основные функции HAL для работы с I2C:
//HAL_I2C_Master_Transmit(): передача данных от мастера к слейву. Эта функция используется для отправки данных от микроконтроллера (мастера) к периферийному устройству (слейву).
//HAL_I2C_Master_Receive(): прием данных от слейва к мастеру. Эта функция позволяет микроконтроллеру (мастеру) получать данные от периферийного устройства (слейва).
//HAL_I2C_Slave_Transmit(): передача данных от слейва к мастеру. Эта функция используется, когда микроконтроллер работает в режиме слейва и отправляет данные мастеру.
//HAL_I2C_Slave_Receive(): прием данных от мастера к слейву. Эта функция позволяет микроконтроллеру в режиме слейва получать данные от мастера.
