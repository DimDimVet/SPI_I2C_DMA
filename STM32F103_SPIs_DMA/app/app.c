#include "app.h"
uint32_t tst;
uint32_t tst2;
uint32_t readData; 
int main()
{
	Init_LED();
	Init_SPI();
	int i;
	
	i++;
	while(1)
	{
		tst2=tst;
		//i++;
		//delay_s(1);
		tst = SPI_TransmitReceive(i);	
		
//		while (!(SPI1->SR & SPI_SR_RXNE))
//			{

//			}		// Ждём, пока TXE станет 1
//			tst = SPI1->DR;
//			LED13();
//			
//		while (!(SPI1->SR & SPI_SR_TXE))
//			{
//				
//			}		// Ждём, пока TXE станет 
		//	SPI1->DR=i;
			
	}
	return 0;
}