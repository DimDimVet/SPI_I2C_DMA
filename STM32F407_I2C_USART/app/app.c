#include "app.h"

int main()
{
    Init_LED();
    Init_I2C();

    while(1)
    {
        while (I2C_Master_TransmitT(I2C_ADDRESS, dataToSend, 2) != 0)
        {
            Error_Handler();
        }

        while(I2C_Master_ReceiveT(I2C_ADDRESS, receivedData, 2) != 0)
        {
            Error_Handler();
        }

        delay_ms(100);
    }

    return 0;
}