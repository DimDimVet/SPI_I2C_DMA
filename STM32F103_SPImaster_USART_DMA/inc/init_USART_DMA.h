#ifndef INIT_USART_DMA
#define INIT_USART_DMA

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"                  // Device header

//char* startStr="STM32F103";
//int sizeTx, sizeRx=1;
//char* dataBufTx;
//char* dataBufRx;

void Init_USART(int baudRate);//main init usart
void Enable_RCC_AHB(void);//DMA1
void Enable_RCC_APB2(void);//GpioC pin13 LED, GpioA usart1
	
void LED(void);//GpioC pin13 LED
void Config_LED(void);//Config GpioC pin13 LED

void Config_GPIO_USART(int baudRate);
void Config_DMA1(void);

int DMA1_GetStatus();//status Rx
char DMA1_ReadChar();//read DR

void DMA1_SetString(char* str);

#endif
