#include "init_I2C.h"

char dataBufRxSPI[SIZE_BUF];

void Init_I2C()
{
		Enable_RCC_I2C();
		Config_GPIO_I2C();
		Config_I2C();
		//Config_SPI1_DMA1();
}

void Enable_RCC_I2C(void)
{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//тактирование GPIOA
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;//тактирование
		//RCC->AHBENR |= RCC_AHBENR_DMA1EN;   // Включаем DMA1
}

void Config_GPIO_I2C()
{

 // Настраиваем PB6 (SCL), PB7 (SDA) как альтернативные функции

//		GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);//reset
//    GPIOB->CRL |= GPIO_CRL_MODE6; // PB6
//		GPIOB->CRL |= (GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1); // PB6 alt
//		
//		GPIOB->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);//reset
//		GPIOB->CRL |= GPIO_CRL_MODE7; // PB7
//		GPIOB->CRL |= (GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1); // PB7 alt



    GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
    GPIOB->CRL |= (GPIO_CRL_MODE6_1 | GPIO_CRL_CNF6); // PB6: Output, Open-drain
    GPIOB->CRL |= (GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7); // PB7: Output, Open-drain

}

void Config_I2C()
{

////    // Настройка I2C (100kHz)
//    I2C1->CR1 |= I2C_CR1_SWRST;  // Сброс I2C
//    I2C1->CR1 &= ~I2C_CR1_SWRST; // Снятие сброса

//    I2C1->CCR = 10; // Устанавливаем делитель для 100kHz
//    I2C1->TRISE = 10;  // Максимальное время подъема
//    I2C1->OAR1 = (0x50); // Установка адреса устройства

//    // Настройка I2C в режиме Slave
//    I2C1->CR1 = 0; // Отключаем I2C
//    I2C1->OAR1 = 0x30 << 1; // Устанавливаем адрес устройства
//    I2C1->CR1 |= I2C_CR1_ACK; // Включаем подтверждение
//    I2C1->CR1 |= I2C_CR1_PE; // Включаем I2C
	
////I2C1->CR1 &= ~I2C_CR1_PE; // Отключение I2C
////I2C1->CR2 = 8; // SYSCLK в МГц
////I2C1->CCR = 50;//210; // Настройка для 400 кГц
////I2C1->TRISE = 9;//43; // Настройка TRISE

//////I2C1->CR2 |= I2C_CR2_ITBUFEN;
//////I2C1->CR2 |= I2C_CR2_ITEVTEN;

//////I2C1->OAR1 = (0x1A); // Адрес слейва
//////I2C1->CR1 |= I2C_CR1_ACK;

////I2C1->CR1 |= I2C_CR1_PE; // Включение I2C

    // Настройка I2C
    I2C1->CR1 = 0; // Отключаем I2C
    I2C1->CR2 = 132; // Частота тактирования 36 МГц
    ///I2C1->CCR = 500; // Частота передачи 100 КГц
    ///I2C1->TRISE = 9; // Максимальное время для одной передачи
    I2C1->OAR1 = 0x30 << 1; // Устанавливаем адрес устройства
    I2C1->CR1 |= I2C_CR1_ACK; // Включаем подтверждение
    I2C1->CR1 |= I2C_CR1_PE; // Включаем I2C

}

void Config_SPI1_DMA1()
{
//Channel 2 SPI1_RX, Channel 3 SPI1_TX

		DMA1_Channel3->CCR |=0;
		DMA1_Channel3->CCR &= ~DMA_CCR3_MEM2MEM;//режим памяти в память
		DMA1_Channel3->CCR &= ~DMA_CCR3_PL;//уровень приоритета канала
		DMA1_Channel3->CCR &= ~DMA_CCR3_MSIZE_0;//размер памяти
		DMA1_Channel3->CCR &= ~DMA_CCR3_MSIZE_1;
		DMA1_Channel3->CCR &= ~DMA_CCR3_PSIZE_0;//размер периферии
		DMA1_Channel3->CCR &= ~DMA_CCR3_PSIZE_1;
		DMA1_Channel3->CCR |= DMA_CCR3_MINC;//Режим приращения памяти
		DMA1_Channel3->CCR &= ~DMA_CCR3_PINC;//Режим периферийного приращения
		DMA1_Channel3->CCR &= ~DMA_CCR3_CIRC;//Кольцевой режим
		DMA1_Channel3->CCR |= DMA_CCR3_DIR;//Направление передачи данных
		DMA1_Channel3->CCR |= DMA_CCR3_TCIE;//разрешение прерывания по завершению передачи
		DMA1_Channel3->CPAR = (uint32_t)(&SPI1->DR); //Адрес регистра данных spi
		DMA1_Channel3->CNDTR = 0; //размер массива
		DMA1_Channel3->CMAR = 0; //Адрес буфера
		DMA1_Channel3->CCR |= DMA_CCR3_EN; // Включение канала DMA

		DMA1_Channel2->CCR |=0;
		DMA1_Channel2->CCR &= ~DMA_CCR2_MEM2MEM;//режим памяти в память
		DMA1_Channel2->CCR &= ~DMA_CCR2_PL;//уровень приоритета канала
		DMA1_Channel2->CCR |= DMA_CCR2_MSIZE_0;//размер памяти
		DMA1_Channel2->CCR |= DMA_CCR2_MSIZE_1;
		DMA1_Channel2->CCR |= DMA_CCR2_PSIZE_0;//размер периферии
		DMA1_Channel2->CCR |= DMA_CCR2_PSIZE_1;
		DMA1_Channel2->CCR |= DMA_CCR2_MINC;//Режим приращения памяти
		DMA1_Channel2->CCR |= DMA_CCR2_PINC;//Режим периферийного приращения
		DMA1_Channel2->CCR |= DMA_CCR2_CIRC;//Кольцевой режим
		DMA1_Channel2->CCR &= ~DMA_CCR2_DIR;//Направление передачи данных
		DMA1_Channel2->CCR |= DMA_CCR2_TCIE;//разрешение прерывания по завершению передачи
		DMA1_Channel2->CPAR = (uint32_t)(&SPI1->DR); //Адрес регистра данных spi
		DMA1_Channel2->CNDTR = SIZE_BUF; //размер массива
		DMA1_Channel2->CMAR = (uint32_t)dataBufRxSPI; //Адрес буфера
		DMA1_Channel2->CCR |= DMA_CCR2_EN; // Включение канала DMA
		
		DMA1->IFCR |= DMA_IFCR_CTCIF3;
		DMA1->IFCR |= DMA_IFCR_CTCIF2;
		
		NVIC_EnableIRQ(DMA1_Channel3_IRQn); // Включение прерываний DMA
		NVIC_EnableIRQ(DMA1_Channel2_IRQn); // Включение прерываний DMA

}

//////////////
uint8_t SPI_TransmitReceive(uint8_t data)
{
	uint8_t data1;

		if(SPI1->SR & SPI_SR_RXNE)
		{
					data1=SPI1->DR;
			    while (!(SPI1->SR & SPI_SR_TXE)); // Ждём, пока TXE станет 1
					SPI1->DR = data1;  
			return SPI1->DR;
		}
		else
		{
			return SPI1->DR;
		}
}

char* Read_SPI1_DMA1()
{
    return dataBufRxSPI;
}


void SPI1_DMA1_TransmitReceive(char *str_data)
{
    DMA1_Channel3->CCR &= ~DMA_CCR3_EN;
    DMA1_Channel3->CNDTR = SIZE_BUF;		
    DMA1_Channel3->CMAR = (uint32_t)str_data; // Указание адреса буфера передачи
    DMA1_Channel3->CCR |= DMA_CCR3_EN;     // Включаем DMA
}


///////////////////////
//мер кода для режима Master:
//#include "stm32f10x.h" // Подключаем библиотеку для работы с STM32

//void I2C_Master_Init() {
//    // Включение тактирования для I2C1
//    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
//    
//    // Включаем тактирование для GPIOB (если используются PB6 и PB7)
//    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

//    // Настройка PB6 (SCL) и PB7 (SDA) на альтернативную функцию
//    GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
//    GPIOB->CRL |= (GPIO_CRL_MODE6_1 | GPIO_CRL_CNF6_1); // PB6: Output, Open-drain
//    GPIOB->CRL |= (GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7_1); // PB7: Output, Open-drain

//    // Настройка I2C
//    I2C1->CR1 = 0; // Отключаем I2C
//    I2C1->CR2 = 36; // Частота тактирования 36 МГц
//    I2C1->CCR = 180; // Частота передачи 100 КГц
//    I2C1->TRISE = 37; // Максимальное время для одной передачи
//    I2C1->CR1 |= I2C_CR1_PE; // Включаем I2C
//}

//void I2C_Master_Transmit(uint8_t address, uint8_t *data, uint16_t size) {
//    // Выбор адреса подчиненного устройства
//    I2C1->CR1 |= I2C_CR1_START;
//    while (!(I2C1->SR1 & I2C_SR1_SB)); // Ожидание старта передачи

//    I2C1->DR = address << 1; // Записываем адрес устройства
//    while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Ожидание завершения адресации

//    (void)I2C1->SR2; // Сброс флага адреса

//    for (uint16_t i = 0; i < size; i++) {
//        I2C1->DR = data[i]; // Передаем байт данных
//        while (!(I2C1->SR1 & I2C_SR1_BTF)); // Ожидание передаче полного байта
//    }

//    I2C1->CR1 |= I2C_CR1_STOP; // Отправка команды STOP


///////////////////////
//Пример кода для режима Slave:
//#include "stm32f10x.h" // Подключаем библиотеку для работы с STM32

//#define SLAVE_ADDRESS 0x30 // Адрес подчиненного устройства

//void I2C_Slave_Init() {
//    // Включение тактирования для I2C1
//    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

//    // Включаем тактирование для GPIOB (если используются PB6 и PB7)
//    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

//    // Настройка PB6 (SCL) и PB7 (SDA) на альтернативную функцию
//    GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
//    GPIOB->CRL |= (GPIO_CRL_CNF6_1); // PB6: Open-drain
//    GPIOB->CRL |= (GPIO_CRL_CNF7_1); // PB7: Open-drain

//    // Настройка I2C для режима Slave
//    I2C1->CR1 = 0; // Отключаем I2C
//    I2C1->OAR1 = SLAVE_ADDRESS << 1; // Устанавливаем адрес устройства
//    I2C1->CR1 |= I2C_CR1_ACK; // Включаем подтверждение
//    I2C1->CR1 |= I2C_CR1_PE; // Включаем I2C
//}

//void I2C_Slave_Receive() {
//    while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Ожидание адреса
//    (void)I2C1->SR2; // Сброс флага адреса

//    while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Ожидание поступления данных
//    uint8_t data = I2C1->DR; // Чтение данных
//    // Обработка полученных данных
//}