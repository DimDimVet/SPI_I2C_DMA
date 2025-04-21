#include "app.h"

uint8_t tst;
char data_In_Base[SIZE_REVERC_DATA];
char* data_In=data_In_Base;

void DMA1_Channel2_IRQHandler() 
{
    if (DMA1->ISR & DMA_ISR_TCIF2) 
		{
				data_In=Read_SPI1_DMA1();

				SPI1_DMA1_TransmitReceive(data_In);
        DMA1->IFCR |= DMA_IFCR_CTCIF2; // Очистка флага
    }
		LED13();
}

void DMA1_Channel3_IRQHandler() 
{
		if (DMA1->ISR & DMA_ISR_TCIF3) 
		{
        DMA1->IFCR |= DMA_IFCR_CTCIF3; // Очистка флага
				LED13();
    }
}

int main()
{
	Init_LED();
	Init_SPI();

	while(1)
	{
		tst = SPI_TransmitReceive(0);	
	}
	return 0;
}