#include "app.h"



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

int main()
{
	Init_LED();
	Init_I2C();
	NVIC_EnableIRQ(I2C1_EV_IRQn);
	
	while(1)
	{
//		if (I2C1->SR1 & I2C_SR1_ADDR)
//		{ // Проверка адреса
//        (void)I2C1->SR2; // Сброс флага
//    }
			
		//tst = SPI_TransmitReceive(0);	
	}
	return 0;
}
