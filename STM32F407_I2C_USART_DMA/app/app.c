#include "app.h"

uint8_t txBuffer[BUFFER_SIZE] = "Hello I2C";
uint8_t rxBuffer[BUFFER_SIZE];

/////////////////

int main()
{
    Init_LED();
    Init_I2C();
//    Init_SPI();

	    // Передача данных
    I2C_Write(I2C_ADDRESS, txBuffer, sizeof(txBuffer));

    // Прием данных
    I2C_Read(I2C_ADDRESS, rxBuffer, sizeof(rxBuffer));
	
    while(1)
    {
				//I2C_Write(I2C_ADDRESS, txBuffer, sizeof(txBuffer));
				//delay_s(1);
				//SPI2_DMA_TransmitReceive(receivedChar);
        //tst = SPI2_TransmitReceive(0xDF);//test net DMA
    }
		
    return 0;
}