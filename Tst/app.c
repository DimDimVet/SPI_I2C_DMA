#include "stm32f10x.h"  // Подключите заголовочный файл для STM32F103

//void GPIO_Config(void) {
//    // Включаем тактирование GPIOA
//    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
//    
//    // Настраиваем пины PA5, PA6, PA7 в альтернативную функцию
//    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
//    GPIOA->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_CNF5_1); // PA5: выход, альт. функция, 2 MHz
//    GPIOA->CRL |= (GPIO_CRL_MODE6_0 | GPIO_CRL_CNF6_0); // PA6: вход, флотирующий
//    GPIOA->CRL |= (GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7_1); // PA7: выход, альт. функция, 2 MHz
//}

//void SPI_Config(void) {
//    // Включаем тактирование SPI1
//    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

//    // Сбрасываем настройки SPI
//    SPI1->CR1 = 0;

//    // Настраиваем параметры SPI: Мастер, частота делителя, режим CPOL/CPHA
//    SPI1->CR1 |= SPI_CR1_MSTR       // Режим мастера
//                | SPI_CR1_SPE        // Включить SPI
//                | SPI_CR1_BR_1       // Частота SPI: Fpclk / 4 (можно настроить по необходимости)
//                | SPI_CR1_CPOL       // Полярность (если необходимо)
//                | SPI_CR1_CPHA;      // Фаза (если необходимо)

//    // Включаем SPI
//    SPI1->CR1 |= SPI_CR1_SPE;
//}

//uint8_t SPI_TransmitReceive(uint8_t data) {
//    // Ждем, пока не будет готово устройство для передачи
//    //while (!(SPI1->SR & SPI_SR_TXE)); // Проверка на наличие места для передачи
//    SPI1->DR = data;                  // Отправка данных

//    // Ждем, пока данные не будут приняты
//    //while (!(SPI1->SR & SPI_SR_RXNE)); // Проверка на наличие данных для приема
//    return SPI1->DR;                  // Прием данных и возвращение значения
//}

//int main(void) {
//    GPIO_Config(); // Настройка GPIO для SPI
//    SPI_Config();  // Настройка SPI

//    // Пример передачи и приема данных
//    uint8_t txData = 0xAA; // Данные для передачи
//    uint8_t rxData;

//    while (1) {
//        rxData = SPI_TransmitReceive(txData); // Передача данных
//        // Здесь добавьте обработку rxData по необходимости
//    }
//}


 uint8_t SPI_TransmitReceive(uint8_t data) {
       SPI1->DR = data; // Отправка данных
       while (!(SPI1->SR & SPI_SR_RXNE)); // Ожидание окончания приема
       return SPI1->DR; // Возврат принятого байта
   }

void Test_SPI1() {
       uint8_t transmit_data = 0x55; // Данные для передачи
       uint8_t received_data;
       
       received_data = SPI_TransmitReceive(transmit_data); // Передача и прием
       
       // Обработка полученных данных, например:
       if (received_data == transmit_data) {
           // Успешная передача
       } else {
           // Ошибка передачи
       }
   }

int main()
	{
		
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
   RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
		
		   GPIOA->CRL &= ~((0xF << (4 * 5)) | (0xF << (4 * 6)) | (0xF << (4 * 7)));
   GPIOA->CRL |= (0xB << (4 * 5)); // PA5 - SCK
   GPIOA->CRL |= (0x4 << (4 * 6)); // PA6 - MISO
   GPIOA->CRL |= (0xB << (4 * 7)); // PA7 - MOSI
		
		SPI1->CR1 = 0; // Сброс настроек
   SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SPE; // Master, Включить SPI
		
       // Настройка тактирования, GPIO и SPI1
       
       while (1) {
           Test_SPI1(); // Выполнение теста SPI
           // Задержка или другая логика
       }
   }