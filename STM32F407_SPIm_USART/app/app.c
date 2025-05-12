#include "app.h"

void ExecutorTerminal_USART_Irq(void)
{
char rezultReadSPI_[SIZE_BUF_USART];

USART1_ReadChar(receivedChar); // Читаем из консоли

	if(count_size_buf >= SIZE_BUF_USART)
	{
		count_size_buf=0;
		
		__disable_irq();
		
		for (int i = 0; i < SIZE_BUF_USART; i++)
		{
		
			SPI2_SetBayt(rezultReadConsol[i]);
			rezultReadSPI_[i]= SPI2_ReadBayt();
		
		}

	if(1)
	{
		USART1_SetString(rezultReadSPI_);
		__enable_irq();
	}
	else
	{
		USART1_SetString(rezultReadConsol);
		__enable_irq();
	}
	
	//USART1_SetString(rezultReadConsol);
	
	}
	else
	{

		rezultReadConsol[count_size_buf]=receivedChar_;
		count_size_buf++;
	}

}

void SPI2_IRQHandler(void)
{
	//__disable_irq();
	//rezultReadSPI[0] = SPI2_ReadBayt();
	//rezultReadSPI[count_size_buf] = SPI2_ReadBayt();
		//count_size_buf++;

	 // Читаем из консоли

//	if(count_size_buf >= SIZE_BUF_USART-1)
//	{
//		count_size_buf=0;
//			
//		USART1_SetString(rezultReadSPI);
//		
//	}
//	else
//	{
//		rezultReadSPI[count_size_buf] = SPI2_ReadBayt();
//		count_size_buf++;
//		USART1_SetString(rezultReadSPI);
//	}
//USART1_SetString(rezultReadSPI);

//		for (int i = 0; i < 1; i++)
//		{
//			rezultReadSPI[i] = SPI2_ReadBayt();
//		}
//			USART1_SetString(rezultReadSPI);
}

/////////////////

int main(void)
{
	Init_LED();
	Init_USART1(BAUND_RATE);
	Init_SPI();

	while (1)
	{

	}

	return 0;
}
