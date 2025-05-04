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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

#define I2C_STATE_MASTER_BUSY_TX1  ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_TX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_MASTER))            /*!< Master Busy TX, combinaison of State LSB and Mode enum */
#define I2C_STATE_MASTER_BUSY_RX  ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_MASTER))            /*!< Master Busy RX, combinaison of State LSB and Mode enum */
#define I2C_TIMEOUT_BUSY_FLAG     25U
#define I2C_NO_OPTION_FRAME       0xFFFF0000U /*!< XferOptions default value */

	#define SLAVE_ADDR  0x68
	uint8_t dataToSend[5] = {0xAA, 0xAF, 0xA5, 0xA4, 0xA3}; // Пример данных для отправки
	uint8_t receivedData[5];
/////////////////////////////

uint8_t I2C_GET_FLAG(uint32_t I2C_FLAG_BTF1)
{
		
		if(((uint8_t)(I2C_FLAG_BTF1 >> 16U)) == 0x01U)
		{
				if(((I2C1->SR1) & (I2C_FLAG_BTF1 & I2C_FLAG_MASK)) == (I2C_FLAG_BTF1 & I2C_FLAG_MASK))
				{
					return 1;
				}
				else
				{
					return 0;
				}
		}
		else
		{
				if(((I2C1->SR2) & (I2C_FLAG_BTF1 & I2C_FLAG_MASK)) == (I2C_FLAG_BTF1 & I2C_FLAG_MASK))
				{
					return 1;
				}
				else
				{
					return 0;
				}
		}
}	


uint8_t I2C_RX_SetTime()
{
    //ждем прием данных
    while (!(I2C1->SR1 & I2C_SR1_RXNE) )
    {
    }
    return 0;
}

uint8_t I2C_BTFBit_SetTime()
{
    //ждем передачу данных
    while (!(I2C1->SR1 & I2C_SR1_BTF))
    {
    }
    return 0;
}
uint8_t I2C_BUSYBit_SetTime()//#define I2C_FLAG_BUSY                   0x00100002U
{
    //ждем передачу данных
	while (I2C_GET_FLAG(I2C_FLAG_BUSY) == I2C_TIMEOUT_BUSY_FLAG)
  {

  }
    return 0;
}





//ffff
HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeoutTT(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart)
{
  /* Wait until flag is set */
  while (__HAL_I2C_GET_FLAG(hi2c, Flag) == Status)
  {
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
      {
        //hi2c->PreviousState     = I2C_STATE_NONE;
        hi2c->State             = HAL_I2C_STATE_READY;
        hi2c->Mode              = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode         |= HAL_I2C_ERROR_TIMEOUT;

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
  }
  return HAL_OK;
}

static HAL_StatusTypeDef I2C_WaitOnMasterAddressFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, uint32_t Timeout, uint32_t Tickstart)
{
  while (__HAL_I2C_GET_FLAG(hi2c, Flag) == RESET)
  {
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET)
    {
      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

      /* Clear AF Flag */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

      //hi2c->PreviousState       = I2C_STATE_NONE;
      hi2c->State               = HAL_I2C_STATE_READY;
      hi2c->Mode                = HAL_I2C_MODE_NONE;
      hi2c->ErrorCode           |= HAL_I2C_ERROR_AF;

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
      {
        //hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
  }
  return HAL_OK;
}


static HAL_StatusTypeDef I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{

  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == RESET)
  {
    /* Check if a STOPF is detected */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == SET)
    {
      /* Clear STOP Flag */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

      //hi2c->PreviousState       = I2C_STATE_NONE;
      hi2c->State               = HAL_I2C_STATE_READY;
      hi2c->Mode                = HAL_I2C_MODE_NONE;
      hi2c->ErrorCode           |= HAL_I2C_ERROR_NONE;

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
    {
      //hi2c->PreviousState       = I2C_STATE_NONE;
      hi2c->State               = HAL_I2C_STATE_READY;
      hi2c->Mode                = HAL_I2C_MODE_NONE;
      hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }
  }
  return HAL_OK;
}

static HAL_StatusTypeDef I2C_MasterRequestRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout, uint32_t Tickstart)
{
  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  uint32_t CurrentXferOptions = hi2c->XferOptions;

  /* Enable Acknowledge */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

  /* Generate Start condition if first transfer */
  if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_FIRST_FRAME)  || (CurrentXferOptions == I2C_NO_OPTION_FRAME))
  {
    /* Generate Start */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
  }
//  else if (hi2c->PreviousState == I2C_STATE_MASTER_BUSY_TX1)
//  {
//    /* Generate ReStart */
//    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
//  }
//  else
//  {
//    /* Do nothing */
//  }

  /* Wait until SB flag is set */
  if (I2C_WaitOnFlagUntilTimeoutTT(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)
  {
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)
    {
      hi2c->ErrorCode = HAL_I2C_WRONG_START;
    }
    return HAL_TIMEOUT;
  }

  if (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
  {
    /* Send slave address */
    hi2c->Instance->DR = I2C_7BIT_ADD_READ(DevAddress);
  }
  else
  {
    /* Send header of slave address */
    hi2c->Instance->DR = I2C_10BIT_HEADER_WRITE(DevAddress);

    /* Wait until ADD10 flag is set */
    if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADD10, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Send slave address */
    hi2c->Instance->DR = I2C_10BIT_ADDRESS(DevAddress);

    /* Wait until ADDR flag is set */
    if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Clear ADDR flag */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

    /* Generate Restart */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

    /* Wait until SB flag is set */
    if (I2C_WaitOnFlagUntilTimeoutTT(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)
    {
      if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)
      {
        hi2c->ErrorCode = HAL_I2C_WRONG_START;
      }
      return HAL_TIMEOUT;
    }

    /* Send header of slave address */
    hi2c->Instance->DR = I2C_10BIT_HEADER_READ(DevAddress);
  }

  /* Wait until ADDR flag is set */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

																									
																									
																								

static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart)
{
  /* Wait until flag is set */
 // while (__HAL_I2C_GET_FLAG(hi2c, Flag) == Status)
	  while (I2C_GET_FLAG(Flag) == Status)
  {
    /* Check for the Timeout */
//    if (Timeout != HAL_MAX_DELAY)
//    {
////      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
////      {
////        //hi2c->PreviousState     = I2C_STATE_NONE;
////        hi2c->State             = HAL_I2C_STATE_READY;
////        hi2c->Mode              = HAL_I2C_MODE_NONE;
////        hi2c->ErrorCode         |= HAL_I2C_ERROR_TIMEOUT;

////        /* Process Unlocked */
////        __HAL_UNLOCK(hi2c);

////        return HAL_ERROR;
////      }
//    }
  }
  return HAL_OK;
}


uint8_t I2C_MasterRequestRead1(uint16_t DevAddress)
{

    I2C1->CR1 |=1 << I2C_CR1_ACK_Pos;

    I2C1->CR1 |=1 << I2C_CR1_START_Pos;

        while (!(I2C1->SR1 & I2C_SR1_SB) )
    {
    };

    I2C1->DR  =(uint8_t)(DevAddress | I2C_OAR1_ADD0);  //I2C_7BIT_ADD_READ(DevAddress);

        while (!(I2C1->SR1 & I2C_SR1_ADDR))
    {
    }
    return 0;
}



HAL_StatusTypeDef HAL_I2C_Master_ReceiveTT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* Init tickstart for timeout management*/
  //uint32_t tickstart = HAL_GetTick();

//  if (hi2c->State == HAL_I2C_STATE_READY)
//  {
    /* Wait until BUSY flag is reset */
//    if (I2C_WaitOnFlagUntilTimeoutTT(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)
//    {
//      return HAL_BUSY;
//    }
		I2C_BUSYBit_SetTime();

//    /* Process Locked */
//    __HAL_LOCK(hi2c);

    /* Check if the I2C is already enabled */
//    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
//    {
//      /* Enable I2C peripheral */
//      __HAL_I2C_ENABLE(hi2c);
//    }

//    /* Disable Pos */
//    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);
    /* Disable Pos */
    I2C1->CR1 |= 0 << I2C_CR1_POS_Pos;

//    hi2c->State       = HAL_I2C_STATE_BUSY_RX;
//    hi2c->Mode        = HAL_I2C_MODE_MASTER;
//    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;

//    /* Prepare transfer parameters */
//    hi2c->pBuffPtr    = pData;
//    hi2c->XferCount   = Size;
//    //hi2c->XferSize    = hi2c->XferCount;
//    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* Send Slave Address */
    if (I2C_MasterRequestRead1(DevAddress) != HAL_OK)
    {
      return HAL_ERROR;
    }

    if (Size == 0U)
    {
//      /* Clear ADDR flag */
//      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
        //сброс флага адреса
        I2C1->SR2;

//      /* Generate Stop */
//      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
        /* Generate Stop */
        I2C1->CR1 |=1 << I2C_CR1_STOP_Pos;
    }
    else if (Size == 1U)
    {
//      /* Disable Acknowledge */
//      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
        //откл проверку адреса
        I2C1->CR1 &= ~(1 << I2C_CR1_ACK_Pos);

//      /* Clear ADDR flag */
//      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
			        //сброс флага адреса
        I2C1->SR2;

//      /* Generate Stop */
//      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
                /* Generate Stop */
                I2C1->CR1 |=1 << I2C_CR1_STOP_Pos;
								
    }
    else if (Size == 2U)
    {
//      /* Disable Acknowledge */
//      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
        //откл проверку адреса
        I2C1->CR1 &= ~(1 << I2C_CR1_ACK_Pos);
				
//      /* Enable Pos */
//      SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);
        /* Enable Pos */
        I2C1->CR1 |=1 << I2C_CR1_POS_Pos;

//      /* Clear ADDR flag */
//      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
        //сброс флага адреса
        I2C1->SR2;
    }
    else
    {
//      /* Enable Acknowledge */
//      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
        //вкл проверку адреса
        I2C1->CR1 |=1 << I2C_CR1_ACK_Pos;

//      /* Clear ADDR flag */
//      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
			        //сброс флага адреса
        I2C1->SR2;
    }

    while (Size > 0U)
    {
      if (Size <= 3U)
      {
        /* One byte */
        if (Size == 1U)
        {

						I2C_RX_SetTime();

                /* Read data from DR */
                *pData = (uint8_t)I2C1->DR;
                pData++;
                Size--;
        }
        /* Two bytes */
        else if (Size == 2U)
        {

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
        /* 3 Last bytes */
        else
        {
					I2C_BTFBit_SetTime();
					
					        //откл проверку адреса
        I2C1->CR1 &= ~(1 << I2C_CR1_ACK_Pos);

                /* Read data from DR */
                *pData = (uint8_t)I2C1->DR;
                pData++;
                Size--;


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
					I2C_RX_SetTime();
                /* Read data from DR */
                *pData = (uint8_t)I2C1->DR;
                pData++;
                Size--;

        if (I2C_GET_FLAG(I2C_FLAG_BTF) == SET)
        {
                /* Read data from DR */
                *pData = (uint8_t)I2C1->DR;
                pData++;
                Size--;
        }
      }
    }

//    hi2c->State = HAL_I2C_STATE_READY;
//    hi2c->Mode = HAL_I2C_MODE_NONE;

//    /* Process Unlocked */
//    __HAL_UNLOCK(hi2c);

//    return HAL_OK;
//  }
//  else
//  {
//    return HAL_BUSY;
//  }
}

HAL_StatusTypeDef HAL_I2C_Master_ReceiveT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* Init tickstart for timeout management*/
 // uint32_t tickstart = HAL_GetTick();

//  if (hi2c->State == HAL_I2C_STATE_READY)
//  {
    /* Wait until BUSY flag is reset */
//    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)
//    {
//      return HAL_BUSY;
//    }
		I2C_BUSYBit_SetTime();
		
//    /* Process Locked */
//    __HAL_LOCK(hi2c);

    /* Check if the I2C is already enabled */
//    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
//    {
//      /* Enable I2C peripheral */
//      __HAL_I2C_ENABLE(hi2c);
//    }

    /* Disable Pos */
//    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);
		    I2C1->CR1 |= 0 << I2C_CR1_POS_Pos;

//    hi2c->State       = HAL_I2C_STATE_BUSY_RX;
//    hi2c->Mode        = HAL_I2C_MODE_MASTER;
//    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;

//    /* Prepare transfer parameters */
//    hi2c->pBuffPtr    = pData;
//    //hi2c->XferCount   = Size;
//    //hi2c->XferSize    = hi2c->XferCount;
//    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

//    /* Send Slave Address */
//    if (I2C_MasterRequestRead(hi2c, DevAddress, Timeout, tickstart) != HAL_OK)
//    {
//      return HAL_ERROR;
//    }
    /* Send Slave Address */
    if (I2C_MasterRequestRead1(DevAddress) != 0)
    {
      return 1;
    }

    if (Size == 0U)
    {
//      /* Clear ADDR flag */
//      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
        //сброс флага адреса
        I2C1->SR2;

//      /* Generate Stop */
//      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
        /* Generate Stop */
        I2C1->CR1 |=1 << I2C_CR1_STOP_Pos;
    }
    else if (Size == 1U)
    {
//      /* Disable Acknowledge */
//      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
        //откл проверку адреса
        I2C1->CR1 &= ~(1 << I2C_CR1_ACK_Pos);

//      /* Clear ADDR flag */
//      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
			        //сброс флага адреса
        I2C1->SR2;

//      /* Generate Stop */
//      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
                /* Generate Stop */
                I2C1->CR1 |=1 << I2C_CR1_STOP_Pos;
    }
    else if (Size == 2U)
    {
//      /* Disable Acknowledge */
//      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
        //откл проверку адреса
        I2C1->CR1 &= ~(1 << I2C_CR1_ACK_Pos);
				
//      /* Enable Pos */
//      SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);
        /* Enable Pos */
        I2C1->CR1 |=1 << I2C_CR1_POS_Pos;

//      /* Clear ADDR flag */
//      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
        //сброс флага адреса
        I2C1->SR2;
    }
    else
    {
//      /* Enable Acknowledge */
//      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
        //вкл проверку адреса
        I2C1->CR1 |=1 << I2C_CR1_ACK_Pos;

//      /* Clear ADDR flag */
//      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
			        //сброс флага адреса
        I2C1->SR2;
    }

    while (Size > 0U)
    {
      if (Size <= 3U)
      {
        /* One byte */
        if (Size == 1U)
        {

						I2C_RX_SetTime();

                /* Read data from DR */
                *pData = (uint8_t)I2C1->DR;
                pData++;
                Size--;
        }
        /* Two bytes */
        else if (Size == 2U)
        {

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
        /* 3 Last bytes */
        else
        {
					I2C_BTFBit_SetTime();
					
					        //откл проверку адреса
        I2C1->CR1 &= ~(1 << I2C_CR1_ACK_Pos);

                /* Read data from DR */
                *pData = (uint8_t)I2C1->DR;
                pData++;
                Size--;


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
					I2C_RX_SetTime();
                /* Read data from DR */
                *pData = (uint8_t)I2C1->DR;
                pData++;
                Size--;

        if (I2C_GET_FLAG(I2C_FLAG_BTF) == SET)
        {
                /* Read data from DR */
                *pData = (uint8_t)I2C1->DR;
                pData++;
                Size--;
        }
      }
    }

//    hi2c->State = HAL_I2C_STATE_READY;
//    hi2c->Mode = HAL_I2C_MODE_NONE;

//    /* Process Unlocked */
//    __HAL_UNLOCK(hi2c);

//    return HAL_OK;
//  }
//  else
//  {
//    return HAL_BUSY;
//  }
return 0;
}



//////////////////////////////
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDR, dataToSend, 5,HAL_MAX_DELAY);
		HAL_I2C_Master_ReceiveT(&hi2c1, SLAVE_ADDR, receivedData, 5,HAL_MAX_DELAY);
		HAL_Delay(100);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
