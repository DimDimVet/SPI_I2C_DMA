#ifndef INIT_SPI_MASTER
#define INIT_SPI_MASTER

#include "stm32f10x.h" 

void Init_SPI(void);//config spi
void Enable_RCC_SPI(void);//прерывание
	
void Config_GPIO_SPI(void);//порты spi
void Config_SPI(void);//spi
uint32_t SPI_TransmitReceive(uint32_t data);


void Config_SPI_DMA1(void);//spi dma

#endif
