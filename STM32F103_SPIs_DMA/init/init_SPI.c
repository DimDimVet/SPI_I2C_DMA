#include "init_SPI.h"

void Init_SPI()//main init spi
{
		Enable_RCC_SPI();
		Config_GPIO_SPI();
		Config_SPI();
}

void Enable_RCC_SPI()
{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//тактирование GPIOA
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;//тактирование SPI1
}

void Config_GPIO_SPI()
{

    // Настраиваем PA4 (nSS) как вход
    GPIOA->CRL &= ~GPIO_CRL_CNF4; // сбрасываем настройки
    GPIOA->CRL |= GPIO_CRL_MODE4_0; // режим входа с подтяжкой

//    // Настраиваем PA5 (SCK), PA6 (MISO), PA7 (MOSI) как альтернативные функции
//    GPIOA->CRL &= ~((GPIO_CRL_CNF5 | GPIO_CRL_MODE5) |
//                    (GPIO_CRL_CNF6 | GPIO_CRL_MODE6) |
//                    (GPIO_CRL_CNF7 | GPIO_CRL_MODE7));
//    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5_0 |  
//                   GPIO_CRL_CNF6_0 | GPIO_CRL_MODE6_0 |  
//                   GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7_0);



		GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);//reset
    GPIOA->CRL |= GPIO_CRL_MODE5_0; // PA5 2MHz
		GPIOA->CRL |= GPIO_CRL_CNF5_0; // PA5 alt SCK
		
		GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);//reset
		GPIOA->CRL |= GPIO_CRL_MODE7_0; // PA7 2MHz
		GPIOA->CRL |= GPIO_CRL_CNF7_1; // PA7 alt MOSI
                  
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
		GPIOA->CRL |= GPIO_CRL_MODE6_0; // PA7 2MHz
    GPIOA->CRL |= (GPIO_CRL_CNF6_1); // PA6 input MISO
}

void Config_SPI()
{
    SPI1->CR1 = 0;//reset
	
		SPI1->CR1 &= ~SPI_CR1_MSTR;// master1
		SPI1->CR1 &= ~SPI_CR1_BIDIMODE;//включение режима двунаправленных данных mode:master
		SPI1->CR1 &= ~SPI_CR1_BIDIOE;//включение вывода в двунаправленном режиме
		SPI1->CR1 &= ~SPI_CR1_CRCEN;//аппаратный расчет CRC включен
		SPI1->CR1 &= ~SPI_CR1_CRCNEXT;//следующая передача CRC
		SPI1->CR1 |= SPI_CR1_DFF;//16-битный формат кадра данных master1
		SPI1->CR1 &= ~SPI_CR1_RXONLY;//Только прием mode:slave
		SPI1->CR1 &= ~SPI_CR1_SSM;// Программное управление mode:master1
		SPI1->CR1 &= ~SPI_CR1_SSI;// Внутренний раб выбор mode:master1
		SPI1->CR1 &= ~SPI_CR1_LSBFIRST;//Формат кадра LSB
		SPI1->CR1 &= ~SPI_CR1_BR_0;// f/4
		SPI1->CR1 |= SPI_CR1_BR_1;// f/4
		SPI1->CR1 &= ~SPI_CR1_BR_2;// f/4
		SPI1->CR1 |= SPI_CR1_CPOL;// начальный фронт
		SPI1->CR1 |= SPI_CR1_CPHA;// фаза...

		//SPI1->CR2 |=SPI_CR2_TXDMAEN;//переключили дма на spi - передача, DMAT = Tx
		//SPI1->CR2 |=SPI_CR2_RXDMAEN;//переключили дма на spi - чтение, DMAR = Rx
		
		SPI1->CR1 |= SPI_CR1_SPE;//Вкл SPI
		
		
		//
//		    SPI1->CR1 = 0;//reset
//	
//		SPI1->CR1 |= SPI_CR1_MSTR;// master1
//		SPI1->CR1 &= ~SPI_CR1_BIDIMODE;//включение режима двунаправленных данных mode:master
//		SPI1->CR1 &= ~SPI_CR1_BIDIOE;//включение вывода в двунаправленном режиме
//		SPI1->CR1 &= ~SPI_CR1_CRCEN;//аппаратный расчет CRC включен
//		SPI1->CR1 &= ~SPI_CR1_CRCNEXT;//следующая передача CRC
//		SPI1->CR1 |= SPI_CR1_DFF;//16-битный формат кадра данных master1
//		SPI1->CR1 &= ~SPI_CR1_RXONLY;//Только прием mode:slave
//		SPI1->CR1 |= SPI_CR1_SSM;// Программное управление mode:master1
//		SPI1->CR1 |= SPI_CR1_SSI;// Внутренний раб выбор mode:master1
//		SPI1->CR1 &= ~SPI_CR1_LSBFIRST;//Формат кадра LSB
//		SPI1->CR1 |= SPI_CR1_BR_0;// f/4
//		SPI1->CR1 |= SPI_CR1_BR_1;// f/4
//		SPI1->CR1 &= ~SPI_CR1_BR_2;// f/4
//		SPI1->CR1 |= SPI_CR1_CPOL;// начальный фронт
//		SPI1->CR1 |= SPI_CR1_CPHA;// фаза...

//		//SPI1->CR2 |=SPI_CR2_TXDMAEN;//переключили дма на spi - передача, DMAT = Tx
//		//SPI1->CR2 |=SPI_CR2_RXDMAEN;//переключили дма на spi - чтение, DMAR = Rx
//		
////		MODIFY_REG(SPI1->CR1, SPI_CR1_BIDIMODE| SPI_CR1_BIDIOE|SPI_CR1_CRCEN|SPI_CR1_CRCNEXT|
////												SPI_CR1_RXONLY|SPI_CR1_LSBFIRST|SPI_CR1_BR_2,
////												SPI_CR1_SSM|SPI_CR1_SSI|SPI_CR1_BR_1|SPI_CR1_DFF|SPI_CR1_BR_0|
////												SPI_CR1_MSTR|SPI_CR1_CPOL|SPI_CR1_CPHA);
//												
//    SPI1->CR1 |= SPI_CR1_SPE;//Вкл SPI
}

uint32_t SPI_TransmitReceive(uint32_t data)
{
	uint32_t data1;
	 // Ждем, пока не будет готово устройство для передачи
                // Отправляем данные

    // Ждем, пока данные не будут приняты
    //while (!(SPI1->SR & SPI_SR_RXNE)); // Ждём, пока RXNE станет 1
		if(SPI1->SR & SPI_SR_RXNE)
		{
					data1=SPI1->DR;
			    while (!(SPI1->SR & SPI_SR_TXE)); // Ждём, пока TXE станет 1
					SPI1->DR = data1;  
			return SPI1->DR;
		}
		else
		{
			return 0;
		}
    
}
