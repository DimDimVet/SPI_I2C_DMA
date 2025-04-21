#include "app.h"

char rezultStrBase[SIZESTR];
char *rezultStr=rezultStrBase;

char receivedStringConsoleBase[SIZESTR];
char *receivedStringConsole=receivedStringConsoleBase;

char receivedStringSPIBase[SIZESTR];
char *receivedStringSPI=receivedStringSPIBase;

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
						receivedStringSPI=(char*)Read_SPI2_DMA();
			
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
				delay_ms(100);
        LED6();
				//SPI2_DMA_TransmitReceive(receivedStringConsole);
    }
}

void ExecutorTerminal()
{
	
//	  while (!(USART2->SR & USART_SR_TXE))//Проверим окончание передачи
//		{
//		}
    USART2->DR = 0x11;
	
  DMA2_USART1_ReadChar(receivedStringConsole); // Читаем из консоли
	// отправляем в консоль
	snprintf(rezultStr, SIZESTR, "%s%s-%s",set_infoStr,receivedStringConsole,receivedStringSPI);
	
	SPI2_DMA_TransmitReceive(rezultStr);
	delay_ms(500);
	DMA2_USART1_SetString(rezultStr);
	//
}

/////////////////

int main()
{
    Init_LED();
    Init_USART1(BAUND_RATE);
    Init_SPI();

    while(1)
    {
        //tst = SPI2_TransmitReceive(0xDF);//test net DMA
    }
		
    return 0;
}

