uint32_t F_CPU = 16000000L;
uint32_t data_is_ready = 0;
uint8_t received_data_using_uart = 0;

void Uart1_Init(uint16_t Baudrate){
  UART1->CR1 &= (0 << 5);
  UART1->CR1 &= (0 << 5);
  UART1->CR1 &= (0 << 2);

  UART1->CR2 |= (1 << 3);
  UART1->CR2 |= (1 << 2);
//  UART1->CR2 |= (1 << 7);
  UART1->CR2 |= (1 << 5);
  
  uint32_t baud_divisor = (F_CPU + Baudrate / 2) / Baudrate;
  UART1->BRR1 = (baud_divisor >> 8) & 0xF0;
  UART1->BRR2 |= baud_divisor & 0x0F;
  UART1->BRR1 = (baud_divisor >> 4) & 0xFF;

  UART1->CR3 &= (0x00 << 4);
}
void Uart1_Transmit_Char(uint8_t c){
  while(!(UART1->SR & (1 << 7)));
  UART1->DR = c;
}
void Uart1_Transmit_String(char *str){
  uint8_t *c = (uint8_t *)str; 
  while(*c){
    Uart1_Transmit_Char(*c);
    c++;
  }
}
void Uart1_Transmit_Number(uint16_t number){
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
    while(!(UART1->SR & (1 << 7)));
    UART1->DR = buff[i];
  }
}
uint8_t Uart1_Receive(){
  while(!(UART1->SR & (1 << 5)));
  return UART1->DR; 
}
uint8_t Uart1_Interrupt_Receive(){
  while(!data_is_ready);
  data_is_ready = 0;
  return received_data_using_uart;
}