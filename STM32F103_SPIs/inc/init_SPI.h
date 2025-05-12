#ifndef INIT_SPI_MASTER
#define INIT_SPI_MASTER

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "delay.h"

#define SIZE_BUF_SPI 10

void Init_SPI(void);         // config spi
void Enable_RCC_SPI1(void);  // прерывание
void Config_GPIO_SPI1(void); // порты spi
void Config_SPI1(void);      // spi

uint8_t SPI1_ReadBayt(void);
void SPI1_SetBayt(uint8_t byte);
void ExecutorData(uint8_t *data);

uint8_t SPI_TransmitReceive(void);//test

#endif
