#ifndef INIT_SPI
#define INIT_SPI

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"

#define SIZE_BUF_RX_SPI 100

void Init_SPI(void);
void Enable_RCC_SPI1(void);
void Config_GPIO_SPI1(void);
void Config_SPI1(void);
void Config_SPI1_DMA1(void);

uint8_t SPI2_TransmitReceive(uint8_t data);
void SPI2_DMA_TransmitReceive(char *str_data);
uint32_t* Read_SPI2_DMA(void);

#endif

