#include "init_I2C.h"

uint32_t dataBufRx[1];

void Init_I2C()
{
    Enable_RCC_I2C();
    Config_GPIO_I2C();
    Config_I2C();
    Config_I2C_DMA1();
}

void Enable_RCC_I2C()
{
    RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOBEN_Pos; // Включаем тактирование порта B
    RCC->APB1ENR |= 1 << RCC_APB1ENR_I2C1EN_Pos; // Включаем тактирование 
    RCC->AHB1ENR |= 1 << RCC_AHB1ENR_DMA1EN_Pos; // Включаем тактирование DMA1

}

void Config_GPIO_I2C()
{
////    //PB6 (SCL), PB7 (SDA)

    GPIOB->MODER |= 0 << GPIO_MODER_MODE6_Pos; // Очистка режима для PB6
    GPIOB->MODER |= 2 << GPIO_MODER_MODE6_Pos;   // Альтернативная функция для PB6 (SCL)
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT6_Pos;//открытый коллектор
		GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED6_Pos;//скорость

    GPIOB->MODER |= 0 << GPIO_MODER_MODE7_Pos;  // Очистка режима для PB7
    GPIOB->MODER |= 2 << GPIO_MODER_MODE7_Pos;   // Альтернативная функция для PB7 (SDA)
		GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT7_Pos;//открытый коллектор
		GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED7_Pos;//скорость
	
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL6_Pos;// AF4 для I2C PB6 (SCL)
    GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL7_Pos;// AF4 для I2C PB7 (SDA)
	
}

uint32_t I2C_SPEED(uint32_t pclk, uint32_t speed, uint32_t dutyCycle)
{
    uint32_t timing = 0;

    if (dutyCycle == I2C_CCR_DUTY)
    {
        // Формула для коэффициента заполнения 16/9
        timing = (pclk / (speed * 9)) - 1;
    }
    else
    {
        // Формула для коэффициента заполнения 1/2
        timing = (pclk / (speed * 2)) - 1;
    }

    return timing;
}


void Config_I2C()
{
	 uint32_t freqrange;
   uint32_t pclk1;
	 
	/*Reset I2C*/
  I2C1->CR1 |= I2C_CR1_SWRST;
  I2C1->CR1 &= ~I2C_CR1_SWRST;
	
	  /* Get PCLK1 frequency */
  //pclk1 = HAL_RCC_GetPCLK1Freq();
	pclk1 =0x00F42400;
  freqrange = pclk1/1000000;
	
  /* Configure I2Cx: Frequency range */
	I2C1->CR2 |= freqrange << I2C_CR2_FREQ_Pos;
	
  /* Configure I2Cx: Rise Time */
	I2C1->TRISE |= I2C_RISE_TIME(freqrange, CLOCK_SPEED) << I2C_TRISE_TRISE_Pos;

  /* Configure I2Cx: Speed */
 	I2C1->CCR |= I2C_SPEED(pclk1, CLOCK_SPEED, 0);// << (I2C_CCR_FS_Pos | I2C_CCR_DUTY_Pos | I2C_CCR_CCR_Pos);

  /* Configure I2Cx: Generalcall and NoStretch mode */
	I2C1->CR1 |= (0 | 0) << (I2C_CR1_ENGC_Pos | I2C_CR1_NOSTRETCH_Pos);

  /* Configure I2Cx: Own Address1 and addressing mode */
	I2C1->OAR1 |= (I2C_ADDRESSINGMODE_7BIT | 0) << (I2C_OAR1_ADDMODE_Pos|I2C_OAR1_ADD0_Pos);
	
  /* Configure I2Cx: Dual mode and Own Address2 */
	I2C1->OAR2 |= (0 | 0) << (I2C_OAR2_ENDUAL_Pos | I2C_OAR2_ADD2_Pos);
	
	//I2C1->CR2 |=1 << I2C_CR2_LAST_Pos;
	
	I2C1->CR2 |=1 << I2C_CR2_DMAEN_Pos;
	
	//I2C1->CR1 |= i2c_;
	
  /* Enable the selected I2C peripheral */
	I2C1->CR1 |= 1<<I2C_CR1_PE_Pos;
	

}

void Config_I2C_DMA1()
{
    //Stream 6-Channel 1 I2C1_TX, Stream 5 Channel 1 I2C1_RX = 000: channel 0 selected
    DMA1_Stream6->CR = 0;
    DMA1_Stream6->CR |= 1 << DMA_SxCR_CHSEL_Pos;  // tream 6-Channel 1 I2C1_TX
    DMA1_Stream6->CR |= 0 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
    DMA1_Stream6->CR |= 0 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
    DMA1_Stream6->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
    DMA1_Stream6->CR |= 0 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
    DMA1_Stream6->CR |= 0 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти1
    DMA1_Stream6->CR |= 0 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
    DMA1_Stream6->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
    DMA1_Stream6->CR |= 0 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
    DMA1_Stream6->CR |= 0 << DMA_SxCR_CIRC_Pos;//кольцевой режим
    DMA1_Stream6->CR |= 1 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
    DMA1_Stream6->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
    DMA1_Stream6->PAR = (uint32_t)(&I2C1->DR);// Адрес регистра данных I2C1
    DMA1_Stream6->NDTR = 0;//размер массива
    DMA1_Stream6->M0AR = 0;// Адрес буфера
    DMA1_Stream6->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока

    DMA1_Stream5->CR = 0;
    DMA1_Stream5->CR |= 1 << DMA_SxCR_CHSEL_Pos;  // Stream 5 Channel 1 I2C1_RX
    DMA1_Stream5->CR |= 0 << DMA_SxCR_MBURST_Pos;//Конфигурация передачи пакета памяти
    DMA1_Stream5->CR |= 0 << DMA_SxCR_PBURST_Pos;//Конфигурация периферийной пакетной передачи
    DMA1_Stream5->CR |= 0 << DMA_SxCR_PL_Pos;//уровень приоритета
    DMA1_Stream5->CR |= 0 << DMA_SxCR_PINCOS_Pos;//размер смещения периферийного приращения связан с PSIZE
    DMA1_Stream5->CR |= 2 << DMA_SxCR_MSIZE_Pos;//Размер данных памяти
    DMA1_Stream5->CR |= 2 << DMA_SxCR_PSIZE_Pos;//Размер периферийных данных
    DMA1_Stream5->CR |= 1 << DMA_SxCR_MINC_Pos;//Режим приращения памяти
    DMA1_Stream5->CR |= 1 << DMA_SxCR_PINC_Pos;//Режим приращения периферийных устройств
    DMA1_Stream5->CR |= 1 << DMA_SxCR_CIRC_Pos;//кольцевой режим
    DMA1_Stream5->CR |= 0 << DMA_SxCR_DIR_Pos;//направление передачи данных 00: периферийное устройство-память 01: память-периферийное устройство
    DMA1_Stream5->CR |= 1 << DMA_SxCR_TCIE_Pos;//Разрешение прерывания завершения передачи
    DMA1_Stream5->PAR = (uint32_t)(&I2C1->DR);// Адрес регистра данных I2C1
    DMA1_Stream5->NDTR = 1;//размер массива
    DMA1_Stream5->M0AR = (uint32_t)dataBufRx;// Адрес буфера
    DMA1_Stream5->CR |= 1 << DMA_SxCR_EN_Pos;//включение потока

    DMA1->HIFCR |= 1 << DMA_HIFCR_CHTIF6_Pos;
    DMA1->HIFCR |= 1 << DMA_HIFCR_CTCIF5_Pos;
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);// Включение прерываний DMA
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);// Включение прерываний DMA
}

///////////////////////

uint32_t* Read_I2C_DMA()
{
    return dataBufRx;
}

void I2C_DMA_TransmitReceive(char *str_data)
{
		uint8_t sizeTx = strlen(str_data);

    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    DMA1_Stream6->NDTR = sizeTx;		
    DMA1_Stream6->M0AR = (uint32_t)str_data; // Указание адреса буфера передачи
    DMA1_Stream6->CR |= DMA_SxCR_EN;     // Включаем DMA
}

