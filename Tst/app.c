
#include "stm32f10x.h"  // Подключите заголовочный файл для STM32F103


void GPIO_Config(void) {
    // Включаем тактирование GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Настраиваем PA5 (SCK), PA7 (MOSI) как альтернативные функции
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_MODE7 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
    GPIOA->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_CNF5_1) | // PA5: альтернативная функция, max 2MHz
                  (GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7_1);  // PA7: альтернативная функция, max 2MHz
                  
    // Настраиваем PA6 (MISO) как вход
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
    GPIOA->CRL |= (GPIO_CRL_CNF6_0); // PA6: флотирующий вход
}

void SPI_Config(void) {
    // Включаем тактирование SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Сбрасываем настройки SPI
    SPI1->CR1 = 0;

    // Настройка параметров SPI: Мастер, режим CPOL, CPHA, делитель частоты
    SPI1->CR1 |= SPI_CR1_MSTR       // Режим Мастера
                | SPI_CR1_SSM        // Включение режима программного управления
                | SPI_CR1_SSI        // Подтверждение
                | SPI_CR1_BR_1       // Настройка делителя частоты (Fpclk / 4)
                | SPI_CR1_CPOL       // Полярность (настраиваем в зависимости от устройства)
                | SPI_CR1_CPHA;      // Фаза (настраиваем в зависимости от устройства)

    // Включаем SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}

uint8_t SPI_TransmitReceive(uint8_t data) {
    // Ждем, пока не будет готово устройство для передачи
    while (!(SPI1->SR & SPI_SR_TXE)); // Ждём, пока TXE станет 1
    SPI1->DR = data;                  // Отправляем данные

    // Ждем, пока данные не будут приняты
    while (!(SPI1->SR & SPI_SR_RXNE)); // Ждём, пока RXNE станет 1
    return SPI1->DR;                  // Возвращаем принятые данные
}

int main(void) {
    GPIO_Config();    // Настраиваем GPIO
    SPI_Config();     // Настраиваем SPI1

    uint8_t sendData = 0xA5; // Данные для отправки
    uint8_t receivedData = 0;

    while (1) {
        receivedData = SPI_TransmitReceive(sendData); // Передаем и получаем данные
        // Здесь вы можете использовать receivedData, например, передать его куда-то или вывести на экран
    }

    return 0;
}