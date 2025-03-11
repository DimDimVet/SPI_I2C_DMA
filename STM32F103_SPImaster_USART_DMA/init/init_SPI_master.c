#include "init_SPI_master.h"

int sizeSPITx, sizeSPIRx=100;
char* dataBufSPIRx;

void Init_SPI(void)//main init spi
{
	Enable_RCC_APB2_SPI();
	Enable_AHBENR_SPI();
	Config_GPIO_SPI();
	Config_SPI();
	Config_SPI_DMA1();
}

void Enable_RCC_APB2_SPI()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//тактирование GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;//тактирование SPI1
}

void Enable_AHBENR_SPI()
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;   // Включаем DMA1
}

void  Config_GPIO_SPI()
{
		GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);//reset
    GPIOA->CRL |= GPIO_CRL_MODE5_1; // PA5 2MHz
		GPIOA->CRL |= GPIO_CRL_CNF5_1; // PA5 alt SCK
		
		GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);//reset
		GPIOA->CRL |= GPIO_CRL_MODE7_1; // PA7 2MHz
		GPIOA->CRL |= GPIO_CRL_CNF7_1; // PA7 alt MOSI
                  
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
    GPIOA->CRL |= (GPIO_CRL_CNF6_0); // PA6 input MISO
}

void  Config_SPI()
{
    SPI1->CR1 = 0;//reset
	
		SPI1->CR1 |= SPI_CR1_MSTR;// master
		SPI1->CR1 |= SPI_CR1_BIDIMODE;//включение режима двунаправленных данных mode:master
		SPI1->CR1 |= SPI_CR1_BIDIOE;//включение вывода в двунаправленном режиме
		SPI1->CR1 |= SPI_CR1_CRCEN;//аппаратный расчет CRC включен
		SPI1->CR1 |= SPI_CR1_CRCNEXT;//следующая передача CRC
		//SPI1->CR1 |= SPI_CR1_DFF;//16-битный формат кадра данных
		//SPI1->CR1 |= SPI_CR1_RXONLY;//Только прием mode:slave
		SPI1->CR1 |= SPI_CR1_SSM;// Программное управление mode:master
		SPI1->CR1 |= SPI_CR1_SSI;// Внутренний раб выбор mode:master
		SPI1->CR1 |= SPI_CR1_LSBFIRST;//Формат кадра LSB
		SPI1->CR1 |= SPI_CR1_BR_1;// f/4
		SPI1->CR1 |= SPI_CR1_CPOL;// начальный фронт
		SPI1->CR1 |= SPI_CR1_CPHA;// фаза...

    SPI1->CR1 |= SPI_CR1_SPE;//Вкл SPI
}

void Config_SPI_DMA1()
{
	dataBufSPIRx=malloc(sizeSPIRx * sizeof(char));
	
	//канал 3
		DMA1_Channel3->CCR |= (DMA_CCR3_PL_0 //Приоритет канала
													/*| DMA_CCR3_MSIZE_0*/ //Размер данных передатчика
													| DMA_CCR3_MINC //Автоматическое увеличение адреса передатчика
													| DMA_CCR3_DIR); // Направление передачи (от памяти к периферии
    //DMA1_Channel3->CNDTR = sizeSPITx; // Количество данных
    //DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR; // Адрес регистра данных SPI
    //DMA1_Channel3->CMAR = (uint32_t)dataBufTx; // Адрес буфера
		DMA1_Channel3->CCR |= DMA_CCR3_TCIE;// разрешение прерыван
    
		
		SPI1->CR2 |=SPI_CR2_TXDMAEN;//переключили дма на spi - передача, DMAT = Tx
		DMA1_Channel3->CCR |= DMA_CCR3_EN; // Включение канала DMA
		DMA1->IFCR |= DMA_IFCR_CTCIF3;//сбрасываем флаг прерывания
		NVIC_EnableIRQ(DMA1_Channel3_IRQn); // Включение прерываний DMA
	
	//канал 2
		DMA1_Channel2->CCR |= (DMA_CCR2_PL_0 //Приоритет канала
													| DMA_CCR2_PSIZE //Размер данных источника
													| DMA_CCR2_PINC //Автоматическое увеличение адреса источника
													| DMA_CCR2_CIRC); // циклический режим
    DMA1_Channel2->CNDTR = sizeSPIRx; // Количество данных
    DMA1_Channel2->CPAR = (uint32_t)&SPI1->DR; // Адрес регистра данных SPI
    DMA1_Channel2->CMAR = (uint32_t)dataBufSPIRx; // Адрес буфера
		DMA1_Channel2->CCR |= DMA_CCR2_TCIE;// разрешение прерыван
    
		
		SPI1->CR2 |=SPI_CR2_RXDMAEN;//переключили дма на spi - чтение, DMAR = Rx
		DMA1_Channel2->CCR |= DMA_CCR2_EN; // Включение канала DMA
		
		DMA1->IFCR |= DMA_IFCR_CTCIF2;//сбрасываем флаг прерывания
		NVIC_EnableIRQ(DMA1_Channel2_IRQn); // Включение прерываний DMA

}

void DMA1_SPI_SetString(char* str)//Установка строки по символьно
{
		SPI1->CR1 &= ~ SPI_CR1_SPE;
		DMA1_Channel3->CCR &= ~DMA_CCR3_EN;
		delay_ms(100);
		sizeSPITx = strlen(str);

		DMA1_Channel3->CNDTR=sizeSPITx;// Количество данных
		DMA1_Channel3->CMAR = (uint32_t)str;// Адрес буфера
		SPI1->CR1 |= SPI_CR1_SPE;//Вкл SPI
		DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR;
		DMA1_Channel3->CCR |= DMA_CCR3_EN;// Включение канала DMA
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