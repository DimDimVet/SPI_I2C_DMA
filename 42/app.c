#include "stm32f4xx.h"
#include "delay.h"

#define BUFFER_SIZE 32

uint8_t* count_device;
uint8_t txBuffer[BUFFER_SIZE] = "Hello I2C";
uint8_t rxBuffer[BUFFER_SIZE];


void Enable_RCC_I2C()
{
    RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOBEN_Pos; // Включаем тактирование порта B
    RCC->APB1ENR |= 1 << RCC_APB1ENR_I2C1EN_Pos; // Включаем тактирование 
		RCC->APB1ENR |= 1 << RCC_APB1ENR_I2C2EN_Pos; // Включаем тактирование
    //RCC->AHB1ENR |= 1 << RCC_AHB1ENR_DMA1EN_Pos; // Включаем тактирование DMA1

}

void Config_GPIO_I2C()
{
////    //PB6 (SCL), PB7 (SDA)


	
		GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
    GPIOB->MODER |= (2 << GPIO_MODER_MODER6_Pos | 2 << GPIO_MODER_MODER7_Pos); // Альтернативная функция
	
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL6_Pos;// AF4 для I2C PB6 (SCL)
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL7_Pos;// AF4 для I2C PB7 (SDA)
		
		///////////////
		
		GPIOB->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11);
    GPIOB->MODER |= (2 << GPIO_MODER_MODER10_Pos | 2 << GPIO_MODER_MODER11_Pos); // Альтернативная функция
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT10_Pos;//открытый коллектор
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT11_Pos;//открытый коллектор
		GPIOB->PUPDR |= 0 << GPIO_PUPDR_PUPD10_Pos;
		GPIOB->PUPDR |= 0 << GPIO_PUPDR_PUPD11_Pos;
		
    GPIOB->AFR[1] |= 4 << GPIO_AFRH_AFSEL10_Pos;// AF4 для I2C PB10 (SCL)
    GPIOB->AFR[1] |= 4 << GPIO_AFRH_AFSEL11_Pos;// AF4 для I2C PB11 (SDA)
	
	
	
}

void Config_I2C()
{

	
        I2C1->CR1 &= ~I2C_CR1_PE; // Отключаем I2C
        I2C1->CR2 = 42; // Частота APB1 в MHz
//				I2C1->OAR1 = (0x2A << 1); // Адрес слейва
//				I2C1->CR1 |= I2C_CR1_ACK;
        I2C1->CCR = 210; // Настройка скорости I2C для 100kHz
        I2C1->TRISE = 43; // TRISE
        I2C1->CR1 |= I2C_CR1_PE; // Включаем I2C
				///
				I2C2->CR1 &= ~I2C_CR1_PE; // Отключаем I2C
        I2C2->CR2 = 42; // Частота APB1 в MHz
				I2C2->OAR1 = (0x1A << 1); // Адрес слейва
				I2C2->CR1 |= I2C_CR1_ACK;
        I2C2->CCR = 210; // Настройка скорости I2C для 100kHz
        I2C2->TRISE = 43; // TRISE
        I2C2->CR1 |= I2C_CR1_PE; // Включаем I2C
				

}


///////////////////////
void I2C_Start(void) 
{
		I2C1->CR1 |= 1 << I2C_CR1_ACK_Pos;
    I2C1->CR1 |= 1 << I2C_CR1_START_Pos; // Генерация стартового состояния
    while(!(I2C1->SR1 & I2C_SR1_SB)); // Ожидание завершения
}

void I2C_Stop(void) 
{
    I2C1->CR1 |=1 << I2C_CR1_STOP_Pos; // Генерация стоп-состояния
    while (I2C1->CR1 & I2C_SR1_STOPF); // Ожидание завершения
}

void I2C1_SendAddress(uint8_t addr)
{
    I2C1->DR = addr; // Отправка адреса
		delay_ms(10);
		/////
				if (I2C2->SR1 & I2C_SR1_ADDR)
				{		// Проверка адреса
						(void)I2C2->SR2; // Сброс флага
				}

		    if (I2C2->SR1 & I2C_SR1_RXNE)
		{ // Если данные приняты
        uint8_t received_data = I2C2->DR; // Чтение данных
        // Обработка received_data
    }
		
		if (I2C2->SR1 & I2C_SR1_TXE) 
		{ // Если готовы отправить данные
        I2C2->DR = 0x1A;
		}
		//////////
		
		
		
    while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Ждем подтверждения
    I2C1->SR2; // Сброс флага
}

void I2C2_SendAddress(uint8_t addr)
{
    I2C2->DR = addr; // Отправка адреса
    while (!(I2C2->SR1 & I2C_SR1_ADDR)); // Ждем подтверждения
    I2C2->SR2; // Сброс флага
}

void I2C1_SendData(uint8_t data) 
{
    I2C1->DR = data; // Отправка данных
    while (!(I2C1->SR1 & I2C_SR1_TXE)); // Ждем, пока передатчик готов
}

uint8_t I2C1_ReceiveData() 
{
    while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Ожидание получения байта
    return I2C1->DR; // Чтение байта
}

void I2C_Write(uint8_t address, uint8_t *data, uint8_t size)
{
    I2C_Start();
		
    I2C1_SendAddress(address << 1); // Адрес устройства и бит записи
	
    for (uint8_t i = 0; i < size; i++)
		{
        I2C1_SendData(data[i]);
    }
		
    I2C_Stop();
}

void I2C_Read(uint8_t address, uint8_t *data, uint8_t size)
{
    I2C_Start();
	
    I2C1_SendAddress((address << 1) | 1); // Адрес устройства и бит чтения
	
    for (uint16_t i = 0; i < size - 1; i++)
		{
        data[i] = I2C1_ReceiveData();
    }
		
    I2C_Stop();
}

uint8_t* I2C_Scan_Bus(uint8_t count)
{
		uint8_t* count_device;
		
		if(count <=0)
		{
				count=128;
		};
		
		for (uint8_t i=0; i < count; i++)
		{
				I2C_Start();
			
				I2C1->DR=(i<<1|0); 
        while(!(I2C1->SR1)|!(I2C1->SR2))
				{};
					
				I2C_Stop();
				*count_device++ =(I2C1->SR1&I2C_SR1_ADDR);
		};
		
		return count_device;
}


int main()
{
    Enable_RCC_I2C();
    Config_GPIO_I2C();
    Config_I2C();

	    // Передача данных
    //I2C_Write(0x30, txBuffer, sizeof(txBuffer));

    // Прием данных
   // I2C_Read(I2C_ADDRESS, rxBuffer, sizeof(rxBuffer));
	 
	 I2C_Write(0x1A, txBuffer, sizeof(txBuffer));

    while(1)
    {
				
				//count_device=I2C_Scan_Bus(128);
//				I2C_Start();
//				I2C1->DR = 0x30 <<1; // Отправка адреса
//				while (!(I2C1->SR1 & I2C_SR1_ADDR)) // Ждем подтверждения
//  			I2C1->SR2; // Сброс флага
//				I2C_Stop();
				
				////////////////////
		if (I2C2->SR1 & I2C_SR1_ADDR)
		{ // Проверка адреса
        (void)I2C2->SR2; // Сброс флага
    }

    if (I2C2->SR1 & I2C_SR1_RXNE)
		{ // Если данные приняты
        uint8_t received_data = I2C2->DR; // Чтение данных
        // Обработка received_data
    }
		
		if (I2C2->SR1 & I2C_SR1_ADDR)
		{ // Проверка адреса
        (void)I2C2->SR2; // Сброс флага
    }

    if (I2C2->SR1 & I2C_SR1_RXNE) 
		{ // Если данные приняты
        uint8_t received_data = I2C2->DR; // Чтение данных
        // Обработка received_data
    }
		////////////////////////
				delay_ms(100);
    }
		
    return 0;
}