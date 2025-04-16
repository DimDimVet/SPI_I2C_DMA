#ifndef INIT_I2C
#define INIT_I2C

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"

#define BUFFER_SIZE_I2C 32
#define I2C_ADDRESSINGMODE_7BIT         0x00004000U
#define CLOCK_SPEED 100000
																																	
#define I2C_RISE_TIME(__FREQRANGE__, __SPEED__)            (((__SPEED__) <= 100000U) ? ((__FREQRANGE__) + 1U) : ((((__FREQRANGE__) * 300U) / 1000U) + 1U))												

void Init_I2C(void);
void Enable_RCC_I2C(void);
void Config_GPIO_I2C(void);
void Config_I2C(void);
void Config_I2C_DMA1(void);

uint32_t I2C_SPEED(uint32_t pclk, uint32_t speed, uint32_t dutyCycle);


void I2C_DMA_TransmitReceive(char *str_data);
uint32_t* Read_I2C_DMA(void);

#endif

