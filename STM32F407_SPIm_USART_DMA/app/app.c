#include "app.h"

char rezultStr [SIZESTR];
char* receivedChar;
uint32_t* str_In;
uint8_t tst;

void DMA1_Stream4_IRQHandler(void)
{
    LED7();
		
    if (DMA1->HISR & DMA_HISR_TCIF4)
    {
        DMA1->HIFCR |= DMA_HIFCR_CTCIF4; // Сбрасываем флаг
    }
}

void DMA1_Stream3_IRQHandler(void)
{
    LED6();
		
    if (DMA1->LISR & DMA_LISR_TCIF3)
    {
						str_In=Read_SPI2_DMA();
			
        DMA1->LIFCR |= DMA_LIFCR_CTCIF3; // Сбрасываем флаг
    }
}
/////////////////
void DMA2_Stream7_IRQHandler(void)
{
    if ((DMA2->HISR & DMA_HISR_TCIF7) == DMA_HISR_TCIF7)
    {
        DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
    }
		
    LED7();
}

void DMA2_Stream2_IRQHandler(void)
{
    if ((DMA2->LISR & DMA_LISR_TCIF2) == DMA_LISR_TCIF2)
    {
        ExecutorTerminal();
        DMA2->LIFCR |= DMA_LIFCR_CTCIF2;
        LED6();
    }
}

void ExecutorTerminal()
{
  receivedChar = DMA2_ReadChar(); // Читаем из консоли
	// отправляем в консоль
	snprintf(rezultStr, sizeof rezultStr, "%s%s",set_infoStr,(char*)str_In);
	DMA2_SetString(rezultStr);
}

/////////////////

int main()
{
    Init_LED();
    Init_USART1(BAUND_RATE);
    Init_SPI();

    while(1)
    {
				SPI2_DMA_TransmitReceive(receivedChar);
        //tst = SPI2_TransmitReceive(0xDF);//test net DMA
    }
		
    return 0;
}

