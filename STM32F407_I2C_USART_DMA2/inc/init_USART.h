#ifndef INIT_USART
#define INIT_USART

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "init_LED.h"

#define SIZE_BUF_USART 10
#define SIZE_BUF_USART_MAX 100
#define SIZE_BUF_INFO 35



void Init_USART1(uint16_t baudRate);
void Enable_RCC_USART1(void);
void Config_GPIO_USART1(void);
void Config_USART1(uint16_t baudRate);
void Config_USART1_DMA2(void);

void USART1_ReadString(uint8_t *data, uint8_t size_buf);
void USART1_SetString(uint8_t *str);

void USART1_DMA2_SetString(uint8_t* str,uint8_t size_buf);
void USART1_DMA2_ReadChar(uint8_t* readChar);

void ExecutorTerminal_USART_Irq(void);
#endif
