#include "app.h"


uint8_t rezultRead[SIZE_BUF];

int main()
{
	Init_LED();
	Init_I2C();
	
	while(1)
	{
				while(I2C_Slave_Receive(rezultRead, SIZE_BUF)!= 0)
        {
            Error_Handler();
        }

        if(I2C_Slave_Transmit(rezultRead, SIZE_BUF)!= 0)
        {
            Error_Handler();
        }
				delay_us(1000);

	}
	return 0;
}


