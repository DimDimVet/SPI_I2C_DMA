#ifndef APP_H_
#define APP_H_

#include "delay.h"
#include "init_LED.h"
#include "init_I2C.h"
#include "init_USART.h"

#define BAUND_RATE 9600

#define I2C_ADDRESS 0x68  // Адрес I2C устройства

	char rezultRead[SIZE_BUF_USART];
	char receivedStringConsole[SIZE_BUF_USART];
	
	uint8_t rezultRead1[SIZE_BUF_USART];
	uint8_t receivedStringConsole1[SIZE_BUF_USART];

uint8_t dataToSend[5] = {0x68, 0xf7};
uint8_t receivedData[5];


#endif