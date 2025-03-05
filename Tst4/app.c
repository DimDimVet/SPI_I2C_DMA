#include "stm32f4xx.h"  // Подключите соответствующий заголовочный файл
#include "delay.h"



/////////////////////////////////////////////////////////////
void LED7()//GpioA pin7 LED
{
    GPIOA->ODR ^= GPIO_ODR_OD7;
}

void LED6()//GpioA pin6 LED
{
    GPIOA->ODR ^= GPIO_ODR_OD6;
}

void Config_LED()//Config GpioA pin6 pin7 LED
{
		GPIOA->MODER &= ~ (GPIO_MODER_MODER6|GPIO_MODER_MODER7); // сброс
    GPIOA->MODER |= (GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0);//режим на выход
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6|GPIO_OSPEEDR_OSPEED7); // сброс скорости
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED6_0|GPIO_OSPEEDR_OSPEED7_0); // установка средней скорости
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD6|GPIO_PUPDR_PUPD7); // сброс режим подтяжки
    GPIOA->PUPDR |= (GPIO_PUPDR_PUPD6_0|GPIO_PUPDR_PUPD7_0); // установка подтяжки к + (1)  РА0
    GPIOA->OTYPER &= ~ (GPIO_OTYPER_OT6|GPIO_OTYPER_OT7); // сброс режима нагрузки
    GPIOA->OTYPER |= (GPIO_OTYPER_OT6|GPIO_OTYPER_OT7); // установка в режим с открытым коллектором
		GPIOA->ODR |= (GPIO_ODR_OD6|GPIO_ODR_OD7);
}
//////////////////////////////////////////////////////////

void GPIO_Config(void) {
    // Включаем тактирование GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

			//PA5 (SCK), PA7 (MOSI) PA6 (MISO)
    // Настраиваем пины PA5, PA6, PA7 в альтернативную функцию
    GPIOA->MODER &= ~(3 << (5 * 2));  // Очистка режима для PA5
    GPIOA->MODER |= (2 << (5 * 2));   // Альтернативная функция для PA5

    GPIOA->MODER &= ~(3 << (6 * 2));  // Очистка режима для PA6
    GPIOA->MODER |= (2 << (6 * 2));   // Альтернативная функция для PA6

    GPIOA->MODER &= ~(3 << (7 * 2));  // Очистка режима для PA7
    GPIOA->MODER |= (2 << (7 * 2));   // Альтернативная функция для PA7

    // Настраиваем альтернативные функции (AF5 для SPI1)
    GPIOA->AFR[0] |= (5 << (5 * 4)) | (5 << (6 * 4)) | (5 << (7 * 4)); // AF5 для SPI1
}

void SPI_Config(void) {
    // Включаем тактирование SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Сбрасываем настройки SPI
    SPI1->CR1 = 0;

//    // Настраиваем параметры SPI: мастер, частота делителя, режим
//    SPI1->CR1 |= SPI_CR1_MSTR        // Режим мастера
//               | SPI_CR1_BR_1 // Частота делителя (SPI_CLK / 16, можно настроить по необходимости)
//               | SPI_CR1_CPOL          // Полярность (0: первый фронт по восходящей)
//               | SPI_CR1_CPHA          // Фаза (0: данные считаются на третьем фронте)
//               | SPI_CR1_SSI           // Включение внутреннего уровня
//               | SPI_CR1_SSM;          // Установка SS в программном режиме


    // Включаем SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}

uint8_t SPI_TransmitReceive(uint8_t data) 
{
    // Ждем, пока не будет готов приемник
    while (!(SPI1->SR & SPI_SR_TXE))
		{
			//LED6();
		}; // Ждем, пока не станет готов передатчик

    // Отправляем данные
    SPI1->DR = data;

    // Ждем, пока не будет готов приемник
    while (!(SPI1->SR & SPI_SR_RXNE))
		{
			//LED7();
		}; // Ждем, пока не пришли данные

    // Читаем данные

		
    return SPI1->DR; // Возвращаем полученные данные
}

int main(void) {
    GPIO_Config(); // Настройка GPIO
		//Config_LED();
    SPI_Config();  // Настройка SPI
		uint8_t txData = 0xA6; // Пример передаваемых данных
		uint8_t rxData=0;
    while (1)
			{
				
				//LED7();
				//delay_s(1);
        
        rxData = SPI_TransmitReceive(txData); // Передача и прием данных

        // Обработка полученных данных (rxData) по необходимости
    }
}