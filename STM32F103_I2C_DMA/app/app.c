#include "app.h"

uint8_t* count_device;

void I2C1_EV_IRQHandler(void)
{
    if (I2C1->SR1 & I2C_SR1_ADDR)
		{ // Проверка адреса
        (void)I2C1->SR2; // Сброс флага
    }

    if (I2C1->SR1 & I2C_SR1_RXNE) 
		{ // Если данные приняты
        uint8_t received_data = I2C1->DR; // Чтение данных
        // Обработка received_data
    }

    if (I2C1->SR1 & I2C_SR1_TXE) 
		{ // Если готовы отправить данные
        //I2C1->DR = /* данные для отправки */
    }
}

void I2C_Slave_Receive() 
{
uint8_t data;
    //while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Ожидание адреса
    //(void)I2C1->SR2; // Сброс флага адреса
		if (I2C1->SR1 & I2C_SR1_ADDR)
		{ // Проверка адреса
        (void)I2C1->SR2; // Сброс флага
    }

    //while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Ожидание поступления данных
    //uint8_t data = I2C1->DR; // Чтение данных
		if (I2C1->SR1 & I2C_SR1_RXNE) 
		{ // Если данные приняты
        data = I2C1->DR; // Чтение данных
        // Обработка received_data
    }
		
    // Обработка полученных данных (например, сохранение в буфер)
		if (I2C1->SR1 & I2C_SR1_TXE) 
		{ // Если готовы отправить данные
        I2C1->DR = 0xAF;
    }
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
						I2C1->CR1 |= I2C_CR1_ACK;
						I2C1->CR1 |= I2C_CR1_START; // Генерация стартового состояния
						while(!(I2C1->SR1 & I2C_SR1_SB)); // Ожидание завершения
			
				I2C1->DR=(i<<1|0); 
        while(!(I2C1->SR1)|!(I2C1->SR2))
				{};
					
				    I2C1->CR1 |=I2C_CR1_STOP; // Генерация стоп-состояния
						while (I2C1->CR1 & I2C_SR1_STOPF); // Ожидание завершения
						
				*count_device++ =(I2C1->SR1&I2C_SR1_ADDR);
		};
		
		return count_device;
}

int main()
{
	Init_LED();
	Init_I2C();
	NVIC_EnableIRQ(I2C1_EV_IRQn);
	
	while(1)
	{
			//count_device=I2C_Scan_Bus(128);
			I2C_Slave_Receive(); // Получение данныхI2C_Slave_Receive(); // Получение данных
			delay_ms(100);
	}
	return 0;
}
