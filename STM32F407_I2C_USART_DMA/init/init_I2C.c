#include "init_I2C.h"

uint32_t dataBufRxSPI[1];

void Init_I2C()
{
    Enable_RCC_I2C();
    Config_GPIO_I2C();
    Config_I2C();
    //Config_I2C_DMA1();
}

void Enable_RCC_I2C()
{
    RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOBEN_Pos; // Включаем тактирование порта B
    RCC->APB1ENR |= 1 << RCC_APB1ENR_I2C1EN_Pos; // Включаем тактирование 
    //RCC->AHB1ENR |= 1 << RCC_AHB1ENR_DMA1EN_Pos; // Включаем тактирование DMA1

}

void Config_GPIO_I2C()
{
////    //PB6 (SCL), PB7 (SDA)

    GPIOB->MODER |= 0 << GPIO_MODER_MODE6_Pos; // Очистка режима для PB6
    GPIOB->MODER |= 2 << GPIO_MODER_MODE6_Pos;   // Альтернативная функция для PB6 (SCL)
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT6_Pos;//открытый коллектор
		GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED6_Pos;//скорость
		//GPIOB->PUPDR |= 2 << GPIO_PUPDR_PUPD6_Pos;//подтянем+

    GPIOB->MODER |= 0 << GPIO_MODER_MODE7_Pos;  // Очистка режима для PB7
    GPIOB->MODER |= 2 << GPIO_MODER_MODE7_Pos;   // Альтернативная функция для PB7 (SDA)
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT7_Pos;//открытый коллектор
		GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED7_Pos;//скорость
		//GPIOB->PUPDR |= 2 << GPIO_PUPDR_PUPD7_Pos;//подтянем+
	
//		GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
//    GPIOB->MODER |= (2 << GPIO_MODER_MODER6_Pos | 2 << GPIO_MODER_MODER7_Pos); // Альтернативная функция
//	
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL6_Pos;// AF4 для I2C PB6 (SCL)
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL7_Pos;// AF4 для I2C PB7 (SDA)
	
//	GPIOB->MODER &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
//    GPIOB->MODER |= (GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1); // Альтернативный режим
//	GPIOB->AFR[0] |= (4 << (6 * 4)) | (4 << (7 * 4)); // AF4 для I2C1
//    //GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7); // Открытый коллектор
//    GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7); // Высокая скорость
//    //GPIOB->PUPDR |= (GPIO_PUPDR_PUPD6_1 | GPIO_PUPDR_PUPD7_1); // Подтяжка к питанию
		
}

void Config_I2C()
{
//		I2C1->CR1=I2C_CR1_SWRST;//сброс
//		I2C1->CR1&=~I2C_CR1_SWRST;//отключили сброс
//		
//		I2C1->CR2 |= 16 << I2C_CR2_FREQ_Pos;//установка частоты
//		I2C1->CCR |= 80 << I2C_CCR_CCR_Pos;// Настройка условного задержки
//		I2C1->TRISE |= 17 << I2C_TRISE_TRISE_Pos;// Максимальное время подъема
//		
//		I2C1->CR1 |= 1 << I2C_CR1_PE_Pos;
	
//	    // Настройка I2C
//    I2C1->CR1 = 0; // Отключаем I2C
//    I2C1->CR2 = 0x10; // Устанавливаем частоту 16 МГц
//    I2C1->CCR = 0x28; // Устанавливаем коэффициент для 100 кГц
//    I2C1->TRISE = 0x10; // Максимальное время восходящего фронта
//    I2C1->CR1 |= I2C_CR1_PE; // Включаем I2C
	
        I2C1->CR1 &= ~I2C_CR1_PE; // Отключаем I2C
        I2C1->CR2 = 16; //42; // Частота APB1 в MHz
//				I2C1->OAR1 = (0x2A << 1); // Адрес слейва
//				I2C1->CR1 |= I2C_CR1_ACK;
        I2C1->CCR = 180; //210; // Настройка скорости I2C для 100kHz
        I2C1->TRISE = 9; //43; // TRISE
        I2C1->CR1 |= I2C_CR1_PE; // Включаем I2C

}

void Config_I2C_DMA1()
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
    DMA1_Stream4->CR |= 0 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
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
    DMA1_Stream3->CR |= 2 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти
    DMA1_Stream3->CR |= 2 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
    DMA1_Stream3->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
    DMA1_Stream3->CR |= 1 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
    DMA1_Stream3->CR |= 1 << DMA_SxCR_CIRC_Pos;//кольцевой режим
    DMA1_Stream3->CR |= 0 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
    DMA1_Stream3->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
    DMA1_Stream3->PAR = (uint32_t)(&SPI2->DR);// Адрес регистра данных spi
    DMA1_Stream3->NDTR = 1;//размер массива
    DMA1_Stream3->M0AR = (uint32_t)dataBufRxSPI;// Адрес буфера
    DMA1_Stream3->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока

    DMA1->HIFCR |= 1 << DMA_HIFCR_CHTIF4_Pos;
    DMA1->LIFCR |= 1 << DMA_LIFCR_CTCIF3_Pos;
    NVIC_EnableIRQ(DMA1_Stream4_IRQn);// Включение прерываний DMA
    NVIC_EnableIRQ(DMA1_Stream3_IRQn);// Включение прерываний DMA
}

///////////////////////
void I2C_Start(void) 
{
		I2C1->CR1 |= 1 << I2C_CR1_ACK_Pos;
    I2C1->CR1 |= 1 << I2C_CR1_START_Pos; // Генерация стартового состояния
    while(!(I2C1->SR1 & I2C_SR1_SB)); // Ожидание завершения
}

void I2C_Stop(void) 
{
    I2C1->CR1 |=1 << I2C_CR1_STOP_Pos; // Генерация стоп-состояния
    while (I2C1->CR1 & I2C_SR1_STOPF); // Ожидание завершения
}

void I2C1_SendAddress(uint8_t addr)
{
    I2C1->DR = addr; // Отправка адреса
    while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Ждем подтверждения
    I2C1->SR2; // Сброс флага
}

void I2C1_SendData(uint8_t data) 
{
    I2C1->DR = data; // Отправка данных
    while (!(I2C1->SR1 & I2C_SR1_TXE)); // Ждем, пока передатчик готов
}

uint8_t I2C1_ReceiveData() 
{
    while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Ожидание получения байта
    return I2C1->DR; // Чтение байта
}

void I2C_Write(uint8_t address, uint8_t *data, uint8_t size)
{
    I2C_Start();
		
    I2C1_SendAddress(address << 1); // Адрес устройства и бит записи
	
    for (uint8_t i = 0; i < size; i++)
		{
        I2C1_SendData(data[i]);
    }
		
    I2C_Stop();
}

void I2C_Read(uint8_t address, uint8_t *data, uint8_t size)
{
    I2C_Start();
	
    I2C1_SendAddress((address << 1) | 1); // Адрес устройства и бит чтения
	
    for (uint16_t i = 0; i < size - 1; i++)
		{
        data[i] = I2C1_ReceiveData();
    }
		
    I2C_Stop();
}

uint8_t* I2C_Scan_Bus(uint8_t count)
{
		uint8_t* count_device;
		
		if(count <=0)
		{
				count=128;
		};
		
		for (uint8_t i=0; i < count; i++)
		{
				I2C_Start();
			
				I2C1->DR=(i<<1|0); 
        while(!(I2C1->SR1)|!(I2C1->SR2))
				{};
					
				I2C_Stop();
				*count_device++ =(I2C1->SR1&I2C_SR1_ADDR);
		};
		
		return count_device;
}

uint32_t* Read_I2C_DMA()
{
    return dataBufRxSPI;
}

void I2C_DMA_TransmitReceive(char *str_data)
{
		uint8_t sizeTxU = strlen(str_data);
	
    DMA1_Stream4->CR &= ~DMA_SxCR_EN;
    DMA1_Stream4->NDTR = sizeTxU;		
    DMA1_Stream4->M0AR = (uint32_t)str_data; // Указание адреса буфера передачи
    DMA1_Stream4->CR |= DMA_SxCR_EN;     // Включаем DMA
}

