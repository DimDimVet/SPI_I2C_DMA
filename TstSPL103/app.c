#include "stm32f10x.h"                  // Device header
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART


void SPIFunc_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	SPI_I2S_DeInit(SPI1);
	SPI_InitTypeDef SPI1_StructInit;
	SPI1_StructInit.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI1_StructInit.SPI_Mode=SPI_Mode_Master;
	SPI1_StructInit.SPI_DataSize=SPI_DataSize_8b;
	SPI1_StructInit.SPI_CPOL=SPI_CPOL_High;
	SPI1_StructInit.SPI_CPHA=SPI_CPHA_2Edge;
	SPI1_StructInit.SPI_NSS=SPI_NSS_Soft;
	SPI1_StructInit.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_64;
	SPI1_StructInit.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI1_StructInit.SPI_CRCPolynomial=7;
	SPI_Init(SPI1,&SPI1_StructInit);
	
}