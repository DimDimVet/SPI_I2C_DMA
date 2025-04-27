#include "app.h"

int flag=0;
int countflag=0;

void ExecutorTerminal_USART_Irq(void)
{
	//I2C1->CR1 |= 0<<I2C_CR1_PE_Pos;
	char rezultRead[SIZE_BUF_USART];
	char rezultStr[SIZE_BUF_USART_MAX];
	char receivedStringConsole[SIZE_BUF_USART];
	char set_infoStr[SIZE_BUF_INFO] = {"Out console in I2C (F407): "};

	USART1_ReadString(receivedStringConsole, SIZE_BUF_USART); // Читаем из консоли
	

//		if (i >= SIZE_BUF_USART - 1)
//		{
			//snprintf(rezultStr, SIZE_BUF_USART_MAX, " %s %s ", set_infoStr, rezultRead);
			//USART1_SetString(rezultStr);
			USART1_SetString(receivedStringConsole);
			flag=1;
//			USART1->CR1 |= 0 << USART_CR1_UE_Pos;
//			delay_ms(100);
//			I2C1->CR1 |= 1<<I2C_CR1_PE_Pos;
//			flag=1;

			  I2C1->CR1 =1 << I2C_CR1_SWRST_Pos;
				I2C1->CR1 =0 << I2C_CR1_SWRST_Pos;
				I2C1->CR1 |= 1<<I2C_CR1_PE_Pos;
//		}
//	}


}

/////////////////

int main()
{
    Init_LED();
		Init_USART1(BAUND_RATE);
		Init_I2C();
		flag=1;
		
    while(1)
    {
		
				if(flag)
				{
						while (I2C_Master_TransmitT(I2C_ADDRESS, dataToSend, 2) != 0)
						{
								Error_Handler();
						}

						while(I2C_Master_ReceiveT(I2C_ADDRESS, receivedData, 2) != 0)
						{
								Error_Handler();
						}
						
//						countflag++;
//						if(countflag >= 100 ){flag=0;}
				}


        delay_ms(100);
    }

    return 0;
}