#include <main.h>

uint32_t Data_is_ready = 0;
uint8_t Received_data = 0;

void Spi_config(void){
  SPI->CR2 |= (1 << 1);
  SPI->CR2 |= (1 << 0);
  
  SPI->ICR |= (1 << 6);
  
  SPI->CR1 |= (1 << 6);
  SPI->CR1 |= (1 << 2);
}
void Spi_transmit_char(uint8_t c){
  while(!(SPI->SR & (1<<1)));
  SPI->DR = c;
}
void Spi_transmit_string(char *str){
  SPI->CR1 |= (1 << 6);
  uint8_t* c = (uint8_t*)str;
  while(*c){
    Spi_transmit_char(*c);
    c++;
  }
}
void Spi_transmit_number(uint16_t number){
  char buff[10];
  int length = 0;
  if(number == 0){
    buff[length++] = '0';
  }
  while(number > 0){
    buff[length++] = number%10 + '0';
    number /= 10;
  }
  for(int i = length - 1;i >= 0;i--){
  while(!(SPI->SR & (1<<1)));
    SPI->DR = buff[i];
  }
}
uint8_t Spi_receive(void){
  while(!(SPI->SR & (1 << 0)));
  return ((uint8_t)SPI->DR);
}
uint8_t Spi_interrupt_receive(){
  while(!Data_is_ready);
  Data_is_ready = 0;
  return Received_data;
}
uint8_t Spi_busy_flag(){
  return (SPI->SR & (1 << 7));
}