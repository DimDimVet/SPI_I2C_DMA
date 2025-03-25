#include "app.h"
uint8_t tst;
char data_In[30];
char data_Out[30]="FREANT";

void DMA1_Channel2_IRQHandler() 
{
    if (DMA1->ISR & DMA_ISR_TCIF2) 
		{
				Read_SPI1_DMA1(data_In);
				//data_Out=data_In;
				SPI1_DMA1_TransmitReceive(data_Out);
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
		//tst = SPI_TransmitReceive(0);	
	}
	return 0;
}