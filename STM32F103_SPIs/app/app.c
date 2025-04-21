#include "app.h"

uint8_t tst;
char data_In_Base[SIZE_REVERC_DATA];
char* data_In=data_In_Base;

#define BUFFER_SIZE 32
volatile uint8_t txBuffer[BUFFER_SIZE];
volatile uint8_t rxBuffer[BUFFER_SIZE];
volatile uint32_t index = 0;



void SPI1_IRQHandler(void) {
    if (SPI1->SR & SPI_SR_RXNE) { // Проверяем, установлен ли флаг RXNE
        rxBuffer[index++] = SPI1->DR; // Считываем данные в буфер
        if (index >= BUFFER_SIZE) {
            index = 0; // Сброс индекса при переполнении
        }
    }
    
		//
		for(int i=0; i < BUFFER_SIZE;i++)
		{
			txBuffer[i]=rxBuffer[i];
		}
		
		
		
    if (SPI1->SR & SPI_SR_TXE) { // Проверяем, установлен ли флаг TXE
        if (index < BUFFER_SIZE) {
            SPI1->DR = txBuffer[index++]; // Отправляем данные
        } else {
            SPI1->CR2 &= ~SPI_CR2_TXEIE; // Отключаем прерывание TXE
        }
    }
}


int main()
{
	Init_LED();
	Init_SPI();

	while(1)
	{
		//tst = SPI_TransmitReceive(0);	
	}
	return 0;
}