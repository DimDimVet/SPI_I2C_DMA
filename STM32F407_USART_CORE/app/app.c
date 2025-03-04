#include "app.h"


int main()
{
	Init_USART(BAUND_RATE);

	while(1)
	{
		receivedChar='f';
		ExecutorTerminal();
		delay_s(1);
	}
	return 0;
}

void USART2_IRQHandler(void)
{
		LED();
		//
		ExecutorTerminal();
		//
		LED();
}

void ExecutorTerminal()
{
	snprintf(rezultStr, sizeof rezultStr, "%s command: %s", greetingsStr,"sdfsdf");
	USART2_SetString(rezultStr);
//		if (USART2_GetStatus())// Проверка на получение
//		{ 
//        receivedChar = USART2_ReadChar(); // Читаем
//				if(receivedChar==CHAR_COMMAND1)
//				{
//						snprintf(rezultStr, sizeof rezultStr, "%s command: %c", greetingsStr,receivedChar);
//						USART2_SetString(rezultStr);
//				}
//				else if(receivedChar==CHAR_COMMAND0)
//				{
//						snprintf(rezultStr, sizeof rezultStr, "%s command: %c", partingStr,receivedChar);
//						USART2_SetString(rezultStr);
//				}
//				else
//				{
//						snprintf(rezultStr, sizeof rezultStr, "%s: %c", errorStr,receivedChar);
//						USART2_SetString(rezultStr);
//				}
//    }
}
