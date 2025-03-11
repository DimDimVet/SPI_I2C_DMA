
//#include "stm32f10x.h"  // Подключите заголовочный файл для STM32F103


//void GPIO_Config(void) {
//    // Включаем тактирование GPIOA
//    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

//    // Настраиваем PA5 (SCK), PA7 (MOSI) как альтернативные функции
//    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_MODE7 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
//    GPIOA->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_CNF5_1) | // PA5: альтернативная функция, max 2MHz
//                  (GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7_1);  // PA7: альтернативная функция, max 2MHz
//                  
//    // Настраиваем PA6 (MISO) как вход
//    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
//    GPIOA->CRL |= (GPIO_CRL_CNF6_0); // PA6: флотирующий вход
//}

//void SPI_Config(void) {
//    // Включаем тактирование SPI1
//    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

//    // Сбрасываем настройки SPI
//    SPI1->CR1 = 0;

//    // Настройка параметров SPI: Мастер, режим CPOL, CPHA, делитель частоты
//    SPI1->CR1 |= SPI_CR1_MSTR       // Режим Мастера
//                | SPI_CR1_SSM        // Включение режима программного управления
//                | SPI_CR1_SSI        // Подтверждение
//                | SPI_CR1_BR_1       // Настройка делителя частоты (Fpclk / 4)
//                | SPI_CR1_CPOL       // Полярность (настраиваем в зависимости от устройства)
//                | SPI_CR1_CPHA;      // Фаза (настраиваем в зависимости от устройства)

//    // Включаем SPI
//    SPI1->CR1 |= SPI_CR1_SPE;
//}

//uint8_t SPI_TransmitReceive(uint8_t data)
// {
//    // Ждем, пока не будет готово устройство для передачи
//    while (!(SPI1->SR & SPI_SR_TXE))
//		{
//		
//		}; // Ждём, пока TXE станет 1
//    SPI1->DR = data;                  // Отправляем данные

//    // Ждем, пока данные не будут приняты
//    while (!(SPI1->SR & SPI_SR_RXNE)); // Ждём, пока RXNE станет 1
//    return SPI1->DR;                  // Возвращаем принятые данные
//}

//int main(void) {
//    GPIO_Config();    // Настраиваем GPIO
//    SPI_Config();     // Настраиваем SPI1

//    uint8_t sendData = 0xA0; // Данные для отправки
//    uint8_t receivedData = 0;
//		uint8_t receivedData2 = 0;

//    while (1)
//		{
////				for(int i=0; i<10000;i++)
////				{
////				};
//        receivedData = SPI_TransmitReceive(sendData); // Передаем и получаем данные
//        receivedData2=receivedData;
//				// Здесь вы можете использовать receivedData, например, передать его куда-то или вывести на экран
//    }

//    return 0;
//}


#include "stm32f10x.h"  // Подключите заголовочный файл для STM32F103

   void DMA1_Channel3_IRQHandler(void) {
       if (DMA1->ISR & DMA_ISR_TCIF3) {
           // Обработка завершения передачи
           DMA1->IFCR |= DMA_IFCR_CTCIF3; // Сбрасываем флаг
       }
   }

   void DMA1_Channel2_IRQHandler(void) {
       if (DMA1->ISR & DMA_ISR_TCIF2) {
           // Обработка завершения приема
           DMA1->IFCR |= DMA_IFCR_CTCIF2; // Сбрасываем флаг
       }
   }

int main(void)
 {
 char* data_tx="Tst",data_rx;
 
 
   RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Включаем SPI1
   RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Включаем GPIOA
   RCC->AHBENR |= RCC_AHBENR_DMA1EN;   // Включаем DMA1

GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);
   GPIOA->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE7_1); // PA5, PA6, PA7
	 
	 SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BR_0; // Мастер, NSS, частота
   SPI1->CR2 = SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN; // Включаем DMA
   SPI1->CR1 |= SPI_CR1_SPE; // Включаем SPI1
	 
	 DMA1_Channel3->CNDTR = 50;             // Кол-во данных для передачи
   DMA1_Channel3->CMAR = (uint32_t)data_tx;   // Адрес данных для передачи
   DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR; // Адрес регистра данных SPI

   DMA1_Channel2->CNDTR = 50;             // Кол-во данных для приема
   DMA1_Channel2->CMAR = (uint32_t)data_rx;   // Адрес данных для приема
   DMA1_Channel2->CPAR = (uint32_t)&SPI1->DR; // Адрес регистра данных SPI
	 
	 DMA1_Channel3->CCR |= DMA_CCR3_EN; // Включаем передачу
   DMA1_Channel2->CCR |= DMA_CCR2_EN; // Включаем прием
	 
	 DMA1_Channel3->CCR |= DMA_CCR3_TCIE; // Разрешаем прерывание по завершению передачи
   NVIC_EnableIRQ(DMA1_Channel3_IRQn);
   NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	 
	 
	 DMA1_Channel3->CNDTR = 50;             // Кол-во данных для передачи
   DMA1_Channel3->CMAR = (uint32_t)data_tx;   // Адрес данных для передачи
   DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR;
	 
    while (1)
		{

    }

    return 0;
}