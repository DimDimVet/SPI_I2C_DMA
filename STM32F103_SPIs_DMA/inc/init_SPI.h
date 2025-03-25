#ifndef INIT_SPI_MASTER
#define INIT_SPI_MASTER

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h" 

void Init_SPI(void);//config spi
void Enable_RCC_SPI1(void);//прерывание
void Config_GPIO_SPI1(void);//порты spi
void Config_SPI1(void);//spi
void Config_SPI1_DMA1(void);

uint8_t SPI_TransmitReceive(uint8_t data);
void SPI1_DMA1_TransmitReceive(char *str_data);
//uint32_t Read_SPI1_DMA1();
char* Read_SPI1_DMA1(char *str_data);

#endif
