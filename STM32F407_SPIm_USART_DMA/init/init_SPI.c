#include "init_SPI.h"

void Init_SPI()//main init spi	
{
	Enable_RCC_SPI1();
	Config_GPIO_SPI1();
	Config_SPI();
}

void Enable_RCC_SPI1()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Включаем тактирование порта B
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // Включаем тактирование SPI2
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // тактирование DMA2
}

void Config_GPIO_SPI1()
{
		//PB13(SCK), PB14(MISO), PB15(MOSI)
    
		GPIOB->MODER &= ~GPIO_MODER_MODE13; // Очистка режима для PA5
    GPIOB->MODER |= GPIO_MODER_MODE13_1;   // Альтернативная функция для PA5

    GPIOB->MODER &= ~GPIO_MODER_MODE14;  // Очистка режима для PA6
    GPIOB->MODER |= GPIO_MODER_MODE14_1;   // Альтернативная функция для PA6

    GPIOB->MODER &= ~GPIO_MODER_MODE15;  // Очистка режима для PA7
    GPIOB->MODER |= GPIO_MODER_MODE15_1;   // Альтернативная функция для PA7

    // Настраиваем альтернативные функции (AF5 для SPI2)
		GPIOB->AFR[1] |= GPIO_AFRH_AFSEL13_0|GPIO_AFRH_AFSEL13_2;// AF5 для SPI1 PA5 (SCK)
		GPIOB->AFR[1] |= GPIO_AFRH_AFSEL14_0|GPIO_AFRH_AFSEL14_2;// AF5 для SPI1 PA6 (MISO)
		GPIOB->AFR[1] |= GPIO_AFRH_AFSEL15_0|GPIO_AFRH_AFSEL15_2;// AF5 для SPI1 PA7 (MOSI)
}

void Config_SPI()//slave mode
{
    SPI2->CR1 = 0;//reset

		SPI2->CR1 |= SPI_CR1_MSTR;// master
		SPI2->CR1 |= SPI_CR1_BIDIMODE;//включение режима двунаправленных данных mode:master
		SPI2->CR1 |= SPI_CR1_BIDIOE;//включение вывода в двунаправленном режиме
		SPI2->CR1 |= SPI_CR1_CRCEN;//аппаратный расчет CRC включен
		SPI2->CR1 |= SPI_CR1_CRCNEXT;//следующая передача CRC
		//SPI2->CR1 |= SPI_CR1_DFF;//16-битный формат кадра данных
		//SPI2->CR1 |= SPI_CR1_RXONLY;//Только прием mode:slave
		SPI2->CR1 |= SPI_CR1_SSM;// Программное управление mode:master
		SPI2->CR1 |= SPI_CR1_SSI;// Внутренний раб выбор mode:master
		SPI2->CR1 |= SPI_CR1_LSBFIRST;//Формат кадра LSB
		SPI2->CR1 |= SPI_CR1_BR_1;// f/4
		SPI2->CR1 |= SPI_CR1_CPOL;// начальный фронт
		SPI2->CR1 |= SPI_CR1_CPHA;// фаза...
		SPI2->CR2 = SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN; // Включаем DMA
	
    SPI2->CR1 |= SPI_CR1_SPE;//Вкл SPI
}

void SPI_Transmit(uint8_t data)
{
    while (!(SPI2->SR & SPI_SR_TXE))//готовность Tx
		{
		};
    SPI2->DR = data;
}

uint8_t SPI_Receive()
{
//    while (!(SPI2->SR & SPI_SR_RXNE))//готовность Rx
//		{
//		};
//    return SPI2->DR;
	uint8_t data;
		if (SPI2->SR & SPI_SR_RXNE) 
			{ // Если есть данные на приеме
           data = SPI2->DR; 
           
      };
			return data;
}
