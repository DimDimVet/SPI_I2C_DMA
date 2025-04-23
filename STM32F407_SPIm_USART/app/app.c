#include "app.h"

void ExecutorTerminal_USART_Irq(void)
{
char rezultRead[SIZE_BUF_USART];
char rezultStr[SIZE_BUF_USART_MAX];
char receivedStringConsole[SIZE_BUF_USART];
char set_infoStr[SIZE_BUF_INFO]={"Out console in SPI (F407): "};


	USART1_ReadString(receivedStringConsole, SIZE_BUF_USART); // Читаем из консоли
	
	for (int i = 0; i < SIZE_BUF_USART; i++)
	{
		if (SPI2_SetBayt(receivedStringConsole[i]))
		{
		}
		////
		rezultRead[i]=SPI2_ReadBayt();
	
		if(i >= SIZE_BUF_USART-1)
		{
			snprintf(rezultStr, SIZE_BUF_USART_MAX, " %s %s ", set_infoStr, rezultRead);
			USART1_SetString(rezultStr);

		}
	}
}


/////////////////

int main(void)
{
	Init_LED();
	Init_USART1(BAUND_RATE);
	Init_SPI();

	while (1)
	{
		//				if(SPI2_SetBayt(0xAA))
		//				{
		//					 tst=SPI2_ReadBayt();
		//				}
		//				for(int i=0; i <100000000; i++){};
	}

	return 0;
}
