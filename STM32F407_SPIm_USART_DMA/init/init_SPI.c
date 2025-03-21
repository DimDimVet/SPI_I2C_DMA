#include "init_SPI.h"

#define SIZESTR 1
uint8_t sizeSPI1=SIZESTR;
uint32_t dataBufSPI1=0xAA;
uint32_t dataBufSPIRx1;

	int sizeTx=10, sizeRx=1;
char* dataBufTx="tests";

void Init_SPI()//main init spi	
{
	Enable_RCC_SPI1();
	Config_GPIO_SPI1();
	Config_SPI();
	Config_SPI_DMA1();
}

void Enable_RCC_SPI1()
{
	//RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; // Включаем тактирование DMA1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Включаем тактирование порта B
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // Включаем тактирование SPI2
	//RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // тактирование DMA2
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; // Включаем тактирование DMA1

}

void Config_GPIO_SPI1()
{
		//PB13(SCK), PB14(MISO), PB15(MOSI)
		
		GPIOB->MODER &= ~GPIO_MODER_MODE13; // Очистка режима для PB13
    GPIOB->MODER |= GPIO_MODER_MODE13_1;   // Альтернативная функция для PB13(SCK)

    GPIOB->MODER &= ~GPIO_MODER_MODE14;  // Очистка режима для PB14
    GPIOB->MODER |= GPIO_MODER_MODE14_1;   // Альтернативная функция для PB14(MISO)

    GPIOB->MODER &= ~GPIO_MODER_MODE15;  // Очистка режима для PB15
    GPIOB->MODER |= GPIO_MODER_MODE15_1;   // Альтернативная функция для PB15(MOSI)

		GPIOB->AFR[1] |= 5 << GPIO_AFRH_AFSEL13_Pos;// AF5 для SPI1 PB13(SCK)
		GPIOB->AFR[1] |= 5 << GPIO_AFRH_AFSEL14_Pos;// AF5 для SPI1 PB14(MISO)
		GPIOB->AFR[1] |= 5 << GPIO_AFRH_AFSEL15_Pos;// AF5 для SPI1 PB15(MOSI)
}

void Config_SPI()//master mode
{
    SPI2->CR1 = 0;//reset

		SPI2->CR1 |= 1 << SPI_CR1_MSTR_Pos;// master
		SPI2->CR1 |= 0 << SPI_CR1_BIDIMODE_Pos;//включение режима двунаправленных данных mode:master
		SPI2->CR1 |= 0 << SPI_CR1_BIDIOE_Pos;//включение вывода в двунаправленном режиме
		SPI2->CR1 |= 0 << SPI_CR1_CRCEN_Pos;//аппаратный расчет CRC включен 0
		SPI2->CR1 |= 0 << SPI_CR1_CRCNEXT_Pos;//следующая передача CRC 0
		SPI2->CR1 |= 1 << SPI_CR1_DFF_Pos;//16-битный формат кадра данных0
		SPI2->CR1 |= 0 << SPI_CR1_RXONLY_Pos;//Только прием mode:slave
		SPI2->CR1 |= 1 << SPI_CR1_SSM_Pos;// Программное управление mode:master
		SPI2->CR1 |= 1 << SPI_CR1_SSI_Pos;// Внутренний раб выбор mode:master
		SPI2->CR1 |= 0 << SPI_CR1_LSBFIRST_Pos;//Формат кадра LSB0
		SPI2->CR1 |= 1 << SPI_CR1_BR_Pos;// f/4
		SPI2->CR1 |= 1 << SPI_CR1_CPOL_Pos;// начальный фронт
		SPI2->CR1 |= 1 << SPI_CR1_CPHA_Pos;// фаза...
		
		//SPI2->CR2 =0;
		SPI2->CR2 |= 1 << SPI_CR2_RXDMAEN_Pos;// Включаем DMA
		SPI2->CR2 |= SPI_CR2_TXDMAEN;// Включаем DMA
	
    SPI2->CR1 |= 1 << SPI_CR1_SPE_Pos;//Вкл SPI
}

void Config_SPI_DMA1()
{
		//Stream 3-Channel 0 SPI2_RX, Stream 4 Channel 0 SPI2_TX = 000: channel 0 selected
		DMA1_Stream4->CR = 0;
		DMA1_Stream4->CR |= 0 << DMA_SxCR_CHSEL_Pos;  // Stream 4 Channel 0 SPI2_TX
		DMA1_Stream4->CR |= 0 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
		DMA1_Stream4->CR |= 0 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
		DMA1_Stream4->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
		DMA1_Stream4->CR |= 0 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
		DMA1_Stream4->CR |= 0 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти1
		DMA1_Stream4->CR |= 0 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
		DMA1_Stream4->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
		DMA1_Stream4->CR |= 1 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
		DMA1_Stream4->CR |= 0 << DMA_SxCR_CIRC_Pos;//кольцевой режим
		DMA1_Stream4->CR |= 1 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
		DMA1_Stream4->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
		DMA1_Stream4->PAR = (uint32_t)(&SPI2->DR);// Адрес регистра данных spi
		DMA1_Stream4->NDTR = 0;//размер массива
		DMA1_Stream4->M0AR = 0;// Адрес буфера
		DMA1_Stream4->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока 

		DMA1_Stream3->CR = 0;
		DMA1_Stream3->CR |= 0 << DMA_SxCR_CHSEL_Pos;  // Stream 3-Channel 0 SPI2_RX
		DMA1_Stream3->CR |= 0 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
		DMA1_Stream3->CR |= 0 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
	  DMA1_Stream3->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
		DMA1_Stream3->CR |= 0 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
	  DMA1_Stream3->CR |= 0 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти
		DMA1_Stream3->CR |= 0 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
 	  DMA1_Stream3->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
	  DMA1_Stream3->CR |= 1 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
		DMA1_Stream3->CR |= 1 << DMA_SxCR_CIRC_Pos;//кольцевой режим
	  DMA1_Stream3->CR |= 0 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
		DMA1_Stream3->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
		DMA1_Stream3->PAR = (uint32_t)(&SPI2->DR);// Адрес регистра данных spi
		DMA1_Stream3->NDTR = 0;//размер массива
		DMA1_Stream3->M0AR = 0;// Адрес буфера
		DMA1_Stream3->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока 
		
		
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4;
		DMA1->LIFCR |= DMA_LIFCR_CTCIF3;
		NVIC_EnableIRQ(DMA1_Stream4_IRQn);// Включение прерываний DMA
		NVIC_EnableIRQ(DMA1_Stream3_IRQn);// Включение прерываний DMA
		

}

///////////////////////
uint32_t SPI2_TransmitReceive(uint32_t data)
{
    while (!(SPI2->SR & SPI_SR_TXE)); // Ждём, пока TXE станет 1
    SPI2->DR = data;                  // Отправляем данные

    // Ждем, пока данные не будут приняты
		if(SPI2->SR & SPI_SR_RXNE)
		{
			return SPI2->DR;
		}
		else
		{
			return 0;
		}
}

uint32_t SPI2_DMA_TransmitReceive(uint32_t data,uint16_t size)
{
		uint32_t data1=0;
    while (!(SPI2->SR & SPI_SR_TXE)); // Ждём, пока TXE станет 1
		
    DMA1_Stream4->CR &= ~DMA_SxCR_EN; // Отключаем DMA после завершения передачи
    DMA1_Stream4->M0AR = (uint32_t)data; // Указание адреса буфера передачи
    DMA1_Stream4->NDTR = size;     
    DMA1_Stream4->CR |= DMA_SxCR_EN;     // Включаем DMA                  // Отправляем данные

    // Ждем, пока данные не будут приняты
		if(SPI2->SR & SPI_SR_RXNE)
		{
			DMA1_Stream3->CR &= ~DMA_SxCR_EN; // Отключаем DMA после завершения приема
			DMA1_Stream3->M0AR = (uint32_t)data1; // Указание адреса буфера приема
			DMA1_Stream3->NDTR = size;  
			DMA1_Stream3->PAR = (uint32_t)&SPI2->DR;		// Установка количества данных
			return data;
		}
		else
		{
			return 0;
		}
}


void SPI2_Receive(uint8_t *data, uint16_t size)
 {

		while (!(SPI2->SR & SPI_SR_RXNE)); // Ждем, пока RXNE станет 1
    DMA1_Stream3->CR &= ~DMA_SxCR_EN; // Отключаем DMA после завершения приема
    DMA1_Stream3->M0AR = (uint32_t)data; // Указание адреса буфера приема
    DMA1_Stream3->NDTR = size;  
		DMA1_Stream3->PAR = (uint32_t)&SPI2->DR;		// Установка количества данных
    //DMA1_Stream3->CR |= DMA_SxCR_EN;     // Включаем DMA

//		DMA1_Stream4->CR &= ~DMA_SxCR_EN; // Отключаем DMA после завершения передачи
//		DMA1_Stream4->M0AR = (uint32_t)_filler; // Указание адреса буфера передачи
//    DMA1_Stream4->NDTR = size;           // Установка количества данных
//    DMA1_Stream4->CR |= DMA_SxCR_EN;     // Включаем DMA

}

void SPI2_Transmit(uint8_t *data, uint16_t size)
 {
		while (!(SPI2->SR & SPI_SR_TXE)); // Ждем, пока TXE станет 1
		DMA1_Stream4->CR &= ~DMA_SxCR_EN; // Отключаем DMA после завершения передачи
    DMA1_Stream4->M0AR = (uint32_t)data; // Указание адреса буфера передачи
    DMA1_Stream4->NDTR = size;     
    DMA1_Stream4->CR |= DMA_SxCR_EN;     // Включаем DMA

    // Ждем завершения передачи
    
//    while (!(SPI2->SR & SPI_SR_RXNE)); // Ждем, пока RXNE станет 1

    // Очищаем флаг DMA после завершения
    
}

