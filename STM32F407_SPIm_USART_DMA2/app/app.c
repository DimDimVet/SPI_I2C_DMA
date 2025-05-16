#include "app.h"
char *tempChar;

void ExecutorTerminal_USART_Irq(void)
{
    USART1_ReadChar(receivedChar); // Читаем из консоли

		//SPI1_DMA1_SetString(receivedChar);

    if(count_size_buf >= SIZE_BUF_USART)
    {
        count_size_buf=0;
				SPI1_DMA1_SetString(rezultReadConsol);
//        //__disable_irq();
//        for (int i = 0; i < SIZE_BUF_USART; i++)
//        {
//           //SPI2_SetBayt(rezultReadConsol[i]);
//						
//					 //rezultReadSPI_[i]= SPI2_ReadBayt();
//					 
//					 rezultReadSPI[i]=SPI1_DMA1_ReadChar();
//					 
//        }
//				USART1_SetString(rezultReadSPI_);		
           // USART1_SetString(rezultReadSPI_);
            //__enable_irq();
    }
    else
    {
        rezultReadConsol[count_size_buf]=receivedChar_;
        count_size_buf++;
    }
}

void Executor_SPI_DMA_RX_Irq(void)
{

					
					rezultReadSPI_[0] = SPI1_DMA1_ReadChar();
					// USART1_SetString(rezultReadSPI_);
					 
    if(count_size_buf >= SIZE_BUF_USART)
    {
        count_size_buf=0;
						
            USART1_SetString(rezultReadSPI_);
            //__enable_irq();
    }
    else
    {
        rezultReadConsol[count_size_buf]=tempChar[0];
        count_size_buf++;
				USART1_SetString("1");
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

    }

    return 0;
}
