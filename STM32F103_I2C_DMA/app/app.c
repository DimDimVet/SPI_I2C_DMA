#include "app.h"

uint8_t dataToSend[2] = {0xAA, 0xAF}; // Пример данных для отправки
uint8_t receivedData[2];


int I2C_AdresSetTime()
{
  //ждем адрес
  while (!(I2C1->SR1 & I2C_SR1_ADDR))
  {
  }
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

int I2C_TX_SetTime()
{
	//ждем передачу данных
  while (!(I2C1->SR1 & I2C_SR1_TXE) )
  {
  }
  return 0;
}

int I2C_Slave_Receive(uint8_t *pData, uint16_t Size)
{
	//откл POS
		I2C1->CR1 &=~I2C_CR1_POS;
	//вкл проверку адреса
		I2C1->CR1 |=I2C_CR1_ACK;
		
  //проверим адрес
	if (I2C_AdresSetTime() != 0)
    {
      return 1;
    }
		
	//сброс флага адреса
		I2C1->SR2;	
		
		while (Size > 0)//крутим
    {
      //проверим данные
      if (I2C_RX_SetTime() != 0)
      {
        //откл проверку адреса
				I2C1->CR1 &=~I2C_CR1_ACK;
        return 1;
      }

      //читаем данные
      *pData = (uint8_t)I2C1->DR;
      pData++;
      Size--;
    }

		//сброс флага
		I2C1->SR1;
		
    //откл проверку адреса
		I2C1->CR1 &=~I2C_CR1_ACK;
		
		return 0;
}
////////////////////
int I2C_Slave_Transmit(uint8_t *pData, uint16_t Size)
{
	//откл POS
		I2C1->CR1 &=~I2C_CR1_POS;
	//вкл проверку адреса
		I2C1->CR1 |=I2C_CR1_ACK;

  //проверим адрес
	if (I2C_AdresSetTime() != 0)
    {
      return 1;
    }
		
	//сброс флага адреса
		I2C1->SR2;

    while (Size > 0U)//крутим
    {
      //проверим данные
      if (I2C_TX_SetTime() != 0)
      {
        //откл проверку адреса
				I2C1->CR1 &=~I2C_CR1_ACK;
        return 1;
      }

      //запишем данные
      I2C1->DR = *pData;
      pData++;
      Size--;
    }

		//сброс флага
		I2C1->SR1;

    //откл проверку адреса
		I2C1->CR1 &=~I2C_CR1_ACK;

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
        while(I2C_Slave_Receive(receivedData, 2)!= 0)
        {
            Error_Handler();
        }

				//delay_us(100);
					
        if(I2C_Slave_Transmit(dataToSend, 2)!= 0)
        {
            Error_Handler();
        }
				
				delay_ms(100);
	}
	return 0;
}
