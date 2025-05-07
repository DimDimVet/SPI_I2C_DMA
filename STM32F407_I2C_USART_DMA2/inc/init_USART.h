#ifndef INIT_USART
#define INIT_USART

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "init_LED.h"

#define SIZE_BUF_USART 5
#define SIZE_BUF_USART_MAX 100
#define SIZE_BUF_INFO 35



void Init_USART1(uint16_t baudRate);
void Enable_RCC_USART1(void);
void Config_GPIO_USART1(void);
void Config_USART1(uint16_t baudRate);
void Config_USART1_DMA2(void);

void USART1_ReadString(char *str, uint8_t size_buf);
void USART1_SetString(char* str);

void USART1_DMA2_SetString(char	*str, uint8_t size_buf);
void USART1_DMA2_ReadString(char *str, uint8_t size_buf);

void ExecutorTerminal_USART_Irq(void);
void ExecutorTerminal_USART_DMA_Irq(void);
#endif
