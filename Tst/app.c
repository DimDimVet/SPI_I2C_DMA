#include "stm32f10x.h"                  // Device header

void SPI_Transmit_DMA(uint8_t *data, uint16_t size) {
    DMA1_Channel3->CNDTR = size; // Установите количество передаваемых данных
    DMA1_Channel3->CMAR = (uint32_t)data; // Установите адрес данных
    SPI1->CR2 |= SPI_CR2_TXDMAEN; // Включение DMA для передачи
    DMA1_Channel3->CCR |= DMA_CCR3_EN; // Включение DMA-канала

    while (!(SPI1->SR & SPI_SR_TXE)); // Ожидание завершения передачи
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN; // Выключение DMA
}

void Test_SPI1_DMA() {
    uint8_t transmit_data[1] = {0x55}; // Данные для передачи
    SPI_Transmit_DMA(transmit_data, 1); // Передача
}
	 
  int main() 
		{
		char data;
	 RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
RCC->AHBENR |= RCC_AHBENR_DMA1EN;
			
	 GPIOA->CRL &= ~((0xF << (4 * 5)) | (0xF << (4 * 6)) | (0xF << (4 * 7)));
GPIOA->CRL |= (0xB << (4 * 5)); // PA5 - SCK
GPIOA->CRL |= (0x4 << (4 * 6)); // PA6 - MISO
GPIOA->CRL |= (0xB << (4 * 7)); // PA7 - MOSI
			
	 SPI1->CR1 = 0; // Сброс настроек
SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SPE | SPI_CR1_SSI | SPI_CR1_SSM; // Master, включить SPI
       
			DMA1_Channel3->CCR &= ~DMA_CCR3_EN; // Отключите канал
DMA1_Channel3->CNDTR = 1; // Количество данных
DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR; // Адрес регистра данных SPI
DMA1_Channel3->CMAR = (uint32_t)&data; // Адрес памяти
DMA1_Channel3->CCR |= DMA_CCR3_MINC | DMA_CCR3_DIR; // Инкремент адреса памяти, передача от памяти к периферии
			
       while (1) {
        Test_SPI1_DMA(); // Выполнение теста SPI с DMA
        // Задержка или другая логика
    }
   }