#include "init_USART.h"

int cou=0;
uint8_t read[5];
uint8_t read1[5];
uint8_t dataDMARxUSART[5];

void Init_USART1(uint16_t baudRate)
{
	Enable_RCC_USART1();
	Config_GPIO_USART1();
	Config_USART1(baudRate);
	Config_USART1_DMA2();
}

void Enable_RCC_USART1(void)
{
	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOAEN_Pos;  // Включаем тактирование порта A
	RCC->APB2ENR |= 1 << RCC_APB2ENR_USART1EN_Pos; // Включаем тактирование Usart1
	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_DMA2EN_Pos; // Включаем тактирование DMA2
}

void Config_GPIO_USART1(void)
{
	// PA9 (TX) и PA10 (RX)
	GPIOA->MODER |= 0 << GPIO_MODER_MODER9_Pos; // Очистка режима для PA9
	GPIOA->MODER |= 2 << GPIO_MODER_MODER9_Pos; // Альтернативная функция для PA9 (TX)

	GPIOA->MODER |= 0 << GPIO_MODER_MODER10_Pos; // Очистка режима для PA10
	GPIOA->MODER |= 2 << GPIO_MODER_MODER10_Pos; // Альтернативная функция для PA10 (RX)

	GPIOA->AFR[1] |= 7 << GPIO_AFRH_AFSEL9_Pos;	 // AF7 для USART1 PA9 (TX)
	GPIOA->AFR[1] |= 7 << GPIO_AFRH_AFSEL10_Pos; // AF7 для USART1 PA10 (RX)
}

void Config_USART1(uint16_t baudRate)
{
	USART1->BRR = SystemCoreClock / baudRate; // SystemCoreClock/Baudrate

	USART1->CR1 |= 1 << USART_CR1_TE_Pos;	  // Включить TX
	USART1->CR1 |= 1 << USART_CR1_RE_Pos;	  // Включить RX
	USART1->CR2 |= 2 << USART_CR2_STOP_Pos;	  // Установили STOP бит
	//USART1->CR1 |= 1 << USART_CR1_RXNEIE_Pos; // Включить прерывание
	
  USART1->CR3 |= 1 << USART_CR3_DMAR_Pos;
  USART1->CR3 |= 1 << USART_CR3_DMAT_Pos;

	USART1->CR1 |= 1 << USART_CR1_UE_Pos; // Включить USART1
	NVIC_SetPriority(USART1_IRQn, 2); // Установите приоритет
	NVIC_EnableIRQ(USART1_IRQn); // Разрешить прерывания для USART2

}

void Config_USART1_DMA2()
{
    //Stream 7-Channel 4 USART1_TX, Stream 2 Channel 4 USART1_RX, 100 channel selected
    DMA2_Stream7->CR = 0;
    DMA2_Stream7->CR |= 4 << DMA_SxCR_CHSEL_Pos;  // Stream 7 Channel 4 USART1_TX
    DMA2_Stream7->CR |= 0 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
    DMA2_Stream7->CR |= 0 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
    DMA2_Stream7->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
    DMA2_Stream7->CR |= 0 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
    DMA2_Stream7->CR |= 0 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти1
    DMA2_Stream7->CR |= 0 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
    DMA2_Stream7->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
    DMA2_Stream7->CR |= 0 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
    DMA2_Stream7->CR |= 0 << DMA_SxCR_CIRC_Pos;//кольцевой режим
    DMA2_Stream7->CR |= 1 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
    DMA2_Stream7->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
    DMA2_Stream7->PAR = (uint32_t)(&USART1->DR);// Адрес регистра данных spi
    DMA2_Stream7->NDTR = 0;//размер массива
    DMA2_Stream7->M0AR = 0;// Адрес буфера
    DMA2_Stream7->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока

    //DMA2_Stream2->CR = 0;
    DMA2_Stream2->CR |= 4 << DMA_SxCR_CHSEL_Pos;  // Stream 2-Channel 4 USART1_RX
    DMA2_Stream2->CR |= 1 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
    DMA2_Stream2->CR |= 1 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
    DMA2_Stream2->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
    DMA2_Stream2->CR |= 1 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
    DMA2_Stream2->CR |= 1 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти1
    DMA2_Stream2->CR |= 1 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
    DMA2_Stream2->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
    DMA2_Stream2->CR |= 1 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
    DMA2_Stream2->CR |= 1 << DMA_SxCR_CIRC_Pos;//кольцевой режим
    DMA2_Stream2->CR |= 0 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
    DMA2_Stream2->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
    DMA2_Stream2->PAR = (uint32_t)(&USART1->DR);// Адрес регистра данных spi
    DMA2_Stream2->NDTR = 5;//размер массива
    DMA2_Stream2->M0AR = (uint32_t)dataDMARxUSART;// Адрес буфера
    DMA2_Stream2->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока

    DMA2->HIFCR |= 1 << DMA_HIFCR_CHTIF7_Pos;
    DMA2->LIFCR |= 1 << DMA_LIFCR_CTCIF2_Pos;
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);// Включение прерываний DMA
    NVIC_EnableIRQ(DMA2_Stream2_IRQn);// Включение прерываний DMA
}
///////////////////////////

void USART1_ReadString(uint8_t *data, uint8_t size_buf) // считываем регистр
{
	for (int i = 0; i < size_buf; i++)
	{
		while (!(USART1->SR & USART_SR_RXNE))
		{
		};

		uint8_t temp = USART1->DR;
		data[i] = temp;
	}
}

void USART1_SetString(uint8_t *str) // Установка строки по символьно
{
	uint8_t size = strlen((char*)str);
	
	if(size==0){return;}
	
	for (int i = 0; i < size; i++)
	{
		USART1->DR = str[i];

		while (!(USART1->SR & USART_SR_TXE)) // Проверим окончание передачи
		{
		};
	}
}

void USART1_DMA2_SetString(uint8_t* str,uint8_t size_buf)//Установка строки по символьно
{

			DMA2_Stream7->CR &= ~DMA_SxCR_EN;
			DMA2_Stream7->NDTR = size_buf;
			DMA2_Stream7->M0AR = (uint32_t)str;
			DMA2_Stream7->CR |= DMA_SxCR_EN;
}

void USART1_DMA2_ReadChar(uint8_t* readChar)//считываем массив[0]
{
	readChar[0]=(uint8_t)dataDMARxUSART[0];
	readChar[1]=(uint8_t)dataDMARxUSART[1];
	readChar[2]=(uint8_t)dataDMARxUSART[2];
	readChar[3]=(uint8_t)dataDMARxUSART[3];
	readChar[4]=(uint8_t)dataDMARxUSART[4];
	
}

// IRQ
void USART1_IRQHandler(void)
{
	//LED7();
	//
	ExecutorTerminal_USART_Irq();
	//
	//LED7();
}

void DMA2_Stream7_IRQHandler(void)
{
    if ((DMA2->HISR & DMA_HISR_TCIF7) == DMA_HISR_TCIF7)
    {
        DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
    }
		
    LED7();
}

void DMA2_Stream2_IRQHandler(void)
{
		
//    if ((DMA2->LISR & DMA_LISR_TCIF2) == DMA_LISR_TCIF2)
		if (DMA2->LISR & DMA_LISR_TCIF2)
    {
        //DMA2->LIFCR |= DMA_LIFCR_CTCIF2;
				USART1_DMA2_ReadChar(read);
				read1[cou]=read[0];
				cou++;
				if(cou > 4){DMA2->LIFCR |= DMA_LIFCR_CTCIF2;}
        LED6();
				//SPI2_DMA_TransmitReceive(receivedStringConsole);
    }
		LED6();
}

///

