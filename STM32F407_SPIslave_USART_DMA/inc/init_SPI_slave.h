#ifndef INIT_SPI_SLAVE
#define INIT_SPI_SLAVE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx.h"                  // Device header

void Init_SPI(void);//config spi
void Enable_RCC_APB2_SPI(void);//прерывание
void Enable_RCC_AHB1_SPI(void);//прерывание
	
void Config_GPIO_SPI(void);//порты spi
void Config_SPI(void);//spi
void Config_SPI_DMA2(void);//dma spi

int DMA2_SPI_ReadSize(void);
int DMA2_SPI_GetStatus(void);
char DMA2_SPI_ReadData(void);
void DMA2_SPI_SetString(char* str);
void SPI_Transmit(uint8_t data);//spi Tx
uint8_t SPI_Receive();//spi Rx

#endif
