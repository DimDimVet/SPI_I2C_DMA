#include "app.h"

int main()
{
	Init_LED();
	Init_I2C();
	
	while(1)
	{
        while(I2C_Slave_Receive(receivedData, 2)!= 0)
        {
            Error_Handler();
        }

				//delay_us(100);
					
        if(I2C_Slave_Transmit(dataToSend, 2)!= 0)
        {
            Error_Handler();
        }
				
				delay_ms(100);	
	}
	return 0;
}


