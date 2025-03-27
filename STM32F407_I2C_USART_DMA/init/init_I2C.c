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
    RCC->APB1ENR |= 1 << RCC_APB1ENR_I2C1EN_Pos; // Включаем тактирование SPI2
    //RCC->AHB1ENR |= 1 << RCC_AHB1ENR_DMA1EN_Pos; // Включаем тактирование DMA1
}

void Config_GPIO_I2C()
{
//    //PB6 (SCL), PB7 (SDA)

    GPIOB->MODER |= 0 << GPIO_MODER_MODE6_Pos; // Очистка режима для PB6
    GPIOB->MODER |= 2 << GPIO_MODER_MODE6_Pos;   // Альтернативная функция для PB6 (SCL)

    GPIOB->MODER |= 0 << GPIO_MODER_MODE7_Pos;  // Очистка режима для PB7
    GPIOB->MODER |= 2 << GPIO_MODER_MODE7_Pos;   // Альтернативная функция для PB7 (SDA)

    GPIOB->AFR[0] |= 5 << GPIO_AFRL_AFSEL6_Pos;// AF4 для I2C PB6 (SCL)
    GPIOB->AFR[0] |= 5 << GPIO_AFRL_AFSEL7_Pos;// AF4 для I2C PB7 (SDA)
	
}

void Config_I2C()
{
//    I2C1->CR1 |= I2C_CR1_PE; // Включение I2C
//    I2C1->CR2 |= (16 << 0); // Частота 16 МГц, 16 тактов
//    I2C1->CCR = 80; // Настройка условного задержки
//    I2C1->TRISE = 17; // Максимальное время подъема
	
		//настраиваем модуль в режим I2C
	I2C2->CR1 &= ~2C_CR1_SMBUS;
	
        //указываем частоту тактирования модуля
	I2C2->CR2 &= ~I2C_CR2_FREQ;
	I2C2->CR2 |= 42; // Fclk1=168/4=42MHz 
	
        //конфигурируем I2C, standart mode, 100 KHz duty cycle 1/2	
	I2C2->CCR &= ~(I2C_CCR_FS | I2C_CCR_DUTY);
        //задаем частоту работы модуля SCL по формуле 10 000nS/(2* TPCLK1) 
	I2C2->CCR |= 208; //10 000ns/48ns = 208
	
	//Standart_Mode = 1000nS, Fast_Mode = 300nS, 1/42MHz = 24nS
	I2C2->TRISE = 42; //(1000nS/24nS)+1

        //включаем модуль
	I2C2->CR1 |= I2C_CR1_PE;

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
void I2C_Start(void) {
    I2C1->CR1 |= I2C_CR1_START; // Генерация стартового состояния
    while (!(I2C1->SR1 & I2C_SR1_SB)); // Ожидание завершения
}

void I2C_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP; // Генерация стоп-состояния
    //while (I2C1->CR1 & I2C_CR1_STOP); // Ожидание завершения
}

void I2C_SendByte(uint8_t data) {
    I2C1->DR = data; // Отправка байта
    while (!(I2C1->SR1 & I2C_SR1_TXE)); // Ожидание, пока передача завершится
}

uint8_t I2C_ReadByte(void) {
    while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Ожидание получения байта
    return I2C1->DR; // Чтение байта
}

void I2C_Write(uint8_t address, uint8_t *data, uint16_t size) {
    I2C_Start();
    I2C_SendByte(address << 1); // Адрес устройства и бит записи
    for (uint16_t i = 0; i < size; i++) {
        I2C_SendByte(data[i]);
    }
    I2C_Stop();
}

void I2C_Read(uint8_t address, uint8_t *data, uint16_t size) {
    I2C_Start();
    I2C_SendByte((address << 1) | 1); // Адрес устройства и бит чтения
    for (uint16_t i = 0; i < size - 1; i++) {
        data[i] = I2C_ReadByte();
    }
    I2C_Stop();
}

uint8_t I2C_TransmitReceive(uint8_t data)
{
    while (!(SPI2->SR & SPI_SR_TXE))
    {}
    SPI2->DR = data;

    while (!(SPI2->SR & SPI_SR_RXNE))
    {}
    return SPI2->DR;
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

