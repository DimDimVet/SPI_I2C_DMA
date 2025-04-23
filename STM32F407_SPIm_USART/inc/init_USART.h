#ifndef INIT_USART
#define INIT_USART

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"  
#include "delay.h"

#define SIZE_BUF_USART 10
#define SIZE_BUF_USART_MAX 100

static char* set_infoStr="Out console in SPI (F407): ";

void Init_USART1(uint16_t baudRate);
void Enable_RCC_USART1(void);
void Config_GPIO_USART1(void);
void Config_USART1(uint16_t baudRate);

void USART1_ReadString(char *data, uint8_t size_buf);
void USART1_SetString(char* str);
#endif
