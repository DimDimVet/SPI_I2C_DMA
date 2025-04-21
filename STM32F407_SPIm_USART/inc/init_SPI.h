#ifndef INIT_SPI
#define INIT_SPI

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"

#define SIZE_BUF_RX_SPI 100

static uint32_t dataBufRxSPI[SIZE_BUF_RX_SPI];

void Init_SPI(void);
void Enable_RCC_SPI1(void);
void Config_GPIO_SPI1(void);
void Config_SPI1(void);

uint32_t SPI2_TransmitReceive(uint8_t data);

#endif

