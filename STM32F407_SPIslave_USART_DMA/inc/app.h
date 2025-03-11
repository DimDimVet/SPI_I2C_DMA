#ifndef APP_H_
#define APP_H_

#include "delay.h"
#include "init_USART.h"
#include "init_SPI_slave.h"

#define BAUND_RATE 9600
#define SIZESTR 50
#define CHAR_COMMAND0 '0'
#define CHAR_COMMAND1 '1'
#define CHAR_COMMAND5 '5'

char rezultStr [SIZESTR];
char receivedChar;
char* tstStr="";
char* greetingsStr="Prived STM32F407";
char* partingStr="BayBay STM32F407";
char* errorStr="error command";

void ExecutorTerminal(void);
void Receive_Data(void);

#endif
