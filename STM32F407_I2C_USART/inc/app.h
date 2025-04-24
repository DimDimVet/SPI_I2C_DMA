#ifndef APP_H_
#define APP_H_

#include "delay.h"
#include "init_LED.h"
#include "init_I2C.h"
//#include "init_USART.h"


//#define BAUND_RATE 9600
//#define SIZESTR 50
//#define BUFFER_SIZE 32

#define I2C_ADDRESS 0x68  // Адрес I2C устройства
uint8_t dataToSend[2] = {0x68, 0xf7};
uint8_t receivedData[2];


#endif