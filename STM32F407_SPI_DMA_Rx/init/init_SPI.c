#include "init_SPI.h"

int sizeTx, sizeRx=1;
char* dataBufTx="tests";;
char dataBufRx[1];

void Init_USART(int baudRate)//main init usart
{
	Enable_RCC();
	Config_LED();

	Config_GPIO_USART(baudRate);
	Config_DMA1();
	TstSPI();
}

void Enable_RCC()//GpioA usart, GpioA pin7 LED, USART1, DMA2
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
}

void LED()//GpioA pin7 LED
{
    GPIOA->ODR ^= GPIO_ODR_OD7;
}

void Config_LED()//Config GpioA pin6 pin7 LED
{
		GPIOA->MODER &= ~ (GPIO_MODER_MODER6|GPIO_MODER_MODER7); // сброс
    GPIOA->MODER |= (GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0);//режим на выход
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6|GPIO_OSPEEDR_OSPEED7); // сброс скорости
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED6_0|GPIO_OSPEEDR_OSPEED7_0); // установка средней скорости
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD6|GPIO_PUPDR_PUPD7); // сброс режим подтяжки
    GPIOA->PUPDR |= (GPIO_PUPDR_PUPD6_0|GPIO_PUPDR_PUPD7_0); // установка подтяжки к + (1)  РА0
    GPIOA->OTYPER &= ~ (GPIO_OTYPER_OT6|GPIO_OTYPER_OT7); // сброс режима нагрузки
    GPIOA->OTYPER |= (GPIO_OTYPER_OT6|GPIO_OTYPER_OT7); // установка в режим с открытым коллектором
		GPIOA->ODR |= (GPIO_ODR_OD6|GPIO_ODR_OD7);
}

void Config_GPIO_USART(int baudRate)
{
		GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);//// Настраиваем пины PA9 (TX) и PA10 (RX)

		GPIOA->MODER |= (GPIO_MODER_MODER9_1) | (GPIO_MODER_MODER10_1); // Установим альтернативный режим
		GPIOA->AFR[1] |= (GPIO_AFRH_AFSEL9_0|GPIO_AFRH_AFSEL9_1|GPIO_AFRH_AFSEL9_2) 
											| (GPIO_AFRH_AFSEL10_0|GPIO_AFRH_AFSEL10_1|GPIO_AFRH_AFSEL10_2); // AF7 для USART1

		USART1->BRR = SystemCoreClock/baudRate; // SystemCoreClock/Baudrate 
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; // Включить TX, RX
		USART1->CR2 |= (USART_CR2_STOP_1);//Установили STOP бит
		USART1->CR3 |= (USART_CR3_DMAR | USART_CR3_DMAT);
		
		USART1->CR1 |= USART_CR1_UE; // Включить USART2
}

void TstSPI()
{
	
	// Включаем тактирование GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Настраиваем PA5 (SCK), PA6 (MISO), PA7 (MOSI) как альтернативные функции
    GPIOA->MODER |= (2 << (5 * 2)) | (2 << (6 * 2)) | (2 << (7 * 2));  // Альтернативная функция
    GPIOA->AFR[0] |= (5 << (5 * 4)) | (5 << (6 * 4)) | (5 << (7 * 4)); // AF5 для SPI1
	
	// Включаем тактирование SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Настраиваем SPI1
    SPI1->CR1 = 0;
    SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM |
                 SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL |
                 SPI_CR1_CPHA; // Настройте параметры в соответствии с вашими требованиями
    SPI1->CR1 |= SPI_CR1_SPE; // Включаем SPI1
	

	
}


void Config_DMA1()
{
		DMA2_Stream7->CR |=(DMA_SxCR_CHSEL_2|DMA_SxCR_CHSEL_2);  // Канал 4 USART1_TX
		DMA2_Stream7->CR |= DMA_SxCR_MSIZE;//размер ячейки 8бит
		//DMA2_Stream7->CR |= DMA_SxCR_PSIZE;//размер данных 8бит
		DMA2_Stream7->CR |= DMA_SxCR_MINC;//режим увеличения объема памяти
		DMA2_Stream7->CR |= DMA_SxCR_DIR_0;  // Из памяти в перефирию
		//DMA2_Stream7->CR |= DMA_SxCR_CIRC;//цикл передачи
		DMA2_Stream7->CR |= DMA_SxCR_TCIE;//прерывания
		DMA2_Stream7->NDTR = sizeTx;//размер массива
		DMA2_Stream7->PAR = (uint32_t)&USART1->DR;// Адрес регистра данных USART
		DMA2_Stream7->M0AR = (uint32_t)dataBufTx;// Адрес буфера
		DMA2_Stream7->CR |= DMA_SxCR_EN;// Включение канала DMA
		
    DMA2_Stream2->CR |=(DMA_SxCR_CHSEL_2|DMA_SxCR_CHSEL_2);  // Канал 4 USART1_RX
    //DMA2_Stream2->CR |= DMA_SxCR_MSIZE;//размер ячейки 8бит
    DMA2_Stream2->CR |= DMA_SxCR_PSIZE;//размер данных 8бит
    DMA2_Stream2->CR |= DMA_SxCR_PINC;//режим увеличения объема памяти
    //DMA2_Stream2->CR |= DMA_SxCR_DIR;  // Из переферии в память
    DMA2_Stream2->CR |= DMA_SxCR_CIRC;//цикл приема
    DMA2_Stream2->CR |= DMA_SxCR_TCIE;//прерывания
    DMA2_Stream2->NDTR = sizeRx;//размер массива
    //DMA2_Stream2->PAR = (uint32_t)&USART1->DR;// Адрес регистра данных USART
		DMA2_Stream2->PAR = (uint32_t)&SPI1->DR;// Адрес регистра данных USART
    DMA2_Stream2->M0AR = (uint32_t)dataBufRx;// Адрес буфера
    DMA2_Stream2->CR |= DMA_SxCR_EN;// Включение канала DMA
		
		NVIC_EnableIRQ(DMA2_Stream7_IRQn);// Включение прерываний DMA
		NVIC_EnableIRQ(DMA2_Stream2_IRQn);// Включение прерываний DMA
}

int DMA2_GetStatus()//Проверим окончание чтения
{
	if((DMA2->LISR & DMA_LISR_TCIF2) == DMA_LISR_TCIF2)
	{
		DMA2->LIFCR |= DMA_LIFCR_CTCIF2;
		return 1;
	}
		return 0;
}

char DMA2_ReadChar()//считываем массив[0] 
{
	return dataBufRx[0];
}

void DMA2_SetString(char* str)//Установка строки по символьно
{
		DMA2_Stream7->CR &= ~DMA_SxCR_EN;
		sizeTx = strlen(str);
		DMA2_Stream7->NDTR = sizeTx;
		DMA2_Stream7->M0AR = (uint32_t)str;
		DMA2_Stream7->CR |= DMA_SxCR_EN;

}

