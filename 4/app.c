#include "stm32f4xx.h" // Подключаем CMSIS для STM32F4

#define I2C1_ADDRESS 0x28 // Пример адреса устройства I2C (установите соответствующий вашему устройству)

void Delay(volatile uint32_t time) {
    for (; time != 0; time--);
}

void I2C1_Init(void) {
    // Включаем тактирование для GPIOB и I2C1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Тактирование GPIOB
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;  // Тактирование I2C1

    // Настраиваем выводы I2C (SCL на PB6, SDA на PB7)
    GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
    GPIOB->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1); // Альтернативная функция
		GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL6_Pos;// AF4 для I2C PB6 (SCL)
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL7_Pos;// AF4 для I2C PB7 (SDA)
	
    // Настраиваем I2C
    I2C1->CR1 = 0; // Сбрасываем настройки
    I2C1->CR2 = 16; // Частота тактирования = 8MHz / (16 + 1) = 0.5MHz
    I2C1->CCR = 80; // Временной период = 1000ns (при 8MHz)
    I2C1->TRISE = 17; // Максимально допустимое время повышения сигнала (должно быть >= 1000ns / 30ns)

    I2C1->CR1 |= I2C_CR1_PE; // Включаем I2C
}

void I2C1_Start(void) {
    while (I2C1->SR2 & I2C_SR2_BUSY); // Ждем, пока I2C не будет занят
    I2C1->CR1 |= I2C_CR1_START; // Генерируем старт
    while (!(I2C1->SR1 & I2C_SR1_SB)); // Ждем завершения старта
}

void I2C1_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP; // Генерируем стоп
}

void I2C1_SendAddress(uint8_t address) {
    I2C1->DR = address; // Отправляем адрес
    while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Ждем завершения передачи адреса
    (void)I2C1->SR2; // Читаем SR2 для сброса флага
}

void I2C1_SendData(uint8_t data) {
    I2C1->DR = data; // Отправляем данные
    while (!(I2C1->SR1 & I2C_SR1_TXE)); // Ждем, пока данные будут отправлены
}

uint8_t I2C1_ReceiveData(void) {
    while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Ждем, пока данные будут доступны
    return I2C1->DR; // Читаем полученные данные
}

int main(void) {
    I2C1_Init(); // Инициализация I2C

    I2C1_Start(); // Начало передачи
    I2C1_SendAddress(I2C1_ADDRESS << 1); // Отправляем адрес устройства с указанием на запись
    
    // Передаем данные
    I2C1_SendData(0xAA); // Пример данных, которые вы хотите отправить
    
    I2C1_Stop(); // Завершение передачи

    while (1); // Бесконечный цикл
}