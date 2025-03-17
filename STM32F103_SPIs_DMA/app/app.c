#include "app.h"
char tst;
uint32_t readData; 
int main()
{
	Init_LED();
	Init_SPI();
	int i;
	char tst2;
	i++;
	while(1)
	{
		
		i++;
		//delay_s(1);
		//tst = SPI_TransmitReceive(i);	
		//tst2=tst;
		while (!(SPI1->SR & SPI_SR_RXNE))
			{

			}		// Ждём, пока TXE станет 1
			tst = SPI1->DR;
			LED13();
			
		while (!(SPI1->SR & SPI_SR_TXE))
			{
				
			}		// Ждём, пока TXE станет 
			SPI1->DR=i;
			
	}
	return 0;
}