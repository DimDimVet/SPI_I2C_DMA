#include "app.h"

uint8_t tst,tst1,tst2,tst3,tst4;
char data_In_Base[SIZE_REVERC_DATA];
char* data_In=data_In_Base;
volatile uint8_t rxIndex = 0;
volatile uint8_t txIndex = 0;





void SPI1_IRQHandler(void)
{
//	SPI1_ReadString(receivedStringSPI);
//	//delay_ms(1000);

//	snprintf(rezultStr, SIZESTR, "%s%s",set_infoStr,receivedStringSPI);
//	
//	SPI1_SetString(receivedStringSPI);
	
//	 if (SPI1->SR & SPI_SR_RXNE)
//		 {
//        receivedStringSPI[rxIndex++] = SPI1->DR;
//        if(rxIndex >= SIZE_REVERC_DATA) rxIndex = 0;
//		}
//		 
//    if ((SPI1->SR & SPI_SR_TXE) && (txIndex < SIZE_REVERC_DATA))
//			{
//        SPI1->DR = receivedStringSPI[txIndex++];
//        if(txIndex >= SIZE_REVERC_DATA) txIndex = 0;
//			}
	
	
	
//				  //Ждем, пока SPI освободится от предыдущей передачи

////	if (SPI1->SR & SPI_SR_RXNE)
////	{
//				while ((SPI1->SR & SPI_SR_RXNE))
//				{
//					receivedStringSPI[rxIndex] = SPI1->DR;
//					rxIndex++;
//					if(rxIndex >= 5)
//					{
//						rxIndex = 0; //break;
//						SPI1->CR2 &= ~SPI_CR2_RXNEIE;
//					}
//				};

////				};
//				////
//				tst=receivedStringSPI[0];
//				tst1=receivedStringSPI[1];
//				tst2=receivedStringSPI[2];
//				tst3=receivedStringSPI[3];
//				tst4=receivedStringSPI[4];
//				
//				
////	}
//	
////		if (SPI1->SR & SPI_SR_TXE)
////	{
//////				while ((SPI1->SR & SPI_SR_TXE))//Проверим окончание передачи
//////				{
////					SPI1->DR = receivedStringSPI[rxIndex++];
////					if(rxIndex >= SIZE_REVERC_DATA)
////					{
////						rxIndex = 0; //break;
////					}
//////				};
////	}
	
}

int main()
{
	Init_LED();
	Init_SPI();
  tst1=0x01;
	
	while(1)
	{
		tst=0;
		tst=SPI1_ReadBayt();
		tst3=tst;
		SPI1_SetBayt(tst3);
		//tst = SPI_TransmitReceive();	
		//SPI1_SetString(receivedStringSPI);
	}
	return 0;
}