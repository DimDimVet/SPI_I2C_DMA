#ifndef APP_H_
#define APP_H_

#include "delay.h"
#include "init_LED.h"
#include "init_SPI.h"
#include "init_USART.h"

#define BAUND_RATE 9600
#define SIZESTR 30


char rezultStr [SIZESTR];
char receivedChar;

char* greetingsStr="Prived STM32F407";
char* partingStr="BayBay STM32F407";

void ExecutorTerminal(void);

#endif
