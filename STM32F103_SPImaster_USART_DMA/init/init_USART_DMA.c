#include "init_USART_DMA.h"

char* startStr="STM32F103";
int sizeTx, sizeRx=1;
char* dataBufTx;
char* dataBufRx;

void Init_USART(int baudRate)//main init usart
{
	//
	Enable_RCC_AHB();
	Enable_RCC_APB2();
	
	Config_LED();
	//
	Config_GPIO_USART(baudRate);
	Config_DMA1();
	DMA1_SetString(startStr);
}

void Enable_RCC_AHB()//DMA1
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
}

void Enable_RCC_APB2()//GpioC pin13 LED, GpioA usart1
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Включение порта C Led
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Включение порта A usart
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Включение  usart1
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

void Config_GPIO_USART(int baudRate)
{
		GPIOA->CRH |= (GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1); // TX
    GPIOA->CRH |= (GPIO_CRH_CNF10_0); // RX
	
		USART1->BRR = SystemCoreClock/baudRate; // SystemCoreClock/Baudrate 
		USART1->CR1 |= USART_CR1_UE ; // Включить USART
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE ; // Включить TX, RX
		USART1->CR1 |= USART_CR1_RXNEIE; // Включить прерывание
}

void Config_DMA1()
{
	dataBufRx=malloc(sizeRx * sizeof(char));
	
	//канал 4
		DMA1_Channel4->CCR |= (DMA_CCR4_PL_0 //Приоритет канала
													/*| DMA_CCR4_MSIZE_0*/ //Размер данных передатчика
													| DMA_CCR4_MINC //Автоматическое увеличение адреса передатчика
													| DMA_CCR4_DIR); // Направление передачи (от памяти к периферии
    //DMA1_Channel4->CNDTR = sizeTx; // Количество данных
    DMA1_Channel4->CPAR = (uint32_t)&USART1->DR; // Адрес регистра данных USART
    //DMA1_Channel4->CMAR = (uint32_t)dataBufTx; // Адрес буфера
		DMA1_Channel4->CCR |= DMA_CCR4_TCIE;// разрешение прерыван
    DMA1_Channel4->CCR |= DMA_CCR4_EN; // Включение канала DMA
		
		USART1->CR3 |=USART_CR3_DMAT;//переключили дма на усарт - передача, DMAT = Tx
	
		DMA1->IFCR |= DMA_IFCR_CTCIF4;//сбрасываем флаг прерывания
		NVIC_EnableIRQ(DMA1_Channel4_IRQn); // Включение прерываний DMA
	
	//канал 5
		DMA1_Channel5->CCR |= (DMA_CCR5_PL_0 //Приоритет канала
													| DMA_CCR5_PSIZE //Размер данных источника
													| DMA_CCR5_PINC //Автоматическое увеличение адреса источника
													| DMA_CCR5_CIRC); // циклический режим
    DMA1_Channel5->CNDTR = sizeRx; // Количество данных
    DMA1_Channel5->CPAR = (uint32_t)&USART1->DR; // Адрес регистра данных USART
    DMA1_Channel5->CMAR = (uint32_t)dataBufRx; // Адрес буфера
		DMA1_Channel5->CCR |= DMA_CCR5_TCIE;// разрешение прерыван
    DMA1_Channel5->CCR |= DMA_CCR5_EN; // Включение канала DMA
		
		USART1->CR3 |=USART_CR3_DMAR;//переключили дма на усарт - чтение, DMAR = Rx
	
		DMA1->IFCR |= DMA_IFCR_CTCIF5;//сбрасываем флаг прерывания
		NVIC_EnableIRQ(DMA1_Channel5_IRQn); // Включение прерываний DMA

}

int DMA1_GetStatus()//Проверим окончание чтения
{
	if(DMA1->ISR & DMA_ISR_TCIF5)
	{
		DMA1->IFCR |= DMA_IFCR_CTCIF5; // Очистка флага
		return 1;
	}
		return 0;
}

char DMA1_ReadChar()//считываем массив[0] 
{
	return dataBufRx[0];
}

void DMA1_SetString(char* str)//Установка строки по символьно
{
		DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
	
		sizeTx = strlen(str);

		DMA1_Channel4->CNDTR=sizeTx;// Количество данных
		DMA1_Channel4->CMAR = (uint32_t)str;// Адрес буфера
		DMA1_Channel4->CCR |= DMA_CCR4_EN;// Включение канала DMA
}
