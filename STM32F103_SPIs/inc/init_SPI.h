#ifndef INIT_SPI_MASTER
#define INIT_SPI_MASTER

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h" 
#include "delay.h"

#define SIZE_BUF_RX_SPI 10

static char dataBufRxSPI[SIZE_BUF_RX_SPI];

void Init_SPI(void);//config spi
void Enable_RCC_SPI1(void);//прерывание
void Config_GPIO_SPI1(void);//порты spi
void Config_SPI1(void);//spi

void SPI1_ReadString(char *data);
void SPI1_SetString(char* str);

uint8_t SPI_TransmitReceive(char *data);

#endif
