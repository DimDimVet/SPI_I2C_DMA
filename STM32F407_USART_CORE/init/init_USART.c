#include "init_USART.h"

void Init_USART(int baudRate)//main init usart
{
	Enable_RCC_AHB1();
	Config_LED();
	//
	Enable_RCC_APB1();
	
	Config_GPIO_USART(baudRate);
}

void Enable_RCC_AHB1()//GpioA usart, GpioA pin7 LED
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}

void Enable_RCC_APB1()//Usart2
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
}

void LED()//GpioA pin7 LED
{
    GPIOA->ODR ^= GPIO_ODR_OD7;
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

void Config_GPIO_USART(int baudRate)
{
		GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);//// Настраиваем пины PA2 (TX) и PA3 (RX)

		GPIOA->MODER |= (GPIO_MODER_MODER2_1) | (GPIO_MODER_MODER3_1); // Установим альтернативный режим
		GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_0|GPIO_AFRL_AFSEL2_1|GPIO_AFRL_AFSEL2_2) 
											| (GPIO_AFRL_AFSEL3_0|GPIO_AFRL_AFSEL3_1|GPIO_AFRL_AFSEL3_2); // AF7 для USART2
	
		USART2->BRR = SystemCoreClock/baudRate; // SystemCoreClock/Baudrate 
		USART2->CR1 |= USART_CR1_UE; // Включить USART2
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE; // Включить TX, RX
		USART2->CR1 |= USART_CR1_RXNEIE; // Включить прерывание

		NVIC_EnableIRQ(USART2_IRQn); // Разрешить прерывания для USART2
}

int USART2_GetStatus()//Проверим окончание чтения
{
	if(USART2->SR & USART_SR_RXNE)
	{
		return 1;
	}
		return 0;
}

char USART2_ReadChar()//считываем регистр 
{
	return USART2->DR;
}

void USART2_SetChar(char c)//Установка символа
{
    while (!(USART2->SR & USART_SR_TXE))//Проверим окончание передачи
		{
		}
    USART2->DR = c;
}

void USART2_SetString(char* str)//Установка строки по символьно
{
		int size = strlen(str);
		
		for(int i=0; i<size;i++)
		{
			USART2_SetChar(str[i]);
		}
}

