#include "app.h"

uint8_t txBuffer[BUFFER_SIZE] = "Hello I2C";
uint8_t rxBuffer[BUFFER_SIZE];
uint8_t* count_device;
/////////////////

int main()
{
    Init_LED();
    Init_I2C();
//    Init_SPI();

	    // Передача данных
    //I2C_Write(0x30, txBuffer, sizeof(txBuffer));

    // Прием данных
   // I2C_Read(I2C_ADDRESS, rxBuffer, sizeof(rxBuffer));
	int a=0;
    while(1)
    {
				//I2C_Write(0x30, txBuffer, sizeof(txBuffer));
				count_device=I2C_Scan_Bus(128);
//				I2C_Start();
//				I2C1->DR = 0x30 <<1; // Отправка адреса
//				while (!(I2C1->SR1 & I2C_SR1_ADDR)) // Ждем подтверждения
//  			I2C1->SR2; // Сброс флага
//				I2C_Stop();
				
				
				delay_ms(100);
    }
		
    return 0;
}