#include "app.h"


#define SIZESTR 1
uint8_t sizeSPI=SIZESTR;
uint32_t dataBufSPI=0xAA;
uint32_t dataBufSPIRx;
static uint8_t _filler = 0xFF;

	  uint8_t txBuffer[] = {0xAB}; // Данные для передачи
    uint8_t rxBuffer[1]; // Буфер для приема данных

   void DMA1_Stream4_IRQHandler(void)
	 {
	 LED7();
      if (DMA1->HISR & DMA_HISR_TCIF4) 
			{
//           // Обработка завершения передачи
//            DMA1_Stream3->CR &= ~(1 << DMA_SxCR_EN_Pos);	
//						DMA1_Stream3->NDTR = 10;//размер массива
//						DMA1_Stream3->M0AR = (uint32_t)dataBufSPIRx;// Адрес буфера
//						DMA1_Stream3->CR |= 1 << DMA_SxCR_EN_Pos;
						
			      DMA1->HIFCR |= DMA_HIFCR_CTCIF4; // Сбрасываем флаг
       }
   }

   void DMA1_Stream3_IRQHandler(void)
	 {
	 LED6();
       if (DMA1->LISR & DMA_LISR_TCIF3) 
			 {
//           // Обработка завершения приема
//							DMA1_Stream4->CR &= ~(1 << DMA_SxCR_EN_Pos);	
//							DMA1_Stream4->NDTR = 10;//размер массива
//							DMA1_Stream4->M0AR = (uint32_t)dataBufSPI;// Адрес буфера
//							DMA1_Stream4->CR |= 1 << DMA_SxCR_EN_Pos;
							
           DMA1->LIFCR |= DMA_LIFCR_CTCIF3; // Сбрасываем флаг
       }
   }


int main()
{
	Init_LED();
	Init_SPI();
	int i;
	uint32_t tst;

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

