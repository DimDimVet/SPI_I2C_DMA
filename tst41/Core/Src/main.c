
#include "main.h"
#include "i2c.h"
#include "gpio.h"

#define I2C_STATE_NONE            ((uint32_t)(HAL_I2C_MODE_NONE)) 
#define I2C_TIMEOUT_BUSY_FLAG     25U         /*!< Timeout 25 ms             */
#define I2C_STATE_MSK             ((uint32_t)((uint32_t)((uint32_t)HAL_I2C_STATE_BUSY_TX | (uint32_t)HAL_I2C_STATE_BUSY_RX) & (uint32_t)(~((uint32_t)HAL_I2C_STATE_READY)))) 
#define I2C_STATE_MASTER_BUSY_RX  ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_MASTER))
#define I2C_NO_OPTION_FRAME       0xFFFF0000U /*!< XferOptions default value */

#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) ((((uint8_t)((__FLAG__) >> 16U)) == 0x01U) ? \
                                                  (((((__HANDLE__)->Instance->SR1) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET) : \
                                                  (((((__HANDLE__)->Instance->SR2) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET))



#define I2C_ADDRESS 0x68  // Адрес I2C устройства
uint8_t dataToSend[2] = {0x68, 0xf7}; 
uint8_t receivedData[2];

void SystemClock_Config(void);


#define I2C_7BIT_ADD_WRITE(__ADDRESS__)                    ((uint8_t)((__ADDRESS__) & (uint8_t)(~I2C_OAR1_ADD0)))


int I2C_AdresSetTime()
{
  //ждем адрес
  while (!(I2C1->SR1 & I2C_SR1_ADDR))
  {
  }
  return 0;
}

int I2C_TX_SetTime()
{
	//ждем передачу данных
  while (!(I2C1->SR1 & I2C_SR1_TXE) )
  {
  }
  return 0;
}


int I2C_StartBit_SetTime()
{
	//ждем передачу данных
  while (!(I2C1->SR1 & I2C_SR1_SB) )
  {
  }
  return 0;
}

int I2C_BTFBit_SetTime()
{
	//ждем передачу данных
  while (!(I2C1->SR1 & I2C_SR1_BTF))
  {
  }
  return 0;
}

//HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeoutT(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart)
//{
//  /* Wait until flag is set */
//  //while (__HAL_I2C_GET_FLAG(hi2c, Flag) == 0)
//	while (!(I2C1->SR1 & I2C_SR1_BTF) )
//  {
//		
//		
//		//BTF flag
//    /* Check for the Timeout */
////    if (Timeout != HAL_MAX_DELAY)
////    {
////      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
////      {
////        hi2c->PreviousState     = I2C_STATE_NONE;
////        hi2c->State             = HAL_I2C_STATE_READY;
////        hi2c->Mode              = HAL_I2C_MODE_NONE;
////        hi2c->ErrorCode         |= HAL_I2C_ERROR_TIMEOUT;

////        /* Process Unlocked */
////        __HAL_UNLOCK(hi2c);

////        return HAL_ERROR;
////      }
////    }
//  }
////I2C_StartBit_SetTime();
//  return 0;
//}


int I2C_MasterRequestRead(uint16_t DevAddress)
{

	I2C1->CR1 |=1 << I2C_CR1_ACK_Pos;

	I2C1->CR1 |=1 << I2C_CR1_START_Pos;

	I2C_StartBit_SetTime();
	
	I2C1->DR =I2C_7BIT_ADD_READ(DevAddress);

	I2C_AdresSetTime();
  return 0;
}


int I2C_RX_SetTime()
{
	//ждем прием данных
  while (!(I2C1->SR1 & I2C_SR1_RXNE) )
  {
  }
  return 0;
}

int I2C_MasterRequestWriteT(uint16_t DevAddress)
{

	I2C1->CR1 |=1 << I2C_CR1_START_Pos;
	
	I2C_StartBit_SetTime();

	I2C1->DR =I2C_7BIT_ADD_WRITE(DevAddress);

	I2C_AdresSetTime();

  return 0;
}

///////////////
HAL_StatusTypeDef HAL_I2C_Master_ReceiveT(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  /* Init tickstart for timeout management*/
  //uint32_t tickstart = HAL_GetTick();

		I2C1->CR1 |= 1 << I2C_CR1_PE_Pos;
		
		    /* Disable Pos */
		I2C1->CR1 |= 0 << I2C_CR1_POS_Pos;
		
    /* Send Slave Address */
    if (I2C_MasterRequestRead(DevAddress) != 0)
    {
      return 1;
    }

    if (Size == 0U)
    {
			I2C1->SR2;
			
			/* Generate Stop */
			I2C1->CR1 |=1 << I2C_CR1_STOP_Pos;
    }
    else if (Size == 1U)
    {
      //откл проверку адреса
			I2C1->CR1 &= ~(1 << I2C_CR1_ACK_Pos);

			//сброс флага адреса
			I2C1->SR2;

			/* Generate Stop */
			I2C1->CR1 |=1 << I2C_CR1_STOP_Pos;
    }
    else if (Size == 2U)
    {
      //откл проверку адреса
			I2C1->CR1 &= ~(1 << I2C_CR1_ACK_Pos);

			//откл проверку адреса
			I2C1->CR1 |=0 << I2C_CR1_ACK_Pos;
			
      /* Enable Pos */
			I2C1->CR1 |=1 << I2C_CR1_POS_Pos;

			//сброс флага адреса
			I2C1->SR2;
    }
    else
    {
			//вкл проверку адреса
			I2C1->CR1 |=1 << I2C_CR1_ACK_Pos;

			//сброс флага адреса
			I2C1->SR2;
    }

    while (Size > 0U)
    {
      if (Size <= 3U)
      {

        if (Size == 2U)
        {
		
					/* Wait until BTF flag is set */
//          if (I2C_WaitOnFlagUntilTimeoutT(hi2c, I2C_FLAG_BTF, 0, Timeout, tickstart) != 0)
//          {
//            return HAL_ERROR;
//          }
					I2C_BTFBit_SetTime();
					
					/* Generate Stop */
					I2C1->CR1 |=1 << I2C_CR1_STOP_Pos;
		
          /* Read data from DR */
          *pData = (uint8_t)I2C1->DR;
          pData++;
          Size--;

          /* Read data from DR */
          *pData = (uint8_t)I2C1->DR;
          pData++;
          Size--;
        }
      }
      else
      {
        /* Wait until RXNE flag is set */
        if (I2C_RX_SetTime() != 0)
        {
          return 1;
        }

          /* Read data from DR */
          *pData = (uint8_t)I2C1->DR;
          pData++;
          Size--;

      }
    }

    return 0;
}


//////////////////////////
int HAL_I2C_Master_TransmitT(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{

		I2C1->CR1 |= 1 << I2C_CR1_PE_Pos;
			
    /* Disable Pos */
		I2C1->CR1 |= 0 << I2C_CR1_POS_Pos;

    /* Send Slave Address */
    if (I2C_MasterRequestWriteT(DevAddress) != 0)
    {
      return 1;
    }

			//сброс флага адреса
		I2C1->SR2;
		
    while (Size > 0U)
    {
      /* Write data to DR */
      I2C1->DR = *pData;
			
      /* Increment Buffer pointer */
      pData++;
      Size--;

      //проверим данные
      if (I2C_TX_SetTime() != 0)
      {
        //откл проверку адреса
				I2C1->CR1 |=0 << I2C_CR1_ACK_Pos;
        return 1;
      }
    }

    /* Generate Stop */
		I2C1->CR1 |=1 << I2C_CR1_STOP_Pos;

    return 0;
}



///////////////////////////////

int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
	
  while (1)
  {
////// Отправка данных
	//старт передачи
		while (HAL_I2C_Master_TransmitT(I2C_ADDRESS, dataToSend, 2) != HAL_OK)
	{
			Error_Handler();
	}
//	//ожидание конца передачи
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{};
	//старт приема
	while(HAL_I2C_Master_ReceiveT(I2C_ADDRESS, receivedData, 2) != HAL_OK)
	{
//		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//		{
			Error_Handler();
//		}
	}

       HAL_Delay(1000);
//			 for (int i=0; i<1000000;i++)
//				{};
  }

}



/////////////////////////



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
//  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//  __HAL_RCC_PWR_CLK_ENABLE();
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
////версия 1
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
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//  {
//    Error_Handler();
//  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

