#ifndef INIT_SPI
#define INIT_SPI

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "delay.h"

// #define SIZE_BUF_RX_SPI 5
// #define LIMIT_ERROR_COUNT 1000

// static uint32_t dataBufRxSPI[SIZE_BUF_RX_SPI];

// static uint8_t dataBufTxSPIBase[SIZE_BUF_RX_SPI];
// static uint8_t *dataBufTxSPI=dataBufTxSPIBase;

// static uint16_t error_count;

void Init_SPI(void);
void Enable_RCC_SPI1(void);
void Config_GPIO_SPI1(void);
void Config_SPI1(void);

// void SPI2_Tx();
uint8_t SPI2_TransmitReceive(uint8_t data);
uint8_t SPI2_ReadBayt(void);
uint8_t SPI2_SetBayt(uint8_t byte);

#endif
