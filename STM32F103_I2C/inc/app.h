#ifndef APP_H_
#define APP_H_

#include "delay.h"
#include "init_LED.h"
#include "init_I2C.h"

uint8_t dataToSend[2] = {0xAA, 0xAF}; // Пример данных для отправки
uint8_t receivedData[2];

#endif
