#include "app.h"

int size_SPI_Rx;
uint8_t sendData = 0xA5; 
char receivedData = 0;
char* receivedStr;

int main()
{
	Init_USART(BAUND_RATE);
	Init_SPI();
	
	while(1)
	{
//		Receive_Data();
//		ExecutorTerminal();
		//delay_s(1);
	}
	return 0;
}

void Receive_Data()
{
		receivedData=SPI_Receive();
		
		if(receivedData==0x7A)
		{
			sendData = 0xFE;
			SPI_Transmit(sendData);
		}
		else
		{
			sendData = 0x0E;
			SPI_Transmit(sendData);
		}
}


void DMA2_Stream3_IRQHandler(void) 
{
    if ((DMA2->LISR & DMA_LISR_TCIF3) == DMA_LISR_TCIF3)
		{
        DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
    }
		LED();
}

void DMA2_Stream0_IRQHandler(void) 
{
//					if(DMA2_SPI_GetStatus())
//						{	
								size_SPI_Rx=DMA2_SPI_ReadSize();
								//receivedStr=malloc(size_SPI_Rx * sizeof(char));
								receivedStr=DMA2_SPI_ReadData();
								//snprintf(rezultStr, sizeof rezultStr, "%s", receivedStr);
								//DMA2_SetString(rezultStr);
//						}
}
//
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
						snprintf(rezultStr, sizeof rezultStr, "%s command: %s", greetingsStr,receivedStr);
						DMA2_SetString(rezultStr);
				}
				else if(receivedChar==CHAR_COMMAND0)
				{
						snprintf(rezultStr, sizeof rezultStr, "%s command: %c", partingStr,receivedChar);
						DMA2_SetString(rezultStr);
				}
//				else if(receivedChar==CHAR_COMMAND5)
//				{
//						if(DMA2_SPI_GetStatus())
//						{	
//								receivedStr=DMA2_SPI_ReadData();
//								snprintf(rezultStr, sizeof rezultStr, "%c %s", sendData,receivedStr);
//								DMA2_SetString(rezultStr);
//						}
//						
//				}
				else
				{
						snprintf(rezultStr, sizeof rezultStr, "%s: %c", errorStr,receivedChar);
						DMA2_SetString(rezultStr);
				}
				
				LED();
    }
}
