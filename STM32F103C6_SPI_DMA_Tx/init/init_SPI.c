#include "init_SPI.h"

char* startStr="STM32F103";
int sizeTx, sizeRx=1;
char* dataBufTx;
char* dataBufRx;

void Init_SPI()
{
	//
	Enable_RCC_AHB();
	Enable_RCC_APB2();
	
	Config_LED();
	//
	Config_GPIO_SPI();
	Config_SPI1();
	Config_DMA1();
	DMA1_SetString(startStr);
}

void Enable_RCC_AHB()//DMA1
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
}


void Enable_RCC_APB2()//GpioC pin13 LED
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Включение порта C Led
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Включение порта A PA4(CS) PA5(SCK), PA6(MISO), PA7(MOSI)
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Включение SPI1
}

void LED()//GpioC pin13 LED
{
    GPIOC->ODR ^= GPIO_ODR_ODR13;
}

void Config_LED()//Config GpioC pin13 LED
{
		GPIOC->CRH &=~(GPIO_CRH_CNF13|GPIO_CRH_MODE13);
    GPIOC->CRH|=(~GPIO_CRH_CNF13)|GPIO_CRH_MODE13_1;
		GPIOC->BSRR = GPIO_BSRR_BS13;        // Установить
    GPIOC->BSRR = GPIO_BSRR_BR13;        // Сбросить
}

void Config_GPIO_SPI()
{
		//PA5(SCK)
		GPIOA->CRL &= ~(GPIO_CRL_CNF5);
		GPIOA->CRL |= GPIO_CRL_MODE5_0;//10Мгц
		GPIOA->CRL |= GPIO_CRL_CNF5_1;//альт SCK
		//PA7(MOSI)
		GPIOA->CRL &= ~(GPIO_CRL_CNF7);
		GPIOA->CRL |= GPIO_CRL_MODE7_0;//10Мгц
		GPIOA->CRL |= GPIO_CRL_CNF7_1;//альт MOSI
		//PA4(CS)
		GPIOA->CRL &= ~(GPIO_CRL_CNF4);
		GPIOA->CRL |= GPIO_CRL_MODE4_0;//10Мгц
		GPIOA->CRL |= GPIO_CRL_CNF4_1;//альт CS
		//PA6(CS)
		GPIOA->CRL &= ~(GPIO_CRL_CNF6);
		GPIOA->CRL |= GPIO_CRL_CNF6_0;//альт MISO
//	
//		USART1->BRR = SystemCoreClock/baudRate; // SystemCoreClock/Baudrate 
//		USART1->CR1 |= USART_CR1_UE ; // Включить USART
//    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE ; // Включить TX, RX
//		USART1->CR1 |= USART_CR1_RXNEIE; // Включить прерывание

}

void Config_SPI1()
{
	SPI1->CR1 = 0; // Сброс настроек
	SPI1->CR1 |= SPI_CR1_MSTR;//мастер
	SPI1->CR1 |= SPI_CR1_BR_2;//100: fPCLK/32
	SPI1->CR1 |= SPI_CR1_CPOL;//0 покоя
	SPI1->CR1 |= SPI_CR1_CPHA;//начальный фронт
	SPI1->CR1 |= SPI_CR1_SSM;//вкл.NSS
	SPI1->CR1 |= SPI_CR1_SSI;//NSS в 1
	
	SPI1->CR1 |= SPI_CR1_SPE;//вкд. SPI

}

void Config_DMA1()
{
	dataBufRx=malloc(sizeRx * sizeof(char));
	
	//канал 2 SPI1 Tx
		DMA1_Channel2->CCR |= DMA_CCR2_PL_0; //Приоритет канала
		//DMA1_Channel2->CCR |= DMA_CCR2_MSIZE_0; //Размер данных передатчика
		DMA1_Channel2->CCR |= DMA_CCR2_MINC; //Автоматическое увеличение адреса передатчика
		DMA1_Channel2->CCR |= DMA_CCR2_DIR; // Направление передачи (от памяти к периферии
    //DMA1_Channel2->CNDTR = sizeTx; // Количество данных
    DMA1_Channel2->CPAR = (uint32_t)&SPI1->DR; // Адрес регистра данных USART
    //DMA1_Channel2->CMAR = (uint32_t)dataBufTx; // Адрес буфера
		DMA1_Channel2->CCR |= DMA_CCR2_TCIE;// разрешение прерыван
    DMA1_Channel2->CCR |= DMA_CCR2_EN; // Включение канала DMA
		
		SPI1->CR2 |= SPI_CR2_TXDMAEN;// подключили dma к spi
		//USART1->CR3 |=USART_CR3_DMAT;//переключили дма на усарт - передача, DMAT = Tx
	
		DMA1->IFCR |= DMA_IFCR_CTCIF2;//сбрасываем флаг прерывания
		NVIC_EnableIRQ(DMA1_Channel2_IRQn); // Включение прерываний DMA
	
	//канал 3 SPI1 Кx
		DMA1_Channel3->CCR |= DMA_CCR3_PL_0; //Приоритет канала
		DMA1_Channel3->CCR |= DMA_CCR3_PSIZE; //Размер данных источника
		DMA1_Channel3->CCR |= DMA_CCR3_PINC; //Автоматическое увеличение адреса источника
		DMA1_Channel3->CCR |= DMA_CCR3_CIRC; // циклический режим
    DMA1_Channel3->CNDTR = sizeRx; // Количество данных
    DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR; // Адрес регистра данных USART
    DMA1_Channel3->CMAR = (uint32_t)dataBufRx; // Адрес буфера
		DMA1_Channel3->CCR |= DMA_CCR3_TCIE;// разрешение прерыван
    DMA1_Channel3->CCR |= DMA_CCR3_EN; // Включение канала DMA
		
		SPI1->CR2 |= SPI_CR2_RXDMAEN;// подключили dma к spi
		//USART1->CR3 |=USART_CR3_DMAR;//переключили дма на усарт - чтение, DMAR = Rx
	
		DMA1->IFCR |= DMA_IFCR_CTCIF5;//сбрасываем флаг прерывания
		NVIC_EnableIRQ(DMA1_Channel3_IRQn); // Включение прерываний DMA

}

int DMA1_GetStatus()//Проверим окончание чтения
{
	if(DMA1->ISR & DMA_ISR_TCIF3)
	{
		DMA1->IFCR |= DMA_IFCR_CTCIF3; // Очистка флага
		return 1;
	}
		return 0;
}

char DMA1_ReadChar()//считываем массив[0] 
{
	return dataBufRx[0];
}

uint8_t SPI_TransmitReceive(uint8_t data)
{
       SPI1->DR = data; // Отправка данных
       //while (!(SPI1->SR & SPI_SR_RXNE)); // Ожидание окончания приема
       return SPI1->DR; // Возврат принятого байта
}

void DMA1_SetString(char* str)//Установка строки по символьно
{
		DMA1_Channel2->CCR &= ~DMA_CCR2_EN;
	
		sizeTx = strlen(str);

		DMA1_Channel2->CNDTR=sizeTx;// Количество данных
		DMA1_Channel2->CMAR = (uint32_t)str;// Адрес буфера
		DMA1_Channel2->CCR |= DMA_CCR2_EN;// Включение канала DMA
	
	uint8_t received_data;

	for(int i=0;i<sizeTx;i++)
	{
		received_data=SPI_TransmitReceive(str[i]);
		// Обработка полученных данных, например:
       if (received_data == str[i]) 
				 {
           //LED();
       } else {
           //LED();
       }

	}
		
}

