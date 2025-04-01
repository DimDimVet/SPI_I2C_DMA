#include "stm32f10x.h"

#define I2C_SLAVE_ADDRESS 0x28

volatile uint8_t received_data[10]; // Буфер для принятых данных
volatile uint8_t index = 0;          // Индекс для записи данных в буфер

void I2C1_Init(void) {
    // Включение тактирования I2C1 и портов A
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; 
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

//    // Настройка SDA и SCL (PA6, PA7) как альтернативные функции
//    GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
//    GPIOB->CRL |= (GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1); // Open-drain
	
		GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);//reset
    //GPIOB->CRL |= GPIO_CRL_MODE6_0; // PB6
		GPIOB->CRL |= GPIO_CRL_CNF6; // PB6 alt
		
		GPIOB->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);//reset
		//GPIOB->CRL |= GPIO_CRL_MODE7_0; // PB7
		GPIOB->CRL |= GPIO_CRL_CNF7; // PB7 alt

		I2C1->CR1 &= ~I2C_CR1_PE;  // Включение I2C
    // Настройка I2C1
    I2C1->CR2 = (8 << 0);  // Частота 8 МГц
    I2C1->CCR = 40;        // Формат SCL, duty cycle 1:2
    I2C1->TRISE = 9;       // Максимальная Rise Time
		I2C1->CR1 |= I2C_CR1_ACK;  // Подтверждать адрес
    I2C1->OAR1 =  I2C_SLAVE_ADDRESS; // Адрес
		I2C1->CR2 |= I2C_CR2_ITBUFEN;
		I2C1->CR2 |= I2C_CR2_ITERREN;
		I2C1->CR2 |= I2C_CR2_ITEVTEN;
    I2C1->CR1 |= I2C_CR1_PE;  // Включение I2C

    // Включение прерываний на I2C1
    
    NVIC_EnableIRQ(I2C1_EV_IRQn);
    NVIC_EnableIRQ(I2C1_ER_IRQn);
}

void I2C1_EV_IRQHandler(void) {
    if (I2C1->SR1 & I2C_SR1_ADDR) {
        // Прерывание при получении адреса
        (void)I2C1->SR2; // Чтение SR2 для сброса флага
        index = 0;      // Сбросить индекс
    }

    if (I2C1->SR1 & I2C_SR1_RXNE) {
        // Данные получены
        received_data[index++] = I2C1->DR; // Считать данные в буфер
        if (index >= sizeof(received_data)) {
            index = sizeof(received_data) - 1; // Ограничение по размеру
        }
    }
}

void I2C1_ER_IRQHandler(void) {
    // Обработка ошибок I2C
    uint32_t error = I2C1->SR1; // Чтение статуса для сброса флага
    (void)error; // Обработайте ошибку здесь при необходимости
}

int main(void) {
    SystemInit();   // Инициализация системного таймера и тактирования
    I2C1_Init();    // Инициализация I2C1

    while (1) {
        // В основном цикле можно добавить код обработки принятых данных
        // Например, вы можете обработать полученные данные из received_data
    }
}