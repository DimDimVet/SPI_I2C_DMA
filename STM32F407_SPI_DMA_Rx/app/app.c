#include "app.h"

int main()
{
	Init_USART(BAUND_RATE);

	while(1)
	{

	}
	return 0;
}


void DMA2_Stream7_IRQHandler(void) 
{
    if ((DMA2->HISR & DMA_HISR_TCIF7) == DMA_HISR_TCIF7)
		{
        DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
    }
		LED();
}

void DMA2_Stream2_IRQHandler(void) 
{
//    if ((DMA2->LISR & DMA_LISR_TCIF2) == DMA_LISR_TCIF2)
//		{
//        DMA2->LIFCR |= DMA_LIFCR_CTCIF2;
//				LED();
//    }
			ExecutorTerminal();
}

void ExecutorTerminal()
{
	
		if (DMA2_GetStatus())// Проверка на получение
		{ 
        receivedChar = DMA2_ReadChar(); // Читаем

				if(receivedChar==CHAR_COMMAND1)
				{
						snprintf(rezultStr, sizeof rezultStr, "%s command: %c", greetingsStr,receivedChar);
						DMA2_SetString(rezultStr);
				}
				else if(receivedChar==CHAR_COMMAND0)
				{
						snprintf(rezultStr, sizeof rezultStr, "%s command: %c", partingStr,receivedChar);
						DMA2_SetString(rezultStr);
				}
				else
				{
						snprintf(rezultStr, sizeof rezultStr, "%s: %c", errorStr,receivedChar);
						DMA2_SetString(rezultStr);
				}
				
				LED();
    }
}
