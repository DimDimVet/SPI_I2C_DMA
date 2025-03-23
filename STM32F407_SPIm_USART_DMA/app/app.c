#include "app.h"

#define SIZESTRS1 1

uint8_t sizeSPI=SIZESTRS1;
uint32_t dataBufSPI=0xAA;
uint32_t dataBufSPIRx;
static uint8_t _filler = 0xFF;
uint32_t tst;

uint8_t txBuffer[] = {0xAB}; // Данные для передачи
uint8_t rxBuffer[1]; // Буфер для приема данных

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
				DMA1_Stream3->M0AR = (uint32_t)tst;
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
        receivedChar = DMA2_ReadChar(); // Читаем
				tst= SPI2_DMA_TransmitReceive(receivedChar,1);
				snprintf(rezultStr, sizeof rezultStr, "%s: %c", set_infoStr,tst);
				DMA2_SetString(rezultStr);
}

/////////////////

int main()
{
    Init_LED();
    Init_SPI();
		Init_USART1(BAUND_RATE);
	
    int i;
    

    while(1)
    {
        i++;

        tst= SPI2_DMA_TransmitReceive(i,1);
        if(i>100)
        {
            i=0;
        }
        //tst = SPI2_TransmitReceive(i);
    }
    return 0;
}

