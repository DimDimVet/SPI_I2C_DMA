#include "app.h"

static char rezultStrBase[SIZESTR];
static char *rezultStr=rezultStrBase;

static char receivedStringConsoleBase[10];
static char *receivedStringConsole=receivedStringConsoleBase;

static char receivedStringSPIBase[1];
static char *receivedStringSPI=receivedStringSPIBase;

uint32_t* str_In;
uint8_t tst;


void USART1_IRQHandler(void)
{
		LED7();
		//
		ExecutorTerminal();
		//
		LED7();
}

void SPI2_IRQHandler(void)
{
	//delay_ms(100);
//	SPI2_ReadString(receivedStringSPI);
//	// отправляем в консоль
//	snprintf(rezultStr, SIZESTR, "%s%s - %s",set_infoStr,receivedStringConsole,receivedStringSPI);
//	
//	//delay_ms(100);
//	USART1_SetString(rezultStr);
	
//	SPI2->CR2 = 0 << SPI_CR2_RXNEIE_Pos;
//	SPI2->CR1 |= 1 << SPI_CR1_SPE_Pos;//Вкл SPI
}

void ExecutorTerminal(void)
{
	receivedStringConsole="asdf";
	USART1_ReadString(receivedStringConsole);// Читаем из консоли
	USART1_SetString(receivedStringConsole);
	
//	SPI2->CR1 |= 0 << SPI_CR1_SPE_Pos;//Вкл SPI
//	SPI2->CR2 = 1 << SPI_CR2_RXNEIE_Pos;
//	SPI2->CR1 |= 1 << SPI_CR1_SPE_Pos;//Вкл SPI
	
	//SPI2_SetString(receivedStringConsole);
	
}

/////////////////

int main(void)
{
    Init_LED();
		Init_USART1(BAUND_RATE);
    Init_SPI();
		receivedStringConsole="asdf";
    while(1)
    {
				tst = SPI2_TransmitReceive(receivedStringConsole);//test net DMA
				//USART1_SetString(receivedStringConsole);

    }
		
    return 0;
}

