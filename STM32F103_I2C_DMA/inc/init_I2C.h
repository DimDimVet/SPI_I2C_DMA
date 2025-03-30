#ifndef INIT_I2C
#define INIT_I2C

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h" 

#define SIZE_BUF 10

void Init_I2C(void);
void Enable_RCC_I2C(void);
void Config_GPIO_I2C(void);
void Config_I2C(void);
//void Config_SPI1_DMA1(void);

//uint8_t SPI_TransmitReceive(uint8_t data);
//void SPI1_DMA1_TransmitReceive(char *str_data);
//char* Read_SPI1_DMA1();

#endif
