#include "init_SPI_slave.h"

int sizeSPITx, sizeSPIRx=50;
char* dataBufSPITx="tests";;
char* dataBufSPIRx="";


void Init_SPI(void)//main init spi	
{
	Enable_RCC_AHB1_SPI();
	Enable_RCC_APB2_SPI();
	Config_GPIO_SPI();
	Config_SPI();
	Config_SPI_DMA2();
}

void Enable_RCC_AHB1_SPI()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//тактирование GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // тактирование DMA2
}

void Enable_RCC_APB2_SPI()
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;//тактирование SPI1
}

void  Config_GPIO_SPI()
{
		//PA5 (SCK), PA7 (MOSI) PA6 (MISO)
    // Настраиваем пины PA5, PA6, PA7 в альтернативную функцию
		GPIOA->MODER &= ~GPIO_MODER_MODE5;

    GPIOA->MODER |= GPIO_MODER_MODE5_1;   // Альтернативная функция для PA5

    GPIOA->MODER &= ~GPIO_MODER_MODE6;  // Очистка режима для PA6
    GPIOA->MODER |= GPIO_MODER_MODE6_1;   // Альтернативная функция для PA6

    GPIOA->MODER &= ~GPIO_MODER_MODE7;  // Очистка режима для PA7
    GPIOA->MODER |= GPIO_MODER_MODE7_1;   // Альтернативная функция для PA7

    // Настраиваем альтернативные функции (AF5 для SPI1)
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0|GPIO_AFRL_AFSEL5_2;// AF5 для SPI1 PA5 (SCK)
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_0|GPIO_AFRL_AFSEL6_2;// AF5 для SPI1 PA6 (MISO)
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL7_0|GPIO_AFRL_AFSEL7_2;// AF5 для SPI1 PA7 (MOSI)
}

void  Config_SPI()//slave mode
{
    SPI1->CR1 = 0;//reset

//		SPI1->CR1 |= SPI_CR1_MSTR;// master
//		SPI1->CR1 |= SPI_CR1_BIDIMODE;//включение режима двунаправленных данных mode:master
		SPI1->CR1 |= SPI_CR1_BIDIOE;//включение вывода в двунаправленном режиме
		SPI1->CR1 |= SPI_CR1_CRCEN;//аппаратный расчет CRC включен
		SPI1->CR1 |= SPI_CR1_CRCNEXT;//следующая передача CRC
		//SPI1->CR1 |= SPI_CR1_DFF;//16-битный формат кадра данных
		SPI1->CR1 |= SPI_CR1_RXONLY;//Только прием mode:slave
//		SPI1->CR1 |= SPI_CR1_SSM;// Программное управление mode:master
//		SPI1->CR1 |= SPI_CR1_SSI;// Внутренний раб выбор mode:master
		SPI1->CR1 |= SPI_CR1_LSBFIRST;//Формат кадра LSB
		SPI1->CR1 |= SPI_CR1_BR_1;// f/4
		SPI1->CR1 |= SPI_CR1_CPOL;// начальный фронт
		SPI1->CR1 |= SPI_CR1_CPHA;// фаза...
		
    SPI1->CR1 |= SPI_CR1_SPE;//Вкл SPI
}

void Config_SPI_DMA2()
{
		DMA2_Stream3->CR |=(DMA_SxCR_CHSEL_0|DMA_SxCR_CHSEL_1);  // Канал 3 USART1_TX
		DMA2_Stream3->CR |= DMA_SxCR_MSIZE;//размер ячейки 8бит
		//DMA2_Stream3->CR |= DMA_SxCR_PSIZE;//размер данных 8бит
		DMA2_Stream3->CR |= DMA_SxCR_MINC;//режим увеличения объема памяти
		DMA2_Stream3->CR |= DMA_SxCR_DIR_0;  // Из памяти в перефирию
		//DMA2_Stream3->CR |= DMA_SxCR_CIRC;//цикл передачи
		DMA2_Stream3->CR |= DMA_SxCR_TCIE;//прерывания
		DMA2_Stream3->NDTR = sizeSPITx;//размер массива
		DMA2_Stream3->PAR = (uint32_t)&SPI1->DR;// Адрес регистра данных spi
		DMA2_Stream3->M0AR = (uint32_t)dataBufSPITx;// Адрес буфера
		DMA2_Stream3->CR |= DMA_SxCR_EN;// Включение канала DMA
		
    DMA2_Stream0->CR |=(DMA_SxCR_CHSEL_0|DMA_SxCR_CHSEL_1);  // Канал 3 USART1_RX
    //DMA2_Stream0->CR |= DMA_SxCR_MSIZE;//размер ячейки 8бит
    DMA2_Stream0->CR |= DMA_SxCR_PSIZE;//размер данных 8бит
    DMA2_Stream0->CR |= DMA_SxCR_PINC;//режим увеличения объема памяти
    //DMA2_Stream0->CR |= DMA_SxCR_DIR;  // Из переферии в память
    DMA2_Stream0->CR |= DMA_SxCR_CIRC;//цикл приема
    DMA2_Stream0->CR |= DMA_SxCR_TCIE;//прерывания
    DMA2_Stream0->NDTR = sizeSPIRx;//размер массива
    DMA2_Stream0->PAR = (uint32_t)&SPI1->DR;// Адрес регистра данных spi
    DMA2_Stream0->M0AR = (uint32_t)dataBufSPIRx;// Адрес буфера
    DMA2_Stream0->CR |= DMA_SxCR_EN;// Включение канала DMA
		
		SPI1->CR2 = SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN; // Включаем DMA
		DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
		NVIC_EnableIRQ(DMA2_Stream3_IRQn);// Включение прерываний DMA
		NVIC_EnableIRQ(DMA2_Stream0_IRQn);// Включение прерываний DMA
}

int DMA2_SPI_GetStatus()//Проверим окончание чтения
{
	if((DMA2->LISR & DMA_LISR_TCIF0) == DMA_LISR_TCIF0)
	{
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
		return 1;
	}
		return 0;
}

int DMA2_SPI_ReadSize()//считываем массив 
{
	sizeSPIRx = strlen(dataBufSPIRx);
	return sizeSPIRx;
}

char* DMA2_SPI_ReadData()//считываем массив 
{
	return dataBufSPIRx;
}

void DMA2_SPI_SetString(char* str)//Установка строки по символьно
{
		DMA2_Stream7->CR &= ~DMA_SxCR_EN;
		sizeSPITx = strlen(str);
		
		DMA2_Stream7->NDTR = sizeSPITx;
		DMA2_Stream7->M0AR = (uint32_t)str;
		DMA2_Stream7->CR |= DMA_SxCR_EN;

}

void SPI_Transmit(uint8_t data)
{
    while (!(SPI1->SR & SPI_SR_TXE))//готовность Tx
		{
		};
    SPI1->DR = data;
}

uint8_t SPI_Receive()
{
    while (!(SPI1->SR & SPI_SR_RXNE))//готовность Rx
		{
		};
    return SPI1->DR;
}
