#ifndef INIT_I2C
#define INIT_I2C

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "init_LED.h"

#define BUFFER_SIZE_I2C 32
#define I2C_ADDRESSINGMODE_7BIT         0x00004000U
#define CLOCK_SPEED 100000

#define I2C_7BIT_ADD_WRITE(__ADDRESS__)                    ((uint8_t)((__ADDRESS__) & (uint8_t)(~I2C_OAR1_ADD0)))
#define I2C_7BIT_ADD_READ(__ADDRESS__)                     ((uint8_t)((__ADDRESS__) | I2C_OAR1_ADD0))																																	
#define I2C_RISE_TIME(__FREQRANGE__, __SPEED__)            (((__SPEED__) <= 100000U) ? ((__FREQRANGE__) + 1U) : ((((__FREQRANGE__) * 300U) / 1000U) + 1U))												

void Init_I2C(void);
void Enable_RCC_I2C(void);
void Config_GPIO_I2C(void);
void Config_I2C(void);

uint32_t I2C_SPEED(uint32_t pclk, uint32_t speed, uint32_t dutyCycle);
uint8_t I2C_AdresSetTime(void);
uint8_t I2C_StartBit_SetTime(void);
uint8_t I2C_BTFBit_SetTime(void);
uint8_t I2C_TX_SetTime(void);
uint8_t I2C_MasterRequestRead(uint16_t DevAddress);
uint8_t I2C_MasterRequestWriteT(uint16_t DevAddress);
uint8_t I2C_RX_SetTime(void);

uint8_t I2C_Master_ReceiveT(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
uint8_t I2C_Master_TransmitT(uint16_t DevAddress, uint8_t *pData, uint16_t Size);

void Error_Handler(void);
#endif

