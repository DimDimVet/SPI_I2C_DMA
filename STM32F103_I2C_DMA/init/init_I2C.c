#include "init_I2C.h"

char dataBufRxSPI[SIZE_BUF];

void Init_I2C()
{
		Enable_RCC_I2C();
		Config_GPIO_I2C();
		Config_I2C();
		//Config_SPI1_DMA1();
}

void Enable_RCC_I2C(void)
{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//тактирование GPIOA
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;//тактирование
		//RCC->AHBENR |= RCC_AHBENR_DMA1EN;   // Включаем DMA1
}

void Config_GPIO_I2C()
{

 // Настраиваем PB6 (SCL), PB7 (SDA) как альтернативные функции

		GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);//reset
    GPIOB->CRL |= GPIO_CRL_MODE6_0; // PB6
		GPIOB->CRL |= GPIO_CRL_CNF6; // PB6 alt
		
		GPIOB->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);//reset
		GPIOB->CRL |= GPIO_CRL_MODE7_0; // PB7
		GPIOB->CRL |= GPIO_CRL_CNF7; // PB7 alt



//SET_BIT(GPIOB->CRL, GPIO_CRL_CNF7_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_0 | GPIO_CRL_CNF6_0 |\
//                  GPIO_CRL_MODE7_1 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE7_0 | GPIO_CRL_MODE6_0);

		    // Настраиваем PB6 (SCL) и PB7 (SDA) на альтернативные функции
//    GPIOB->CRL &= ~((GPIO_CRL_MODE6 | GPIO_CRL_CNF6) | (GPIO_CRL_MODE7 | GPIO_CRL_CNF7));
//    GPIOB->CRL |= (GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0); // Режим открытого стока
		
//		// Настраиваем PA4 (nSS) как вход, особеность slave SPI
//    GPIOA->CRL &= ~GPIO_CRL_CNF4; // сбрасываем настройки
//    GPIOA->CRL |= GPIO_CRL_MODE4_0; // режим входа с подтяжкой
}

void Config_I2C()
{
	
    // Включаем тактирование I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Настройка I2C (100kHz)
    I2C1->CR1 |= I2C_CR1_SWRST;  // Сброс I2C
    I2C1->CR1 &= ~I2C_CR1_SWRST; // Снятие сброса

    I2C1->CCR = 0x28; // Устанавливаем делитель для 100kHz
    I2C1->TRISE = 9;  // Максимальное время подъема
    I2C1->OAR1 = (0x50); // Установка адреса устройства

    // Включаем I2C
    I2C1->CR1 |= I2C_CR1_PE;
}

void Config_SPI1_DMA1()
{
//Channel 2 SPI1_RX, Channel 3 SPI1_TX

		DMA1_Channel3->CCR |=0;
		DMA1_Channel3->CCR &= ~DMA_CCR3_MEM2MEM;//режим памяти в память
		DMA1_Channel3->CCR &= ~DMA_CCR3_PL;//уровень приоритета канала
		DMA1_Channel3->CCR &= ~DMA_CCR3_MSIZE_0;//размер памяти
		DMA1_Channel3->CCR &= ~DMA_CCR3_MSIZE_1;
		DMA1_Channel3->CCR &= ~DMA_CCR3_PSIZE_0;//размер периферии
		DMA1_Channel3->CCR &= ~DMA_CCR3_PSIZE_1;
		DMA1_Channel3->CCR |= DMA_CCR3_MINC;//Режим приращения памяти
		DMA1_Channel3->CCR &= ~DMA_CCR3_PINC;//Режим периферийного приращения
		DMA1_Channel3->CCR &= ~DMA_CCR3_CIRC;//Кольцевой режим
		DMA1_Channel3->CCR |= DMA_CCR3_DIR;//Направление передачи данных
		DMA1_Channel3->CCR |= DMA_CCR3_TCIE;//разрешение прерывания по завершению передачи
		DMA1_Channel3->CPAR = (uint32_t)(&SPI1->DR); //Адрес регистра данных spi
		DMA1_Channel3->CNDTR = 0; //размер массива
		DMA1_Channel3->CMAR = 0; //Адрес буфера
		DMA1_Channel3->CCR |= DMA_CCR3_EN; // Включение канала DMA

		DMA1_Channel2->CCR |=0;
		DMA1_Channel2->CCR &= ~DMA_CCR2_MEM2MEM;//режим памяти в память
		DMA1_Channel2->CCR &= ~DMA_CCR2_PL;//уровень приоритета канала
		DMA1_Channel2->CCR |= DMA_CCR2_MSIZE_0;//размер памяти
		DMA1_Channel2->CCR |= DMA_CCR2_MSIZE_1;
		DMA1_Channel2->CCR |= DMA_CCR2_PSIZE_0;//размер периферии
		DMA1_Channel2->CCR |= DMA_CCR2_PSIZE_1;
		DMA1_Channel2->CCR |= DMA_CCR2_MINC;//Режим приращения памяти
		DMA1_Channel2->CCR |= DMA_CCR2_PINC;//Режим периферийного приращения
		DMA1_Channel2->CCR |= DMA_CCR2_CIRC;//Кольцевой режим
		DMA1_Channel2->CCR &= ~DMA_CCR2_DIR;//Направление передачи данных
		DMA1_Channel2->CCR |= DMA_CCR2_TCIE;//разрешение прерывания по завершению передачи
		DMA1_Channel2->CPAR = (uint32_t)(&SPI1->DR); //Адрес регистра данных spi
		DMA1_Channel2->CNDTR = SIZE_BUF; //размер массива
		DMA1_Channel2->CMAR = (uint32_t)dataBufRxSPI; //Адрес буфера
		DMA1_Channel2->CCR |= DMA_CCR2_EN; // Включение канала DMA
		
		DMA1->IFCR |= DMA_IFCR_CTCIF3;
		DMA1->IFCR |= DMA_IFCR_CTCIF2;
		
		NVIC_EnableIRQ(DMA1_Channel3_IRQn); // Включение прерываний DMA
		NVIC_EnableIRQ(DMA1_Channel2_IRQn); // Включение прерываний DMA

}

//////////////
uint8_t SPI_TransmitReceive(uint8_t data)
{
	uint8_t data1;

		if(SPI1->SR & SPI_SR_RXNE)
		{
					data1=SPI1->DR;
			    while (!(SPI1->SR & SPI_SR_TXE)); // Ждём, пока TXE станет 1
					SPI1->DR = data1;  
			return SPI1->DR;
		}
		else
		{
			return SPI1->DR;
		}
}

char* Read_SPI1_DMA1()
{
    return dataBufRxSPI;
}


void SPI1_DMA1_TransmitReceive(char *str_data)
{
    DMA1_Channel3->CCR &= ~DMA_CCR3_EN;
    DMA1_Channel3->CNDTR = SIZE_BUF;		
    DMA1_Channel3->CMAR = (uint32_t)str_data; // Указание адреса буфера передачи
    DMA1_Channel3->CCR |= DMA_CCR3_EN;     // Включаем DMA
}