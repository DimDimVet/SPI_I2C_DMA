#include "app.h"

uint8_t sendData = 0xA5; 
uint8_t receivedData = 0;

int main()
{
	int count=0;
	Init_USART(BAUND_RATE);
	Init_SPI();
	
	while(1)
	{
		Receive_Data();
	}
	return 0;
}

void Receive_Data()
{
		SPI_Transmit(sendData);
		receivedData=SPI_Receive();
}

//
void DMA1_Channel4_IRQHandler() 
{
    if (DMA1->ISR & DMA_ISR_TCIF4) 
		{
        DMA1->IFCR |= DMA_IFCR_CTCIF4; // Очистка флага
				
    }
		LED();
}

void DMA1_Channel5_IRQHandler() 
{
		ExecutorTerminal();
	  //
}

void ExecutorTerminal()
{
		if (DMA1_GetStatus())// Проверка на получение
		{ 
        receivedChar = DMA1_ReadChar(); // Читаем

				if(receivedChar==CHAR_COMMAND1)
				{
						snprintf(rezultStr, sizeof rezultStr, "%s command: %c", greetingsStr,receivedChar);
						DMA1_SetString(rezultStr);
				}
				else if(receivedChar==CHAR_COMMAND0)
				{
						snprintf(rezultStr, sizeof rezultStr, "%s command: %c", partingStr,receivedChar);
						DMA1_SetString(rezultStr);
				}
				else
				{
						snprintf(rezultStr, sizeof rezultStr, "%s: %c", errorStr,receivedChar);
						DMA1_SetString(rezultStr);
				}
				
				LED();
    }
}
