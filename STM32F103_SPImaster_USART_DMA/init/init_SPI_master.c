#include "init_SPI_master.h"

void Init_SPI(void)//main init spi
{
	Enable_RCC_APB2_SPI();
	Config_GPIO_SPI();
	Config_SPI();
}

void Enable_RCC_APB2_SPI()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//тактирование GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;//тактирование SPI1
}

void  Config_GPIO_SPI()
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

void  Config_SPI()
{
    SPI1->CR1 = 0;//reset

    SPI1->CR1 |= SPI_CR1_MSTR;// master
		SPI1->CR1 |= SPI_CR1_SSM;// Программное управление
		SPI1->CR1 |= SPI_CR1_SSI;// Внутренний раб выбор
		SPI1->CR1 |= SPI_CR1_BR_1;// f/4
		SPI1->CR1 |= SPI_CR1_CPOL;// начальный фронт
		SPI1->CR1 |= SPI_CR1_CPHA;// фаза...

    SPI1->CR1 |= SPI_CR1_SPE;//Вкл SPI
}

void SPI_Transmit(uint8_t data)
{
    while (!(SPI1->SR & SPI_SR_TXE))//готовность Tx
		{
		};
    SPI1->DR = data;
}

uint8_t SPI_Receive()
{
    while (!(SPI1->SR & SPI_SR_RXNE))//готовность Rx
		{
		};
    return SPI1->DR;
}