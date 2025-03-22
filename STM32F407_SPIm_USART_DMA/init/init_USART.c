#include "init_USART.h"

void Init_USART1(int baudRate)	
{
	Enable_RCC_USART1();
	Config_GPIO_USART1();
	Config_USART1(baudRate);
	Config_USART1_DMA2();
}

void Enable_RCC_USART1()
{
	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOAEN_Pos; // Включаем тактирование порта A
	RCC->APB2ENR |= 1 << RCC_APB2ENR_USART1EN_Pos; // Включаем тактирование Usart1
	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_DMA2EN_Pos; // Включаем тактирование DMA2
}

void Config_GPIO_USART1()
{
		//PA9 (TX) и PA10 (RX)
				GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);//// Настраиваем пины PA9 (TX) и PA10 (RX)

		GPIOA->MODER |= (GPIO_MODER_MODER9_1) | (GPIO_MODER_MODER10_1); // Установим альтернативный режим
		GPIOA->AFR[1] |= (GPIO_AFRH_AFSEL9_0|GPIO_AFRH_AFSEL9_1|GPIO_AFRH_AFSEL9_2) 
											| (GPIO_AFRH_AFSEL10_0|GPIO_AFRH_AFSEL10_1|GPIO_AFRH_AFSEL10_2); // AF7 для USART1
//		GPIOA->MODER |= 0 << GPIO_MODER_MODER9_Pos; // Очистка режима для PA9
//    GPIOA->MODER |= 2 << GPIO_MODER_MODER9_Pos;   // Альтернативная функция для PA9 (TX)

//    GPIOA->MODER |= 0 << GPIO_MODER_MODER10_Pos;  // Очистка режима для PA10
//    GPIOA->MODER |= 2 << GPIO_MODER_MODER10_Pos;   // Альтернативная функция для PA10 (RX)

//    GPIOB->AFR[1] |= 7 << GPIO_AFRH_AFSEL9_Pos;// AF7 для USART1 PA9 (TX)
//		GPIOB->AFR[1] |= 7 << GPIO_AFRH_AFSEL10_Pos;// AF7 для USART1 PA10 (RX)
}

void Config_USART1(int baudRate)
{
//		USART1->BRR = SystemCoreClock/baudRate; // SystemCoreClock/Baudrate
//		
//    USART1->CR1 |= 1 << USART_CR1_TE_Pos; // Включить TX
//		USART1->CR1 |= 1 << USART_CR1_RE_Pos; // Включить RX
//		USART1->CR2 |= 2 << USART_CR2_STOP_Pos;//Установили STOP бит
//		USART1->CR3 |= 1 << USART_CR3_DMAR_Pos;
//		USART1->CR3 |= 1 << USART_CR3_DMAT_Pos;
//		
//		USART1->CR1 |= 1 << USART_CR1_UE_Pos; // Включить USART1

		USART1->BRR = SystemCoreClock/baudRate; // SystemCoreClock/Baudrate 
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; // Включить TX, RX
		USART1->CR2 |= (USART_CR2_STOP_1);//Установили STOP бит
		USART1->CR3 |= (USART_CR3_DMAR | USART_CR3_DMAT);
		
		USART1->CR1 |= USART_CR1_UE; // Включить USART2
}

void Config_USART1_DMA2()
{
		//Stream 7-Channel 4 USART1_TX, Stream 2 Channel 4 USART1_RX, 100 channel selected
		DMA2_Stream7->CR |=(DMA_SxCR_CHSEL_2|DMA_SxCR_CHSEL_2);  // Канал 4 USART1_TX
		DMA2_Stream7->CR |= DMA_SxCR_MSIZE;//размер ячейки 8бит
		//DMA2_Stream7->CR |= DMA_SxCR_PSIZE;//размер данных 8бит
		DMA2_Stream7->CR |= DMA_SxCR_MINC;//режим увеличения объема памяти
		DMA2_Stream7->CR |= DMA_SxCR_DIR_0;  // Из памяти в перефирию
		//DMA2_Stream7->CR |= DMA_SxCR_CIRC;//цикл передачи
		DMA2_Stream7->CR |= DMA_SxCR_TCIE;//прерывания
		DMA2_Stream7->NDTR = 5;//размер массива
		DMA2_Stream7->PAR = (uint32_t)&USART1->DR;// Адрес регистра данных USART
		DMA2_Stream7->M0AR = (uint32_t)5;// Адрес буфера
		DMA2_Stream7->CR |= DMA_SxCR_EN;// Включение канала DMA
		
//		DMA2_Stream7->CR = 0;
//		DMA2_Stream7->CR |= 4 << DMA_SxCR_CHSEL_Pos;  // Stream 7 Channel 4 USART1_TX
//		DMA2_Stream7->CR |= 0 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
//		DMA2_Stream7->CR |= 0 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
//		DMA2_Stream7->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
//		DMA2_Stream7->CR |= 0 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
//		DMA2_Stream7->CR |= 0 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти1
//		DMA2_Stream7->CR |= 0 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
//		DMA2_Stream7->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
//		DMA2_Stream7->CR |= 1 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
//		DMA2_Stream7->CR |= 0 << DMA_SxCR_CIRC_Pos;//кольцевой режим
//		DMA2_Stream7->CR |= 1 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
//		DMA2_Stream7->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
//		DMA2_Stream7->PAR = (uint32_t)(&USART1->DR);// Адрес регистра данных spi
//		DMA2_Stream7->NDTR = 0;//размер массива
//		DMA2_Stream7->M0AR = 0;// Адрес буфера
//		DMA2_Stream7->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока 

    DMA2_Stream2->CR |=(DMA_SxCR_CHSEL_2|DMA_SxCR_CHSEL_2);  // Канал 4 USART1_RX
    //DMA2_Stream2->CR |= DMA_SxCR_MSIZE;//размер ячейки 8бит
    DMA2_Stream2->CR |= DMA_SxCR_PSIZE;//размер данных 8бит
    DMA2_Stream2->CR |= DMA_SxCR_PINC;//режим увеличения объема памяти
    //DMA2_Stream2->CR |= DMA_SxCR_DIR;  // Из переферии в память
    DMA2_Stream2->CR |= DMA_SxCR_CIRC;//цикл приема
    DMA2_Stream2->CR |= DMA_SxCR_TCIE;//прерывания
    DMA2_Stream2->NDTR = 6;//размер массива
    DMA2_Stream2->PAR = (uint32_t)&USART1->DR;// Адрес регистра данных USART
    DMA2_Stream2->M0AR = (uint32_t)6;// Адрес буфера
    DMA2_Stream2->CR |= DMA_SxCR_EN;// Включение канала DMA

//		DMA2_Stream2->CR = 0;
//		DMA2_Stream2->CR |= 4 << DMA_SxCR_CHSEL_Pos;  // Stream 2-Channel 4 USART1_RX
//		DMA2_Stream2->CR |= 0 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
//		DMA2_Stream2->CR |= 0 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
//	  DMA2_Stream2->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
//		DMA2_Stream2->CR |= 0 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
//	  DMA2_Stream2->CR |= 0 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти
//		DMA2_Stream2->CR |= 0 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
// 	  DMA2_Stream2->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
//	  DMA2_Stream2->CR |= 1 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
//		DMA2_Stream2->CR |= 1 << DMA_SxCR_CIRC_Pos;//кольцевой режим
//	  DMA2_Stream2->CR |= 0 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
//		DMA2_Stream2->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
//		DMA2_Stream2->PAR = (uint32_t)(&USART1->DR);// Адрес регистра данных spi
//		DMA2_Stream2->NDTR = 0;//размер массива
//		DMA2_Stream2->M0AR = 0;// Адрес буфера
//		DMA2_Stream2->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока 
		
		DMA2->HIFCR |= 1 << DMA_HIFCR_CHTIF7_Pos;
		DMA2->LIFCR |= 1 << DMA_LIFCR_CTCIF2_Pos;
		NVIC_EnableIRQ(DMA2_Stream7_IRQn);// Включение прерываний DMA
		NVIC_EnableIRQ(DMA2_Stream2_IRQn);// Включение прерываний DMA
}
///////////////////////////
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
	char dataBufRxU[1];
	return dataBufRxU[0];
}

void DMA2_SetString(char* str)//Установка строки по символьно
{
		uint8_t sizeTxU;
		DMA2_Stream7->CR &= ~DMA_SxCR_EN;
		sizeTxU = strlen(str);
		DMA2_Stream7->NDTR = sizeTxU;
		DMA2_Stream7->M0AR = (uint32_t)str;
		DMA2_Stream7->CR |= DMA_SxCR_EN;

}
