#ifndef APP_H_
#define APP_H_

#include "delay.h"
#include "init_LED.h"
#include "init_I2C.h"
//#include "init_USART.h"


#define BAUND_RATE 9600
#define SIZESTR 50
#define BUFFER_SIZE 32
#define I2C_7BIT_ADD_WRITE(__ADDRESS__)                    ((uint8_t)((__ADDRESS__) & (uint8_t)(~I2C_OAR1_ADD0)))
#define I2C_7BIT_ADD_READ(__ADDRESS__)                     ((uint8_t)((__ADDRESS__) | I2C_OAR1_ADD0))

int HAL_I2C_Master_ReceiveT(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
//char* set_infoStr="Otvet ot STM32F103 SPI: ";

//void ExecutorTerminal(void);

#endif