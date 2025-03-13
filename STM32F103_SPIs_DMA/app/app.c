#include "app.h"

uint32_t readData; 
int main()
{
	Init_LED();
	Init_SPI();
	
	while(1)
	{
		LED13();
		readData=SPI_Receive();
		delay_s(1);

	}
	return 0;
}