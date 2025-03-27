#ifndef INIT_USART
#define INIT_USART

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"  

#define SIZE_BUF_RX_USART 1

void Init_USART1(int baudRate);
void Enable_RCC_USART1(void);
void Config_GPIO_USART1(void);
void Config_USART1(int baudRate);
void Config_USART1_DMA2(void);

char* DMA2_ReadChar(void);//read DR
void DMA2_SetString(char* str);

#endif
