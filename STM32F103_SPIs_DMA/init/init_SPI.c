#include "init_SPI.h"

void Init_SPI()//main init spi
{
		Enable_RCC_SPI();
		Config_GPIO_SPI();
		Config_SPI();
}

void Enable_RCC_SPI()
{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//тактирование GPIOA
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;//тактирование SPI1
}

void Config_GPIO_SPI()
{
		GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);//reset
    GPIOA->CRL |= GPIO_CRL_MODE5_1; // PA5 2MHz
		GPIOA->CRL |= GPIO_CRL_CNF5_1; // PA5 alt SCK
		
		GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);//reset
		GPIOA->CRL |= GPIO_CRL_MODE7_1; // PA7 2MHz
		GPIOA->CRL |= GPIO_CRL_CNF7_1; // PA7 alt MOSI
                  
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
    GPIOA->CRL |= (GPIO_CRL_CNF6_0); // PA6 input MISO
}

void Config_SPI()
{
    SPI1->CR1 = 0;//reset
	
		//SPI1->CR1 |= SPI_CR1_MSTR;// master
		//SPI1->CR1 |= SPI_CR1_BIDIMODE;//включение режима двунаправленных данных mode:master
		SPI1->CR1 |= SPI_CR1_BIDIOE;//включение вывода в двунаправленном режиме
		SPI1->CR1 |= SPI_CR1_CRCEN;//аппаратный расчет CRC включен
		SPI1->CR1 |= SPI_CR1_CRCNEXT;//следующая передача CRC
		SPI1->CR1 |= SPI_CR1_DFF;//16-битный формат кадра данных
		SPI1->CR1 |= SPI_CR1_RXONLY;//Только прием mode:slave
		//SPI1->CR1 |= SPI_CR1_SSM;// Программное управление mode:master
		//SPI1->CR1 |= SPI_CR1_SSI;// Внутренний раб выбор mode:master
		SPI1->CR1 |= SPI_CR1_LSBFIRST;//Формат кадра LSB
		SPI1->CR1 |= SPI_CR1_BR_1;// f/4
		SPI1->CR1 |= SPI_CR1_CPOL;// начальный фронт
		SPI1->CR1 |= SPI_CR1_CPHA;// фаза...
		//SPI1->CR2 |=SPI_CR2_TXDMAEN;//переключили дма на spi - передача, DMAT = Tx
		//SPI1->CR2 |=SPI_CR2_RXDMAEN;//переключили дма на spi - чтение, DMAR = Rx
		
    SPI1->CR1 |= SPI_CR1_SPE;//Вкл SPI
}

uint32_t SPI_Receive()
{
		if(SPI1->SR & SPI_SR_RXNE)
		{
				return SPI1->DR;
		}
    return 0;
}
