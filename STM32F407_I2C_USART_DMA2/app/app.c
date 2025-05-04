#include "app.h"





void ExecutorTerminal_USART_Irq(void)
{

	USART1_ReadString(receivedStringConsole, SIZE_BUF_USART); // Читаем из консоли

	__disable_irq();

	while (I2C_Master_TransmitDMA(I2C_ADDRESS, receivedStringConsole, SIZE_BUF_USART) != 0)
	{
		Error_Handler();
	}

	while (I2C_Master_ReceiveT(I2C_ADDRESS, rezultRead, SIZE_BUF_USART) != 0)
	{
		Error_Handler();
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

	while (1)
	{
	}

	return 0;
}