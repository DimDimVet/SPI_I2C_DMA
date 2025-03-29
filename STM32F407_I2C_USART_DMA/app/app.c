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
   // I2C_Write(I2C_ADDRESS, txBuffer, sizeof(txBuffer));

    // Прием данных
   // I2C_Read(I2C_ADDRESS, rxBuffer, sizeof(rxBuffer));
	int a=0;
    while(1)
    {
				count_device=I2C_Scan_Bus(128);
				delay_s(1);
    }
		
    return 0;
}