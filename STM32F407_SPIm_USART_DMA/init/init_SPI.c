#include "init_SPI.h"

#define SIZESTR 1
uint8_t sizeSPI1=SIZESTR;
uint32_t dataBufSPI1=0xAA;
uint32_t dataBufSPIRx1;


void Init_SPI()//main init spi	
{
	Enable_RCC_SPI1();
	Config_GPIO_SPI1();
	Config_SPI();
	Config_SPI_DMA1();
}

void Enable_RCC_SPI1()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Включаем тактирование порта B
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // Включаем тактирование SPI2
	//RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // тактирование DMA2
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; // Включаем тактирование DMA1
}

void Config_GPIO_SPI1()
{
		//PB13(SCK), PB14(MISO), PB15(MOSI)
    
		GPIOB->MODER &= ~GPIO_MODER_MODE13; // Очистка режима для PA5
    GPIOB->MODER |= GPIO_MODER_MODE13_1;   // Альтернативная функция для PA5

    GPIOB->MODER &= ~GPIO_MODER_MODE14;  // Очистка режима для PA6
    GPIOB->MODER |= GPIO_MODER_MODE14_1;   // Альтернативная функция для PA6

    GPIOB->MODER &= ~GPIO_MODER_MODE15;  // Очистка режима для PA7
    GPIOB->MODER |= GPIO_MODER_MODE15_1;   // Альтернативная функция для PA7

    // Настраиваем альтернативные функции (AF5 для SPI2)
//		GPIOB->AFR[1] |= 5 << GPIO_AFRH_AFSEL13_Pos;// AF5 для SPI1 PA5 (SCK)
//		GPIOB->AFR[1] |= 5 << GPIO_AFRH_AFSEL14_Pos;// AF5 для SPI1 PA6 (MISO)
//		GPIOB->AFR[1] |= 5 << GPIO_AFRH_AFSEL15_Pos;// AF5 для SPI1 PA7 (MOSI)
		
		GPIOB->AFR[1] |= GPIO_AFRH_AFSEL13_0|GPIO_AFRH_AFSEL13_2;// AF5 для SPI1 PA5 (SCK)
		GPIOB->AFR[1] |= GPIO_AFRH_AFSEL14_0|GPIO_AFRH_AFSEL14_2;// AF5 для SPI1 PA6 (MISO)
		GPIOB->AFR[1] |= GPIO_AFRH_AFSEL15_0|GPIO_AFRH_AFSEL15_2;// AF5 для SPI1 PA7 (MOSI)
}

void Config_SPI()//slave mode
{
    SPI2->CR1 = 0;//reset

		SPI2->CR1 |= SPI_CR1_MSTR;// master
		//SPI2->CR1 |= SPI_CR1_BIDIMODE;//включение режима двунаправленных данных mode:master 0
		//SPI2->CR1 |= SPI_CR1_BIDIOE;//включение вывода в двунаправленном режиме 0
		//SPI2->CR1 |= SPI_CR1_CRCEN;//аппаратный расчет CRC включен 0
		//SPI2->CR1 |= SPI_CR1_CRCNEXT;//следующая передача CRC 0
		SPI2->CR1 |= SPI_CR1_DFF;//16-битный формат кадра данных
		//SPI2->CR1 |= SPI_CR1_RXONLY;//Только прием mode:slave
		SPI2->CR1 |= SPI_CR1_SSM;// Программное управление mode:master
		SPI2->CR1 |= SPI_CR1_SSI;// Внутренний раб выбор mode:master
		SPI2->CR1 |= SPI_CR1_LSBFIRST;//Формат кадра LSB
		SPI2->CR1 |= SPI_CR1_BR_1;// f/4
		SPI2->CR1 |= SPI_CR1_CPOL;// начальный фронт
		SPI2->CR1 |= SPI_CR1_CPHA;// фаза...
		
		SPI2->CR2 = SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN; // Включаем DMA
	
    SPI2->CR1 |= SPI_CR1_SPE;//Вкл SPI
}

void Config_SPI_DMA1()
{
		//Stream 3-Channel 0 SPI2_RX, Stream 4 Channel 0 SPI2_TX = 000: channel 0 selected
		DMA1_Stream4->CR &= ~(1 << DMA_SxCR_EN_Pos);
		DMA1_Stream4->CR |= DMA_SxCR_CHSEL_0;  // Stream 4 Channel 0 SPI2_TX
		DMA1_Stream4->CR |= 0 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
		DMA1_Stream4->CR |= 0 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
		DMA1_Stream4->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
		DMA1_Stream4->CR |= 0 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
		DMA1_Stream4->CR |= 0 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти
		DMA1_Stream4->CR |= 0x01 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
		DMA1_Stream4->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
		DMA1_Stream4->CR |= 1 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
		DMA1_Stream4->CR |= 0 << DMA_SxCR_CIRC_Pos;//кольцевой режим
		DMA1_Stream4->CR |= 0x01 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
		DMA1_Stream4->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
		DMA1_Stream4->PAR = (uint32_t)&SPI2->DR;// Адрес регистра данных spi
		DMA1_Stream4->NDTR = sizeSPI1;//размер массива
		DMA1_Stream4->M0AR = (uint32_t)dataBufSPI1;// Адрес буфера
		DMA1_Stream4->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока 

		DMA1_Stream3->CR &= ~(1 << DMA_SxCR_EN_Pos);
		DMA1_Stream3->CR |= DMA_SxCR_CHSEL_0;  // Stream 3-Channel 0 SPI2_RX
		DMA1_Stream3->CR |= 0 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
		DMA1_Stream3->CR |= 0 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
		DMA1_Stream3->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
		DMA1_Stream3->CR |= 0 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
		DMA1_Stream3->CR |= 0 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти
		DMA1_Stream3->CR |= 0x01 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
		DMA1_Stream3->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
		DMA1_Stream3->CR |= 1 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
		DMA1_Stream3->CR |= 1 << DMA_SxCR_CIRC_Pos;//кольцевой режим
		DMA1_Stream3->CR |= 0 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
		DMA1_Stream3->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
		DMA1_Stream3->PAR = (uint32_t)&SPI2->DR;// Адрес регистра данных spi
		DMA1_Stream3->NDTR = sizeSPI1;//размер массива
		DMA1_Stream3->M0AR = (uint32_t)dataBufSPIRx1;// Адрес буфера
		DMA1_Stream3->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока 
		
		
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4;
		DMA1->LIFCR |= DMA_LIFCR_CTCIF3;
		NVIC_EnableIRQ(DMA1_Stream4_IRQn);// Включение прерываний DMA
		NVIC_EnableIRQ(DMA1_Stream3_IRQn);// Включение прерываний DMA
}



///////////////////////
uint32_t SPI_TransmitReceive(uint32_t data)
{
	 // Ждем, пока не будет готово устройство для передачи
    while (!(SPI2->SR & SPI_SR_TXE)); // Ждём, пока TXE станет 1
    SPI2->DR = data;                  // Отправляем данные

    // Ждем, пока данные не будут приняты
    while (!(SPI2->SR & SPI_SR_RXNE)); // Ждём, пока RXNE станет 1
    return SPI2->DR;
}

void SPI2_Receive(uint8_t *data, uint16_t size) {
    DMA1_Stream3->M0AR = (uint32_t)data; // Указание адреса буфера приема
    DMA1_Stream3->NDTR = size;           // Установка количества данных
    DMA1_Stream3->CR |= DMA_SxCR_EN;     // Включаем DMA

    // Активируем прием
    SPI2->CR2 |= SPI_CR2_RXDMAEN; // Включаем DMA для приема

    // Ждем завершения приема
    while (!(SPI2->SR & SPI_SR_RXNE)); // Ждем, пока RXNE станет 1

    // Очищаем флаг DMA после завершения
    DMA1_Stream3->CR &= ~DMA_SxCR_EN; // Отключаем DMA после завершения приема
}

void SPI2_Transmit(uint8_t *data, uint16_t size) {
    DMA1_Stream3->M0AR = (uint32_t)data; // Указание адреса буфера передачи
    DMA1_Stream3->NDTR = size;           // Установка количества данных
    DMA1_Stream3->CR |= DMA_SxCR_EN;     // Включаем DMA

    // Активируем передачу
    SPI2->CR2 |= SPI_CR2_TXDMAEN; // Включаем DMA для передачи

    // Ждем завершения передачи
    while (!(SPI2->SR & SPI_SR_TXE)); // Ждем, пока TXE станет 1
    while (!(SPI2->SR & SPI_SR_RXNE)); // Ждем, пока RXNE станет 1

    // Очищаем флаг DMA после завершения
    DMA1_Stream3->CR &= ~DMA_SxCR_EN; // Отключаем DMA после завершения передачи
}

//void DMA1_Config(void) {
//    // Включение тактирования DMA1
//    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

//    // Настройка потока 3 DMA
//    DMA1->Stream3->CR = 0; // Сбрасываем настройки
//    DMA1->Stream3->PAR = (uint32_t)&SPI2->DR; // Адрес регистра данных SPI2
//    DMA1->Stream3->M0AR = 0; // Будем устанавливать буфер в функции передачи
//    DMA1->Stream3->NDTR = 0; // Количество данных будет установлено в функции передачи
//    DMA1->Stream3->CR |= (DMA_SxCR_DIR_0 | DMA_SxCR_MINC | DMA_SxCR_PSIZE_0 | DMA_SxCR_MSIZE_0 | DMA_SxCR_PL_1); // Настройка направления, инкремент, размер
//}



uint32_t SPI_Receive()
{
		if (SPI2->SR & SPI_SR_RXNE) 
			{ // Если есть данные на приеме
           return SPI2->DR; 
           
      };
			return 0;
}
