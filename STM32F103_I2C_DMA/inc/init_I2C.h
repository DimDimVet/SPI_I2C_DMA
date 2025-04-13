#ifndef INIT_I2C
#define INIT_I2C

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h" 

#define SIZE_BUF 10
#define CLOCK_SPEED  100000
#define TICK_FREQ_1KHZ 1U
#define SLAVE_ADDR  0x68

#define I2C_RISE_TIME(__FREQRANGE__, __SPEED__)            (((__SPEED__) <= 100000U) ? ((__FREQRANGE__) + 1U) : ((((__FREQRANGE__) * 300U) / 1000U) + 1U))

#define I2C_DUTYCYCLE_2 0x00000000U																																	
																																	

void Init_I2C(void);
void Enable_RCC_I2C(void);
void Config_GPIO_I2C(void);
void Config_I2C(void);
//void Config_SPI1_DMA1(void);

uint32_t I2C_SPEED(uint32_t pclk, uint32_t speed, uint32_t dutyCycle);

//uint8_t SPI_TransmitReceive(uint8_t data);
//void SPI1_DMA1_TransmitReceive(char *str_data);
//char* Read_SPI1_DMA1();

#endif
