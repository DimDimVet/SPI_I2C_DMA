#include "app.h"

static char rezultStrBase[SIZESTR];
static char *rezultStr=rezultStrBase;

static uint8_t receivedStringConsoleBase[5];
static uint8_t *receivedStringConsole=receivedStringConsoleBase;

static uint8_t receivedStringSPIBase[5];
static uint8_t *receivedStringSPI=receivedStringSPIBase;

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
	SPI2_SetString(receivedStringConsole,sizeof(receivedStringConsole));
}

void ExecutorTerminal(void)
{
//	receivedStringConsole="asdf";
//	USART1_ReadString(receivedStringConsole);// Читаем из консоли
//	USART1_SetString(receivedStringConsole);
	
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
		receivedStringConsole[0]=0xDD;
		receivedStringConsole[1]=0xD1;
		receivedStringConsole[2]=0xD2;
		receivedStringConsole[3]=0xD3;
		receivedStringConsole[4]=0xD4;
	SPI2_Tx();
		
    while(1)
    {
				
				for(int i=0; i <100000000; i++){};
				//SPI2_SetString(receivedStringConsole,SIZE_BUF_RX_SPI);
				//receivedStringSPI = SPI2_TransmitReceive(receivedStringConsole);//test net DMA
				//USART1_SetString(receivedStringConsole);
				//tst=receivedStringSPI[0];
    }
		
    return 0;
}

