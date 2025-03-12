#ifndef APP_H_
#define APP_H_

#include "delay.h"
#include "init_USART_DMA.h"
#include "init_SPI_master.h"

#define BAUND_RATE 9600
#define SIZESTR 30
#define CHAR_COMMAND0 '0'
#define CHAR_COMMAND1 '1'

char rezultStr [SIZESTR];
char receivedChar;
char* greetingsStr="Prived STM32F103";
char* partingStr="BayBay STM32F103";
char* tstSPIStr="Cicl Tx test Data out STM32F103";
char* tstSPIStr2="";
char* errorStr="error command";

void ExecutorTerminal(void);
void Receive_Data(void);

#endif
