#include "app.h"
#define SIZE_BUF_USART 10

uint8_t rezultRead[SIZE_BUF_USART];

int main()
{
	Init_LED();
	Init_I2C();
	
	while(1)
	{
				while(I2C_Slave_Receive(rezultRead, 2)!= 0)
        {
            Error_Handler();
        }
					
        if(I2C_Slave_Transmit(rezultRead, 2)!= 0)
        {
            Error_Handler();
        }
				//delay_ms(10);
	
//        while(I2C_Slave_Receive(receivedData, 2)!= 0)
//        {
//            Error_Handler();
//        }
//					
//        if(I2C_Slave_Transmit(dataToSend, 2)!= 0)
//        {
//            Error_Handler();
//        }
				
	}
	return 0;
}


