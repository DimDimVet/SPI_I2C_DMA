#include "app.h"

static int flag=0;
int countflag=0;
	uint8_t rezultRead[SIZE_BUF_USART];
	char rezultStr[SIZE_BUF_USART_MAX];
	uint8_t receivedStringConsole[SIZE_BUF_USART];
	char set_infoStr[SIZE_BUF_INFO] = {"Out console in I2C (F407): "};
	
	
void ExecutorTerminal_USART_Irq(void)
{

	USART1_ReadString(receivedStringConsole, SIZE_BUF_USART); // Читаем из консоли

	
			flag=1;
			__disable_irq();

				if(flag)
				{
						while (I2C_Master_TransmitT(I2C_ADDRESS, receivedStringConsole, 2) != 0)
						{
								Error_Handler();
						}
						delay_ms(5);
						while(I2C_Master_ReceiveT(I2C_ADDRESS, rezultRead, 2) != 0)
						{
								Error_Handler();
						}
				}
				
				
__enable_irq();
USART1_SetString(rezultRead);

}

/////////////////

int main()
{
    Init_LED();
		Init_USART1(BAUND_RATE);
		Init_I2C();
		flag=0;
		
    while(1)
    {
		
    }

    return 0;
}