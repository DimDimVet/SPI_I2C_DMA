#ifndef INIT_SPI
#define INIT_SPI

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"                  // Device header

void Init_SPI(void);
void Enable_RCC_AHB(void);
void Enable_RCC_APB2(void);
	
void LED(void);
void Config_LED(void);

void Config_GPIO_SPI(void);
void Config_SPI1(void);
void Config_DMA1(void);

int DMA1_GetStatus();
char DMA1_ReadChar();

uint8_t SPI_TransmitReceive(uint8_t data);
void DMA1_SetString(char* str);

#endif
