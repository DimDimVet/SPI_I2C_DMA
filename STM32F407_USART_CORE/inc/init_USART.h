#ifndef INIT_USART
#define INIT_USART

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"                  // Device header

void Init_USART(int baudRate);//main init usart
void Enable_RCC_AHB1(void);//GpioA usart, GpioA pin7 LED
void Enable_RCC_APB1(void);//Usart2
void LED(void);//GpioA pin7 LED
void Config_LED(void);//Config GpioA pin6 pin7 LED
void Config_GPIO_USART(int baudRate);

int USART2_GetStatus();//status Rx
char USART2_ReadChar();//read DR

void USART2_SetChar(char c);
void USART2_SetString(char* str);

#endif
