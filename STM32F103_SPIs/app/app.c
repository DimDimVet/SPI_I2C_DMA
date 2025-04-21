#include "app.h"

uint8_t tst;
char data_In_Base[SIZE_REVERC_DATA];
char* data_In=data_In_Base;

void SPI1_IRQHandler(void)
{
//	SPI1_ReadString(receivedStringSPI);
//	//delay_ms(1000);

//	snprintf(rezultStr, SIZESTR, "%s%s",set_infoStr,receivedStringSPI);
//	
//	SPI1_SetString(receivedStringSPI);
}


int main()
{
	Init_LED();
	Init_SPI();

	while(1)
	{
		tst = SPI_TransmitReceive(receivedStringSPI);	
		//SPI1_SetString(receivedStringSPI);
	}
	return 0;
}