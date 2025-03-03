#include "app.h"

int main()
{
	int count=0;
	Init_SPI();
	
	while(1)
	{
		count++;
		if(count<1)
		{
			//ExecutorTerminal();
		}
		if(count>10000)
		{
			count=0;
		}
	}
	return 0;
}

void DMA1_Channel2_IRQHandler(void) 
{
    if (DMA1->ISR & DMA_ISR_TCIF2) 
		{
        DMA1->IFCR |= DMA_IFCR_CTCIF2; // Очистка флага
    }
		LED();
}

void DMA1_Channel3_IRQHandler() 
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
