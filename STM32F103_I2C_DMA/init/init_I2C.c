#include "init_I2C.h"

char dataBufRxSPI[SIZE_BUF];

void Init_I2C()
{
    Enable_RCC_I2C();
    Config_GPIO_I2C();
    Config_I2C();
    //Config_SPI1_DMA1();
}

void Enable_RCC_I2C(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//тактирование GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;//тактирование
    //RCC->AHBENR |= RCC_AHBENR_DMA1EN;   // Включаем DMA1
}

void Config_GPIO_I2C()
{
// Настраиваем PB6 (SCL), PB7 (SDA) как альтернативные функции

    GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);//reset
    GPIOB->CRL |= GPIO_CRL_MODE6; // PB6
    GPIOB->CRL |= (GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1); // PB6 alt

    GPIOB->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);//reset
    GPIOB->CRL |= GPIO_CRL_MODE7; // PB7
    GPIOB->CRL |= (GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1); // PB7 alt
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
    uint32_t pclk1;
    uint32_t freqrange;

// Настройка I2C (100kHz)

    //pclk1 = HAL_RCC_GetPCLK1Freq();
    pclk1 = 0x00F42400;

    //freqrange = I2C_FREQRANGE(pclk1);
    freqrange = 0x00000010;

    /* Configure I2Cx: Frequency range */
    I2C1->CR2|=I2C_CR2_FREQ_1;

    //MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE, I2C_RISE_TIME(freqrange, ClockSpeed));
    I2C1->TRISE|=I2C_RISE_TIME(freqrange, CLOCK_SPEED);

    //MODIFY_REG(I2C1->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), I2C_SPEED(pclk1, ClockSpeed, I2C_DUTYCYCLE_2));
    I2C1->CCR|= I2C_SPEED(pclk1, CLOCK_SPEED, I2C_DUTYCYCLE_2);
   
    //MODIFY_REG(I2C1->CR1, (I2C_CR1_ENGC | I2C_CR1_NOSTRETCH), (I2C_GENERALCALL_DISABLE | I2C_NOSTRETCH_DISABLE));
    I2C1->CR1|= (0|0);

    //MODIFY_REG(I2C1->OAR1, (I2C_OAR1_ADDMODE | I2C_OAR1_ADD0), (I2C_ADDRESSINGMODE_7BIT | SLAVE_ADDR));
    I2C1->OAR1 =SLAVE_ADDR;
    I2C1->OAR1 &=~I2C_OAR1_ADDMODE;

    //MODIFY_REG(I2C1->OAR2, (I2C_OAR2_ENDUAL | I2C_OAR2_ADD2), (I2C_DUALADDRESS_DISABLE | 0));
    I2C1->OAR2&=~I2C_OAR2_ENDUAL;

    I2C1->CR1 |= I2C_CR1_PE;

    NVIC_EnableIRQ(I2C1_EV_IRQn);
    NVIC_EnableIRQ(I2C1_ER_IRQn);

}

void Config_SPI1_DMA1()
{
//Channel 2 SPI1_RX, Channel 3 SPI1_TX

    DMA1_Channel3->CCR |=0;
    DMA1_Channel3->CCR &= ~DMA_CCR3_MEM2MEM;//режим памяти в память
    DMA1_Channel3->CCR &= ~DMA_CCR3_PL;//уровень приоритета канала
    DMA1_Channel3->CCR &= ~DMA_CCR3_MSIZE_0;//размер памяти
    DMA1_Channel3->CCR &= ~DMA_CCR3_MSIZE_1;
    DMA1_Channel3->CCR &= ~DMA_CCR3_PSIZE_0;//размер периферии
    DMA1_Channel3->CCR &= ~DMA_CCR3_PSIZE_1;
    DMA1_Channel3->CCR |= DMA_CCR3_MINC;//Режим приращения памяти
    DMA1_Channel3->CCR &= ~DMA_CCR3_PINC;//Режим периферийного приращения
    DMA1_Channel3->CCR &= ~DMA_CCR3_CIRC;//Кольцевой режим
    DMA1_Channel3->CCR |= DMA_CCR3_DIR;//Направление передачи данных
    DMA1_Channel3->CCR |= DMA_CCR3_TCIE;//разрешение прерывания по завершению передачи
    DMA1_Channel3->CPAR = (uint32_t)(&SPI1->DR); //Адрес регистра данных spi
    DMA1_Channel3->CNDTR = 0; //размер массива
    DMA1_Channel3->CMAR = 0; //Адрес буфера
    DMA1_Channel3->CCR |= DMA_CCR3_EN; // Включение канала DMA

    DMA1_Channel2->CCR |=0;
    DMA1_Channel2->CCR &= ~DMA_CCR2_MEM2MEM;//режим памяти в память
    DMA1_Channel2->CCR &= ~DMA_CCR2_PL;//уровень приоритета канала
    DMA1_Channel2->CCR |= DMA_CCR2_MSIZE_0;//размер памяти
    DMA1_Channel2->CCR |= DMA_CCR2_MSIZE_1;
    DMA1_Channel2->CCR |= DMA_CCR2_PSIZE_0;//размер периферии
    DMA1_Channel2->CCR |= DMA_CCR2_PSIZE_1;
    DMA1_Channel2->CCR |= DMA_CCR2_MINC;//Режим приращения памяти
    DMA1_Channel2->CCR |= DMA_CCR2_PINC;//Режим периферийного приращения
    DMA1_Channel2->CCR |= DMA_CCR2_CIRC;//Кольцевой режим
    DMA1_Channel2->CCR &= ~DMA_CCR2_DIR;//Направление передачи данных
    DMA1_Channel2->CCR |= DMA_CCR2_TCIE;//разрешение прерывания по завершению передачи
    DMA1_Channel2->CPAR = (uint32_t)(&SPI1->DR); //Адрес регистра данных spi
    DMA1_Channel2->CNDTR = SIZE_BUF; //размер массива
    DMA1_Channel2->CMAR = (uint32_t)dataBufRxSPI; //Адрес буфера
    DMA1_Channel2->CCR |= DMA_CCR2_EN; // Включение канала DMA

    DMA1->IFCR |= DMA_IFCR_CTCIF3;
    DMA1->IFCR |= DMA_IFCR_CTCIF2;

    NVIC_EnableIRQ(DMA1_Channel3_IRQn); // Включение прерываний DMA
    NVIC_EnableIRQ(DMA1_Channel2_IRQn); // Включение прерываний DMA

}

//////////////
uint8_t SPI_TransmitReceive(uint8_t data)
{
    uint8_t data1;

    if(SPI1->SR & SPI_SR_RXNE)
    {
        data1=SPI1->DR;
        while (!(SPI1->SR & SPI_SR_TXE)); // Ждём, пока TXE станет 1
        SPI1->DR = data1;
        return SPI1->DR;
    }
    else
    {
        return SPI1->DR;
    }
}

char* Read_SPI1_DMA1()
{
    return dataBufRxSPI;
}


void SPI1_DMA1_TransmitReceive(char *str_data)
{
    DMA1_Channel3->CCR &= ~DMA_CCR3_EN;
    DMA1_Channel3->CNDTR = SIZE_BUF;
    DMA1_Channel3->CMAR = (uint32_t)str_data; // Указание адреса буфера передачи
    DMA1_Channel3->CCR |= DMA_CCR3_EN;     // Включаем DMA
}
