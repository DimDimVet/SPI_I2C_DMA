#include "app.h"

static char rezultStrBase[SIZESTR];
static char *rezultStr=rezultStrBase;

static char receivedStringConsoleBase[SIZESTR];
static char *receivedStringConsole=receivedStringConsoleBase;

static char receivedStringSPIBase[SIZESTR];
static char *receivedStringSPI=receivedStringSPIBase;


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
	
	USART1_ReadString(receivedStringConsole);// Читаем из консоли
	
	receivedStringSPI=(char*)SPI2_TransmitReceive('A');
	
	// отправляем в консоль
	snprintf(rezultStr, SIZESTR, "%s%s - %s",set_infoStr,receivedStringConsole,receivedStringSPI);
	
	//delay_ms(100);
	USART1_SetString(rezultStr);
}

/////////////////

int main(void)
{
    Init_LED();
    Init_USART1(BAUND_RATE);
    Init_SPI();

    while(1)
    {

    }
		
    return 0;
}

