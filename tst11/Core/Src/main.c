
#include "main.h"
#include "i2c.h"
#include "gpio.h"

#define MAX_DELAY      0xFFFFFFFFU
#define I2C_GET_FLAG(__HANDLE__, __FLAG__) ((((uint8_t)((__FLAG__) >> 16U)) == 0x01U) ? \
                                                  (((((__HANDLE__)->SR1) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET) : \
                                                  (((((__HANDLE__)->SR2) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET))


#define I2C_CLEAR_STOPFLAG(__HANDLE__)           \
  do{                                                  \
    __IO uint32_t tmpreg = 0x00U;                      \
    tmpreg = (__HANDLE__)->SR1;              \
    SET_BIT((__HANDLE__)->CR1, I2C_CR1_PE);  \
    UNUSED(tmpreg);                                    \
  } while(0)


#define I2C_CLEAR_ADDRFLAG(__HANDLE__)    \
  do{                                           \
    __IO uint32_t tmpreg = 0x00U;               \
    tmpreg = (__HANDLE__)->SR1;       \
    tmpreg = (__HANDLE__)->SR2;       \
    UNUSED(tmpreg);                             \
  } while(0)



#define TICK_FREQ_1KHZ 1U
#define SLAVE_ADDR  0x68
I2C_HandleTypeDef hi2c1;

uint8_t dataToSend[2] = {0xAA, 0xAF}; // Пример данных для отправки
uint8_t receivedData[2];


	uint32_t pclk1;
	uint32_t freqrange;
	uint32_t ClockSpeed = 100000;



void SystemClock_Config(void);

//
int I2C_WaitOnFlagUntilTimeout1( uint32_t Flag)
{
  /* Wait until flag is set */
  while (I2C_GET_FLAG(I2C1, Flag) == 0)
  {

  }
  return 0;
}

//
int I2C_WaitOnRXNEFlagUntilTimeout1()
{

  while (I2C_GET_FLAG(I2C1, I2C_FLAG_RXNE) == 0)
  {

  }
  return 0;
}


/////////////////
int I2C_Slave_Receive(uint8_t *pData, uint16_t Size)
{

    /* Disable Pos */
		I2C1->CR1 |=0 << I2C_CR1_POS_Pos;

		I2C1->CR1 |=1 << I2C_CR1_ACK_Pos;
		
    /* Wait until ADDR flag is set */
    if (I2C_WaitOnFlagUntilTimeout1(I2C_FLAG_ADDR) != 0)
    {
      return 1;
    }

    /* Clear ADDR flag */
    I2C_CLEAR_ADDRFLAG(I2C1);

    while (Size > 0)
    {
      /* Wait until RXNE flag is set */
      if (I2C_WaitOnRXNEFlagUntilTimeout1() != 0)
      {
        /* Disable Address Acknowledge */
				I2C1->CR1 |=0 << I2C_CR1_ACK_Pos;
        return 1;
      }

      /* Read data from DR */
      *pData = (uint8_t)I2C1->DR;

      /* Increment Buffer pointer */
      pData++;

      /* Update counter */
      Size--;
    }

    /* Clear STOP flag */
    I2C_CLEAR_STOPFLAG(I2C1);

    /* Disable Address Acknowledge */
		I2C1->CR1 |=0 << I2C_CR1_ACK_Pos;
		
		return 0;
}



int main(void)
{
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
		
    while (1)
    {
		
					
        while(I2C_Slave_Receive(receivedData, 2)!= 0)
        {
            Error_Handler();
        }
        //
        while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
        {};
        //
        if(HAL_I2C_Slave_Transmit(&hi2c1, dataToSend, 2, MAX_DELAY)
                != HAL_OK)
        {
            Error_Handler();
        }

        //HAL_Delay(100);
				for (int i=0; i<1000;i++)
				{};
    }

}

void MX_GPIO_Init(void)
{
		RCC->APB2ENR |=1 << RCC_APB2ENR_IOPDEN_Pos;
		RCC->APB2ENR |=1 << RCC_APB2ENR_IOPAEN_Pos;
		RCC->APB2ENR |=1 << RCC_APB2ENR_IOPBEN_Pos;
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
	GPIO_InitStruct.Pin   = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	

}

#define I2C_RISE_TIME(__FREQRANGE__, __SPEED__)            (((__SPEED__) <= 100000U) ? ((__FREQRANGE__) + 1U) : ((((__FREQRANGE__) * 300U) / 1000U) + 1U))

#define I2C_SPEED(__PCLK__, __SPEED__, __DUTYCYCLE__)      (((__SPEED__) <= 100000U)? (I2C_SPEED_STANDARD((__PCLK__), (__SPEED__))) : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__)) & I2C_CCR_CCR) == 0U)? 1U : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__))) | I2C_CCR_FS))
																																	
#define I2C_SPEED_FAST(__PCLK__, __SPEED__, __DUTYCYCLE__) (((__DUTYCYCLE__) == I2C_DUTYCYCLE_2)? I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 3U) : (I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 25U) | I2C_DUTYCYCLE_16_9))
//#define I2C_SPEED_FAST(__PCLK__, __SPEED__, __DUTYCYCLE__) (((__DUTYCYCLE__) == I2C_DUTYCYCLE_2)? I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 3U) : (I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 25U) | I2C_DUTYCYCLE_16_9))
#define I2C_DUTYCYCLE_2 0x00000000U

#define I2C_STATE_NONE            ((uint32_t)(HAL_I2C_MODE_NONE)) 


void MX_I2C1_Init(void)
{
	RCC->APB1ENR|=1 << RCC_APB1ENR_I2C1EN_Pos;


I2C1->CR1 = 0;

	hi2c1.Instance             = I2C1;
//	hi2c1.Init.ClockSpeed      = 100000;
	//hi2c1.Init.DutyCycle       = I2C_DUTYCYCLE_2;
//	hi2c1.Init.OwnAddress1     = SLAVE_ADDR;
//	hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
//	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//	hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
//	if(HAL_I2C_Init(&hi2c1) != HAL_OK)
//	{
//		Error_Handler();
//	}
	
  /* Get PCLK1 frequency */
  pclk1 = HAL_RCC_GetPCLK1Freq();

  /* Calculate frequency range */
  freqrange = I2C_FREQRANGE(pclk1);

  /*---------------------------- I2Cx CR2 Configuration ----------------------*/
  /* Configure I2Cx: Frequency range */
  //MODIFY_REG(I2C1->CR2, I2C_CR2_FREQ, freqrange);
	I2C1->CR2|=freqrange << I2C_CR2_FREQ_Pos;
  /*---------------------------- I2Cx TRISE Configuration --------------------*/
  /* Configure I2Cx: Rise Time */
  //MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE, I2C_RISE_TIME(freqrange, ClockSpeed));
	I2C1->TRISE|=I2C_RISE_TIME(freqrange, ClockSpeed) << I2C_TRISE_TRISE_Pos;
  /*---------------------------- I2Cx CCR Configuration ----------------------*/
  /* Configure I2Cx: Speed */
  //MODIFY_REG(I2C1->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), I2C_SPEED(pclk1, ClockSpeed, I2C_DUTYCYCLE_2));
	I2C1->CCR|= I2C_SPEED(pclk1, ClockSpeed, I2C_DUTYCYCLE_2) << (I2C_CCR_FS_Pos | I2C_CCR_DUTY_Pos | I2C_CCR_CCR_Pos);
  /*---------------------------- I2Cx CR1 Configuration ----------------------*/
  /* Configure I2Cx: Generalcall and NoStretch mode */
  //MODIFY_REG(I2C1->CR1, (I2C_CR1_ENGC | I2C_CR1_NOSTRETCH), (I2C_GENERALCALL_DISABLE | I2C_NOSTRETCH_DISABLE));
	I2C1->CR1|= (0|0) << (I2C_CR1_ENGC_Pos | I2C_CR1_NOSTRETCH_Pos);
  /*---------------------------- I2Cx OAR1 Configuration ---------------------*/
  /* Configure I2Cx: Own Address1 and addressing mode */
  //MODIFY_REG(I2C1->OAR1, (I2C_OAR1_ADDMODE | I2C_OAR1_ADD0), (I2C_ADDRESSINGMODE_7BIT | SLAVE_ADDR));
	//MODIFY_REG(I2C1->OAR1, (I2C_OAR1_ADD1_7), (SLAVE_ADDR));
	I2C1->OAR1 =SLAVE_ADDR;
	I2C1->OAR1|=(I2C_ADDRESSINGMODE_7BIT) << (I2C_OAR1_ADDMODE_Pos);
	//I2C1->OAR1|=(I2C_ADDRESSINGMODE_7BIT | SLAVE_ADDR) << (I2C_OAR1_ADD8_9 | I2C_OAR1_ADD1_7);
  /*---------------------------- I2Cx OAR2 Configuration ---------------------*/
  /* Configure I2Cx: Dual mode and Own Address2 */
  //MODIFY_REG(I2C1->OAR2, (I2C_OAR2_ENDUAL | I2C_OAR2_ADD2), (I2C_DUALADDRESS_DISABLE | 0));
	I2C1->OAR2|=(I2C_DUALADDRESS_DISABLE | 0) << (I2C_OAR2_ENDUAL_Pos | I2C_OAR2_ADD2_Pos);
	
	hi2c1.ErrorCode = HAL_I2C_ERROR_NONE;
  hi2c1.State = HAL_I2C_STATE_READY;
  hi2c1.PreviousState = I2C_STATE_NONE;
  hi2c1.Mode = HAL_I2C_MODE_NONE;

I2C1->CR1 |= 1<<I2C_CR1_PE_Pos;


    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

}

void SystemClock_Config(void)
{

    RCC->CFGR=0;
    RCC->CFGR |= 16 << RCC_CFGR_PPRE1_Pos;
    RCC->CFGR |= 13 << RCC_CFGR_PPRE2_Pos;
    SysTick_Config(SystemCoreClock / (1000U / TICK_FREQ_1KHZ));

}


void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

