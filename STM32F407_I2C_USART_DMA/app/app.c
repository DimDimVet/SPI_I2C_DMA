#include "app.h"

uint8_t txBuffer[BUFFER_SIZE] = "Hello I2C";
uint8_t rxBuffer[BUFFER_SIZE];
uint8_t* count_device;

#define I2C_ADDRESS 0x68  // Адрес I2C устройства
uint8_t dataToSend[2] = {0x68, 0xf7}; 
uint8_t receivedData[2];
/////////////////
int I2C_AdresSetTime()
{
  //ждем адрес
  while (!(I2C1->SR1 & I2C_SR1_ADDR))
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

int I2C_TX_SetTime()
{
	//ждем передачу данных
  while (!(I2C1->SR1 & I2C_SR1_TXE) )
  {
  }
  return 0;
}

int I2C_MasterRequestRead(uint16_t DevAddress)
{

	I2C1->CR1 |=1 << I2C_CR1_ACK_Pos;

	I2C1->CR1 |=1 << I2C_CR1_START_Pos;

	I2C_StartBit_SetTime();
	
	I2C1->DR =I2C_7BIT_ADD_READ(DevAddress);

	I2C_AdresSetTime();
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

int I2C_RX_SetTime()
{
	//ждем прием данных
  while (!(I2C1->SR1 & I2C_SR1_RXNE) )
  {
  }
  return 0;
}

int HAL_I2C_Master_ReceiveT(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
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

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}


int main()
{
    Init_LED();
    Init_I2C();

    while(1)
    {
//старт передачи
		while (HAL_I2C_Master_TransmitT(I2C_ADDRESS, dataToSend, 2) != 0)
	{
			Error_Handler();
	}
//	//ожидание конца передачи
//	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
//	{};
	//старт приема
	while(HAL_I2C_Master_ReceiveT(I2C_ADDRESS, receivedData, 2) != 0)
	{
//		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//		{
			Error_Handler();
//		}
	}

			
				
				delay_ms(1000);
    }
		
    return 0;
}