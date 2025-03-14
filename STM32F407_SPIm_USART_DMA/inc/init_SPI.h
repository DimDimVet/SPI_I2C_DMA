#ifndef INIT_SPI
#define INIT_SPI

#include "stm32f4xx.h"

void Init_SPI(void);
void Enable_RCC_SPI1(void);
void Config_GPIO_SPI1(void);
void Config_SPI(void);
void Config_SPI_DMA1(void);

void SPI_Transmit(uint32_t data);
uint32_t SPI_Receive(void);
uint32_t SPI_TransmitReceive(uint32_t data);
void SPI2_Transmit(uint8_t *data, uint16_t size);
void SPI2_Receive(uint8_t *data, uint16_t size);
#endif

