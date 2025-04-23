#include "app.h"
int count5=0;
uint8_t tst[10];
uint8_t temp[10];

void SPI1_IRQHandler(void)
{
	if(count5 >= 10)
	{
//		for (int i = 0; i < 10; i++)
//		{
//			tst[i]=temp[10-i];
//		}	
	
		for (int i = 0; i < 10; i++)
		{
			SPI1_SetBayt(tst[i]);
			
		}	
		count5=0;
	}	
	else
	{
		tst[count5]=SPI1_ReadBayt();
		count5++;	
	}

}

int main()
{
	Init_LED();
	Init_SPI();
	
	while(1)
	{
//		tst=0;
//		tst=SPI1_ReadBayt();
//		//tst3=tst;
//		SPI1_SetBayt(tst);
		//tst = SPI_TransmitReceive();	
		//SPI1_SetString(receivedStringSPI);
	}
	return 0;
}