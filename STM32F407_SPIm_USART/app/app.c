#include "app.h"
	uint8_t count_b=0;

	uint8_t tst1[5];
	uint8_t *tst2=tst1;
	uint8_t receivedStringConsole5_[5];

void USART1_IRQHandler(void)
{
		LED7();
		//
		ExecutorTerminal();
		//
		LED7();
}

void ExecutorTerminal(void)
{

	
	char rezultStrBase[SIZE_BUF_USART_MAX];
  char *rezultStr=rezultStrBase;
	
	char receivedStringConsole_[SIZE_BUF_USART];
	char* receivedStringConsole=receivedStringConsole_;
	

	receivedStringConsole5_[0]=0x01;
	receivedStringConsole5_[1]=0x02;
	receivedStringConsole5_[2]=0x03;
	receivedStringConsole5_[3]=0x04;
	receivedStringConsole5_[4]=0x05;
	
	USART1_ReadString(receivedStringConsole,SIZE_BUF_USART);// Читаем из консоли
	SPI2->CR2 |= (1<<SPI_CR2_RXNEIE_Pos);
	for(int i=0; i < 3; i++)
	{
		delay_ms(10);
			if(SPI2_SetBayt(receivedStringConsole5_[i]))
			{
				
				
				
			}
			
	}
	
	snprintf(rezultStr, SIZE_BUF_USART_MAX, " %s %s - %c ",set_infoStr,receivedStringConsole,(char)tst2[0]);
	
	USART1_SetString(rezultStr);
	
}

void SPI2_IRQHandler(void)
{

	tst2[0]=SPI2_ReadBayt();
//	char receivedStringConsole_[SIZE_BUF_USART];
//	char* receivedStringConsole=receivedStringConsole_;
//	
//	
//	
//	
//	count_b++;
//	if(count_b>=3)
//	{
//		count_b=0;
//		//SPI2->CR2 |= (0<<SPI_CR2_RXNEIE_Pos);
//	}
//	SPI2->CR2 |= (1<<SPI_CR2_RXNEIE_Pos);


}
/////////////////

int main(void)
{
    Init_LED();
		Init_USART1(BAUND_RATE);
    Init_SPI();

    while(1)
    {
//				if(SPI2_SetBayt(0xAA))
//				{
//					 tst=SPI2_ReadBayt();
//				}
//				for(int i=0; i <100000000; i++){};
    }
		
    return 0;
}

