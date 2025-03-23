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

char* set_infoStr="Otvet ot STM32F103 SPI: ";
char* partingStr="BayBay STM32F407";

void ExecutorTerminal(void);

#endif
