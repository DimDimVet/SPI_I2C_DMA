
//#include "stm32f10x.h"  // Подключите заголовочный файл для STM32F103


//void GPIO_Config(void) {
//    // Включаем тактирование GPIOA
//    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

//    // Настраиваем PA5 (SCK), PA7 (MOSI) как альтернативные функции
//    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_MODE7 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
//    GPIOA->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_CNF5_1) | // PA5: альтернативная функция, max 2MHz
//                  (GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7_1);  // PA7: альтернативная функция, max 2MHz
//                  
//    // Настраиваем PA6 (MISO) как вход
//    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
//    GPIOA->CRL |= (GPIO_CRL_CNF6_0); // PA6: флотирующий вход
//}

//void SPI_Config(void) {
//    // Включаем тактирование SPI1
//    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

//    // Сбрасываем настройки SPI
//    SPI1->CR1 = 0;

//    // Настройка параметров SPI: Мастер, режим CPOL, CPHA, делитель частоты
//    SPI1->CR1 |= SPI_CR1_MSTR       // Режим Мастера
//                | SPI_CR1_SSM        // Включение режима программного управления
//                | SPI_CR1_SSI        // Подтверждение
//                | SPI_CR1_BR_1       // Настройка делителя частоты (Fpclk / 4)
//                | SPI_CR1_CPOL       // Полярность (настраиваем в зависимости от устройства)
//                | SPI_CR1_CPHA;      // Фаза (настраиваем в зависимости от устройства)

//    // Включаем SPI
//    SPI1->CR1 |= SPI_CR1_SPE;
//}

//uint8_t SPI_TransmitReceive(uint8_t data)
// {
//    // Ждем, пока не будет готово устройство для передачи
//    while (!(SPI1->SR & SPI_SR_TXE))
//		{
//		
//		}; // Ждём, пока TXE станет 1
//    SPI1->DR = data;                  // Отправляем данные

//    // Ждем, пока данные не будут приняты
//    while (!(SPI1->SR & SPI_SR_RXNE)); // Ждём, пока RXNE станет 1
//    return SPI1->DR;                  // Возвращаем принятые данные
//}

//int main(void) {
//    GPIO_Config();    // Настраиваем GPIO
//    SPI_Config();     // Настраиваем SPI1

//    uint8_t sendData = 0xA0; // Данные для отправки
//    uint8_t receivedData = 0;
//		uint8_t receivedData2 = 0;

//    while (1)
//		{
////				for(int i=0; i<10000;i++)
////				{
////				};
//        receivedData = SPI_TransmitReceive(sendData); // Передаем и получаем данные
//        receivedData2=receivedData;
//				// Здесь вы можете использовать receivedData, например, передать его куда-то или вывести на экран
//    }

//    return 0;
//}


#include "stm32f10x.h"  // Подключите заголовочный файл для STM32F103
uint8_t data[10];

void SPIInit(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //Включаем тактирование SPI1
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //включаем тактирование порта GPIOA
  RCC->AHBENR |= RCC_AHBENR_DMA1EN; //Включаем тактирование DMA1
  
  //Настройка GPIO
  
  //PA7 - MOSI
  //PA6 - MISO
  //PA5 - SCK
  //Для начала сбрасываем все конфигурационные биты в нули
  GPIOA->CRL &= ~(GPIO_CRL_CNF5_Msk | GPIO_CRL_MODE5_Msk 
                | GPIO_CRL_CNF6_Msk | GPIO_CRL_MODE6_Msk
                | GPIO_CRL_CNF7_Msk | GPIO_CRL_MODE7_Msk);
  
  //Настраиваем
  //SCK: MODE5 = 0x03 (11b); CNF5 = 0x02 (10b)
  GPIOA->CRL |= (0x02<<GPIO_CRL_CNF5_Pos) | (0x03<<GPIO_CRL_MODE5_Pos);
  
  //MISO: MODE6 = 0x00 (00b); CNF6 = 0x01 (01b)
  GPIOA->CRL |= (0x01<<GPIO_CRL_CNF6_Pos) | (0x00<<GPIO_CRL_MODE6_Pos);
  
  //MOSI: MODE7 = 0x03 (11b); CNF7 = 0x02 (10b)
  GPIOA->CRL |= (0x02<<GPIO_CRL_CNF7_Pos) | (0x03<<GPIO_CRL_MODE7_Pos);
  
  
  //Настройка SPI
  SPI1->CR1 = 0<<SPI_CR1_DFF_Pos  //Размер кадра 8 бит
    | 0<<SPI_CR1_LSBFIRST_Pos     //MSB first
    | 1<<SPI_CR1_SSM_Pos          //Программное управление SS
    | 1<<SPI_CR1_SSI_Pos          //SS в высоком состоянии
    | 0x04<<SPI_CR1_BR_Pos        //Скорость передачи: F_PCLK/32
    | 1<<SPI_CR1_MSTR_Pos         //Режим Master (ведущий)
    | 0<<SPI_CR1_CPOL_Pos | 0<<SPI_CR1_CPHA_Pos; //Режим работы SPI: 0
  
  
  SPI1->CR2 |= 1<<SPI_CR2_TXDMAEN_Pos;
  SPI1->CR2 |= 1<<SPI_CR2_RXDMAEN_Pos;
  SPI1->CR1 |= 1<<SPI_CR1_SPE_Pos; //Включаем SPI
}

void SPI_Receive(uint8_t *data, uint16_t len)
{
  static uint8_t _filler = 0xFF;
  
  //отключаем канал DMA после предыдущей передачи данных
  DMA1_Channel2->CCR &= ~(1 << DMA_CCR1_EN_Pos); 
  
  DMA1_Channel2->CPAR = (uint32_t)(&SPI1->DR); //заносим адрес регистра DR в CPAR
  DMA1_Channel2->CMAR = (uint32_t)data; //заносим адрес данных в регистр CMAR
  DMA1_Channel2->CNDTR = len; //количество передаваемых данных
  
  //Настройка канала DMA
  DMA1_Channel2->CCR = 0 << DMA_CCR_MEM2MEM_Pos //режим MEM2MEM отключен
    | 0x00 << DMA_CCR_PL_Pos //приоритет низкий
    | 0x00 << DMA_CCR_MSIZE_Pos //разрядность данных в памяти 8 бит
    | 0x01 << DMA_CCR_PSIZE_Pos //разрядность регистра данных 16 бит 
    | 1 << DMA_CCR_MINC_Pos //Включить инкремент адреса памяти
    | 0 << DMA_CCR_PINC_Pos //Инкремент адреса периферии отключен
    | 0 << DMA_CCR_CIRC_Pos //кольцевой режим отключен
    | 0 << DMA_CCR_DIR_Pos;  //0 - из периферии в память
  
  DMA1_Channel2->CCR |= 1 << DMA_CCR_EN_Pos; //включаем прием данных
  
  
  //////////////////////////////////////////////////////////////////////////////
  
  //отключаем канал DMA после предыдущей передачи данных
  DMA1_Channel3->CCR &= ~(1 << DMA_CCR_EN_Pos); 
  
  DMA1_Channel3->CPAR = (uint32_t)(&SPI1->DR); //заносим адрес регистра DR в CPAR
  DMA1_Channel3->CMAR = (uint32_t)(&_filler); //заносим адрес данных в регистр CMAR
  DMA1_Channel3->CNDTR = len; //количество передаваемых данных
  
  //Настройка канала DMA
  DMA1_Channel3->CCR = 0 << DMA_CCR_MEM2MEM_Pos //режим MEM2MEM отключен
    | 0x00 << DMA_CCR_PL_Pos //приоритет низкий
    | 0x00 << DMA_CCR_MSIZE_Pos //разрядность данных в памяти 8 бит
    | 0x01 << DMA_CCR_PSIZE_Pos //разрядность регистра данных 16 бит 
    | 0 << DMA_CCR_MINC_Pos //Инкремент адреса памяти отключен
    | 0 << DMA_CCR_PINC_Pos //Инкремент адреса периферии отключен
    | 0 << DMA_CCR_CIRC_Pos //кольцевой режим отключен
    | 1 << DMA_CCR_DIR_Pos;  //1 - из памяти в периферию
  
  DMA1_Channel3->CCR |= 1 << DMA_CCR_EN_Pos; //Запускаем процесс
}

void main()
{
  SPIInit();
  SPI_Receive(data, sizeof(data));
  
  
  for(;;)
  {
  }
}
