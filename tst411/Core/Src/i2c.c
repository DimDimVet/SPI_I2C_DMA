/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */
#define I2C_STATE_NONE            ((uint32_t)(HAL_I2C_MODE_NONE)) 
/* USER CODE END 0 */
#define CLOCK_SPEED 100000
#define I2C_ADDRESSINGMODE_7BIT         0x00004000U
  uint32_t freqrange;
  uint32_t pclk1;
	
I2C_HandleTypeDef hi2c1;

void MX_I2C1_Init(void)
{

	hi2c1.Instance             = I2C1;
	hi2c1.Init.ClockSpeed      = 100000;
	hi2c1.Init.DutyCycle       = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1     = 0x00;
	hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
//	if(HAL_I2C_Init(&hi2c1) != HAL_OK)
//	{
//		Error_Handler();
//	}	
	


  HAL_I2C_MspInit(&hi2c1);

  /*Reset I2C*/
  I2C1->CR1 |= I2C_CR1_SWRST;
  I2C1->CR1 &= ~I2C_CR1_SWRST;

  /* Get PCLK1 frequency */
  //pclk1 = HAL_RCC_GetPCLK1Freq();
	pclk1 =0x00F42400;
  freqrange = pclk1/1000000;

  /*---------------------------- I2Cx CR2 Configuration ----------------------*/
  /* Configure I2Cx: Frequency range */
  //MODIFY_REG(I2C1->CR2, I2C_CR2_FREQ, freqrange);
	I2C1->CR2 |= freqrange << I2C_CR2_FREQ_Pos;
  /*---------------------------- I2Cx TRISE Configuration --------------------*/
  /* Configure I2Cx: Rise Time */
  //MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE, I2C_RISE_TIME(freqrange, hi2c1.Init.ClockSpeed));
	I2C1->TRISE |= I2C_RISE_TIME(freqrange, CLOCK_SPEED) << I2C_TRISE_TRISE_Pos;
  /*---------------------------- I2Cx CCR Configuration ----------------------*/
  /* Configure I2Cx: Speed */
  //MODIFY_REG(I2C1->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), I2C_SPEED(pclk1, hi2c1.Init.ClockSpeed, hi2c1.Init.DutyCycle));
	I2C1->CCR |= I2C_SPEED(pclk1, CLOCK_SPEED, 0);// << (I2C_CCR_FS_Pos | I2C_CCR_DUTY_Pos | I2C_CCR_CCR_Pos);
//	I2C1->CCR |=pclk1 << I2C_CCR_CCR_Pos;
//	I2C1->CCR |=0 << I2C_CCR_FS_Pos;
//	I2C1->CCR |=hi2c1.Init.DutyCycle << I2C_CCR_DUTY_Pos;
	
  /*---------------------------- I2Cx CR1 Configuration ----------------------*/
  /* Configure I2Cx: Generalcall and NoStretch mode */
  //MODIFY_REG(I2C1->CR1, (I2C_CR1_ENGC | I2C_CR1_NOSTRETCH), (hi2c1.Init.GeneralCallMode | hi2c1.Init.NoStretchMode));
	I2C1->CR1 |= (0 | 0) << (I2C_CR1_ENGC_Pos | I2C_CR1_NOSTRETCH_Pos);
  /*---------------------------- I2Cx OAR1 Configuration ---------------------*/
  /* Configure I2Cx: Own Address1 and addressing mode */
  //MODIFY_REG(I2C1->OAR1, (I2C_OAR1_ADDMODE | I2C_OAR1_ADD8_9 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD0), (hi2c1.Init.AddressingMode | hi2c1.Init.OwnAddress1));
	I2C1->OAR1 |= (I2C_ADDRESSINGMODE_7BIT | 0) << (I2C_OAR1_ADDMODE_Pos|I2C_OAR1_ADD0_Pos);
  /*---------------------------- I2Cx OAR2 Configuration ---------------------*/
  /* Configure I2Cx: Dual mode and Own Address2 */
  //MODIFY_REG(I2C1->OAR2, (I2C_OAR2_ENDUAL | I2C_OAR2_ADD2), (hi2c1.Init.DualAddressMode | hi2c1.Init.OwnAddress2));
	I2C1->OAR2 |= (0 | 0) << (I2C_OAR2_ENDUAL_Pos | I2C_OAR2_ADD2_Pos);
//  /* Enable the selected I2C peripheral */
 //__HAL_I2C_ENABLE(&hi2c1);
I2C1->CR1 |= 1<<I2C_CR1_PE_Pos;
//  hi2c1.ErrorCode = HAL_I2C_ERROR_NONE;
  hi2c1.State = HAL_I2C_STATE_READY;
//  hi2c1.PreviousState = I2C_STATE_NONE;
//  hi2c1.Mode = HAL_I2C_MODE_NONE;
	
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();

//			GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
//			GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
//			GPIO_InitStruct.Pull      = GPIO_PULLUP;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
//			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    GPIOB->MODER |= 0 << GPIO_MODER_MODE6_Pos; // Очистка режима для PB6
    GPIOB->MODER |= 2 << GPIO_MODER_MODE6_Pos;   // Альтернативная функция для PB6 (SCL)
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT6_Pos;//открытый коллектор
		GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED6_Pos;//скорость
		//GPIOB->PUPDR |= 2 << GPIO_PUPDR_PUPD6_Pos;//подтянем+

    GPIOB->MODER |= 0 << GPIO_MODER_MODE7_Pos;  // Очистка режима для PB7
    GPIOB->MODER |= 2 << GPIO_MODER_MODE7_Pos;   // Альтернативная функция для PB7 (SDA)
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT7_Pos;//открытый коллектор
		GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED7_Pos;//скорость
		//GPIOB->PUPDR |= 2 << GPIO_PUPDR_PUPD7_Pos;//подтянем+
	
//		GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
//    GPIOB->MODER |= (2 << GPIO_MODER_MODER6_Pos | 2 << GPIO_MODER_MODER7_Pos); // Альтернативная функция
//	
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL6_Pos;// AF4 для I2C PB6 (SCL)
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL7_Pos;// AF4 для I2C PB7 (SDA)



		
    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
		
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);

  }
}


