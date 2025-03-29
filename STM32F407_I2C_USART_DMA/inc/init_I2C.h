#ifndef INIT_I2C
#define INIT_I2C

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"

#define I2C_ADDRESS 0x3C // Адрес устройства I2C
#define BUFFER_SIZE_I2C 32

void Init_I2C(void);
void Enable_RCC_I2C(void);
void Config_GPIO_I2C(void);
void Config_I2C(void);
void Config_I2C_DMA1(void);

void I2C_Start(void);
void I2C_Stop(void);
void I2C1_SendAddress(uint8_t addr);
void I2C1_SendData(uint8_t data);
uint8_t I2C1_ReceiveData(void);

void I2C_Write(uint8_t address, uint8_t *data, uint8_t size);
void I2C_Read(uint8_t address, uint8_t *data, uint8_t size);
uint8_t* I2C_Scan_Bus(uint8_t count);

void I2C_DMA_TransmitReceive(char *str_data);
uint32_t* Read_I2C_DMA(void);

#endif

