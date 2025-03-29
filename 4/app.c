#include "stm32f4xx.h"
#include "stdio.h"

char data[50];
void i2c_init(void){
RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN; //enable gpiob clock
RCC->APB1ENR|=RCC_APB1ENR_I2C1EN; //enable i2c1 clock
//GPIOB->MODER|=0xA0000; //set pb8and9 to alternative function
//GPIOB->AFR[1]|=0x44;
//GPIOB->OTYPER|=GPIO_OTYPER_OT8|GPIO_OTYPER_OT9; //set pb8 and pb9 as open drain
	
    // Включаем тактирование порта B
    
    GPIOB->MODER |= 0 << GPIO_MODER_MODE6_Pos; // Очистка режима для PB6
    GPIOB->MODER |= 2 << GPIO_MODER_MODE6_Pos;   // Альтернативная функция для PB6 (SCL)
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT6_Pos;//открытый коллектор
		GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED6_Pos;//скорость
		GPIOB->PUPDR |= 1 << GPIO_PUPDR_PUPD6_Pos;//подтянем+

    GPIOB->MODER |= 0 << GPIO_MODER_MODE7_Pos;  // Очистка режима для PB7
    GPIOB->MODER |= 2 << GPIO_MODER_MODE7_Pos;   // Альтернативная функция для PB7 (SDA)
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT7_Pos;//открытый коллектор
		GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED7_Pos;//скорость
		GPIOB->PUPDR |= 1 << GPIO_PUPDR_PUPD7_Pos;//подтянем+
		
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL6_Pos;// AF4 для I2C PB6 (SCL)
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL7_Pos;// AF4 для I2C PB7 (SDA)
	
I2C1->CR1=I2C_CR1_SWRST;
I2C1->CR1&=~I2C_CR1_SWRST;	
		
		I2C1->CR2 |= 16 << I2C_CR2_FREQ_Pos;//установка частоты
		I2C1->CCR |= 80 << I2C_CCR_CCR_Pos;// Настройка условного задержки
		I2C1->TRISE |= 17 << I2C_TRISE_TRISE_Pos;// Максимальное время подъема
		
		I2C1->CR1 |= 1 << I2C_CR1_PE_Pos;
}

char i2c_readByte(char saddr,char maddr, char *data)
{

volatile int tmp;
while(I2C1->SR2&I2C_SR2_BUSY){;}
I2C1->CR1|=I2C_CR1_START;
while(!(I2C1->SR1&I2C_SR1_SB)){;}
	
I2C1->DR=saddr<<1;
while(!(I2C1->SR1&I2C_SR1_ADDR)){;}
	
tmp=I2C1->SR2;
while(!(I2C1->SR1&I2C_SR1_TXE)){;}
	
I2C1->DR=maddr;
while(!(I2C1->SR1&I2C_SR1_TXE)){;}
	
I2C1->CR1|=I2C_CR1_START;
while(!(I2C1->SR1&I2C_SR1_SB)){;}	
	
I2C1->DR=saddr<<1|1;
while(!(I2C1->SR1&I2C_SR1_ADDR)){;}
	
I2C1->CR1&=~I2C_CR1_ACK;
tmp =I2C1->SR2;
I2C1->CR1|=I2C_CR1_STOP;
	
while(!(I2C1->SR1&I2C_SR1_RXNE)){;}
	
*data++=I2C1->DR;
return 0;
}

void i2c_writeByte(char saddr,char maddr,char data){

while (I2C1->SR2 & 2);                               //wait until bus not busy
I2C1->CR1 |= 0x100;                                 //generate start
while (!(I2C1->SR1 & 1)){;}
	
	volatile int Temp;
I2C1->DR = saddr<< 1;                 	 // Send slave address
while (!(I2C1->SR1 & 2)){;}                            //wait until address flag is set
Temp = I2C1->SR2; 
	
while (!(I2C1->SR1 & 0x80));                          //Wait until Data register empty
I2C1->DR = maddr;                      // send memory address
while (!(I2C1->SR1 & 0x80));                         //wait until data register empty
I2C1->DR = data; 	
while (!(I2C1->SR1 & 4));                             //wait until transfer finished
I2C1->CR1 |= 0x200;
}

void i2c_WriteMulti(char saddr,char maddr,char *buffer, uint8_t length){

	
	while (I2C1->SR2 & 2);                               //wait until bus not busy
I2C1->CR1 |= 0x100;                                 //generate start
while (!(I2C1->SR1 & 1)){;}
	
	volatile int Temp;
I2C1->DR = saddr<< 1;                 	 // Send slave address
while (!(I2C1->SR1 & 2)){;}                            //wait until address flag is set
Temp = I2C1->SR2; 
	
while (!(I2C1->SR1 & 0x80));                          //Wait until Data register empty
I2C1->DR = maddr;                      // send memory address
while (!(I2C1->SR1 & 0x80));                         //wait until data register empty

for (uint8_t i=0;i<length;i++)
 { 
 I2C1->DR=buffer[i]; //filling buffer with command or data
	while (!(I2C1->SR1 & 4));
 }	
                             //wait until transfer finished
I2C1->CR1 |= 0x200;

}


void i2c_ReadMulti(char saddr,char maddr, int n, char* data)
{
	volatile int temp;
	while (I2C1->SR2 & 2){;}
	I2C1->CR1|=I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB)){;}
	I2C1->DR=saddr<<1;
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){;}
	temp=I2C1->SR2;
	while(!(I2C1->SR1&I2C_SR1_TXE)){;}
	I2C1->DR = maddr;
	while(!(I2C1->SR1&I2C_SR1_TXE)){;}
	I2C1->CR1|=I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB)){;}
	I2C1->DR=saddr<<1|1;
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){;}
	temp=I2C1->SR2;
		
	I2C1->CR1|=I2C_CR1_ACK;
	while(n>0U)
		{
		if(n==1U)
				{
				I2C1->CR1&=~I2C_CR1_ACK;
					I2C1->CR1|=I2C_CR1_STOP;
					while(!(I2C1->SR1&I2C_SR1_RXNE)){;}
					*data++=I2C1->DR;
						break;
				}
			else
					{
					
					while(!(I2C1->SR1&I2C_SR1_RXNE)){;}
						(*data++)=I2C1->DR;
							n--;
					
					}	
				
			
		}	
		
}

void delay(int ms)
	{
	SysTick->LOAD=16-1;
	SysTick->VAL=0;
	SysTick->CTRL=0x5;
		for (int i=0;i<ms;i++)
		{
			while(!(SysTick->CTRL &0x10000)){}
		}
	SysTick->CTRL=0;
	
	}
	
void I2C_Start(void) 
{
		I2C1->CR1 |= 1 << I2C_CR1_ACK_Pos;
    I2C1->CR1 |= 1 << I2C_CR1_START_Pos; // Генерация стартового состояния
    while(!(I2C1->SR1 & I2C_SR1_SB)); // Ожидание завершения
}

void I2C_Stop(void) 
{
    I2C1->CR1 |=1 << I2C_CR1_STOP_Pos; // Генерация стоп-состояния
    while (I2C1->CR1 & I2C_SR1_STOPF); // Ожидание завершения
}

void i2c_bus_scan(void)
{     
				 int a=0; 
         for (uint8_t i=0;i<128;i++)
   {
//            I2C1->CR1 |= I2C_CR1_START;
//            while(!(I2C1->SR1 & I2C_SR1_SB));
						I2C_Start();
		 
            I2C1->DR=(i<<1|0); 
            while(!(I2C1->SR1)|!(I2C1->SR2)){};
							
            //I2C1->CR1 |= I2C_CR1_STOP; 
							I2C_Stop();
							
            delay(100);//minium wait time is 40 uS, but for sure, leave it 100 uS
            a=(I2C1->SR1&I2C_SR1_ADDR);
            if (a==2)
         {
					 
					 //sprintf(data,"Found I2C device at adress 0x%X (hexadecimal), or %d (decimal)\r\n",i,i);
                //UART_Write_String(data);
         }
     }
}

int main()
	{
		//USART2_Init();
		i2c_init();
		
	
	while(1)
		{
		i2c_bus_scan();
			//i2c_writeByte(0x3C,0xFF,100);
			for(int i=0;i<1000000;i++);
		}

	}