#ifndef INIT_USART
#define INIT_USART

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"                  // Device header

void Init_USART(int baudRate);//main init usart
void Enable_RCC(void);//GpioA usart, GpioA pin7 LED, USART1, DMA2
void LED(void);//GpioA pin7 LED
void Config_LED(void);//Config GpioA pin6 pin7 LED
void Config_GPIO_USART(int baudRate);
void Config_DMA1(void);

void read_dma(uint8_t *df, uint16_t size);

int DMA2_GetStatus(void);//status Rx
char DMA2_ReadChar(void);//read DR

void DMA2_SetChar(char c);
void DMA2_SetString(char* str);

#endif
