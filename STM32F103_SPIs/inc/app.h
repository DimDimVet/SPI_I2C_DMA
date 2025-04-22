#ifndef APP_H_
#define APP_H_

#include "delay.h"
#include "init_LED.h"
#include "init_SPI.h"

#define SIZE_REVERC_DATA 5
#define SIZESTR 100

static char* set_infoStr="Out STM32F103 SPI: ";

static char receivedStringSPIBase[SIZE_REVERC_DATA];
static char *receivedStringSPI=receivedStringSPIBase;

static char rezultStrBase[SIZESTR];
static char *rezultStr=rezultStrBase;

#endif
