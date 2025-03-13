#include "app.h"

char sendData = 0xA7; 
char receivedData = 0;

int main()
{
	int count=0;
	Init_USART(BAUND_RATE);
	Init_SPI();

	DMA1_SPI_SetString(tstSPIStr,10);
	DMA1_SPI_GetString(tstSPIStr2,10);
	while(1)
	{
			DMA1_SPI_SetString(tstSPIStr,10);
		delay_s(1);
		//Receive_Data();
		//delay_ms(10);
	}
	return 0;
}

void Receive_Data()
{
		SPI_Transmit(sendData);
		
		
		if(receivedData!=0)
		{
			//sendData = 0xFE;
			//SPI_Transmit(receivedData);
		}

		//receivedData=SPI_Receive();
}


void DMA1_Channel2_IRQHandler() 
{
    if (DMA1->ISR & DMA_ISR_TCIF2) 
		{
        DMA1->IFCR |= DMA_IFCR_CTCIF2; // Очистка флага
    }
		LED();
}

void DMA1_Channel3_IRQHandler() 
{
		if (DMA1->ISR & DMA_ISR_TCIF3) 
		{
        DMA1->IFCR |= DMA_IFCR_CTCIF3; // Очистка флага

				//DMA1_SPI_SetString(tstSPIStr);
				LED();
    }
		
	  //
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
