#include "app.h"
uint8_t tst;

int main()
{
	Init_LED();
	Init_SPI();
	int i;
	while(1)
	{
		i++;
		delay_s(1);
		LED6();
		LED7();
		SPI_Transmit(i);
		//delay_s(1);
		tst = SPI_Receive();
		LED6();
		LED7();
	}
	return 0;
}

