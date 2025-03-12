#ifndef INIT_SPI_MASTER
#define INIT_SPI_MASTER

#include "delay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"                  // Device header

void Init_SPI(void);//config spi
void Enable_RCC_APB2_SPI(void);//прерывание
void Enable_AHBENR_SPI(void);//прерывание
	
void Config_GPIO_SPI(void);//порты spi
void Config_SPI(void);//spi
void Config_SPI_DMA1(void);//spi dma

void DMA1_SPI_GetString(char* str, int sizeSPITx);
void DMA1_SPI_SetString(char* str, int sizeSPITx);
void SPI_Transmit(uint8_t data);//spi Tx
uint8_t SPI_Receive();//spi Rx

#endif
