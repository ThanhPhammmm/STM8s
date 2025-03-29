#include <uart.h>

uint8_t array_receive[20] = "";
uint16_t count_array_receive = 0;

void UART_Configuration(uint32_t baudrate){ 
  CLK->CKDIVR = 0;
  CLK->PCKENR1 = 0xff;
  CLK->PCKENR2 = 0xff;
  
  GPIOD->DDR |=  (1 << 5); // TX
  GPIOD->DDR &= ~(1 << 6); // RX
  
  // baudrate 16MHZ / 57600 = 277 = 0x115
  // baudrate 16MHZ / 9600 = 1666 = 0x682
  
  switch (baudrate)
  {
    case 9600:
      UART1->BRR1 = 0x68;
      UART1->BRR2 = 0x03;
      break;
    case 57600:
      UART1->BRR1 = 0x11;
      UART1->BRR2 = 0x05;
      break;
      break;
  }
  
  UART1->CR1  = (0 << 5);
  UART1->CR2  = (1 << 2) | (1 << 3) | (1 << 5);
  
  enableInterrupts(); 
}

/**
 * @brief Ham gui mang
 * 
 * @param array,length
 */
void UART_Send_Array(uint8_t* array, uint16_t length){
    for (uint16_t i = 0; i < length; i++) 
    {
      UART1_Send_Char(array[i]);
      
      while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    }
}

/**
 * @brief Ham gui chuoi ky tu
 * 
 * @param *message
 */
void UART_Printf_String (char *message) {
  while ((*message))																							
  {
    UART1_Send_Char((unsigned char) *message);									
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);	
    message++;                               
  }
}
/**
 * @brief Ham ngat nhan mang
 * 
 */

uint16_t RX_FLAG_END_LINE = 0;

INTERRUPT_HANDLER(UART1_RX_IRQHandler,18){
  if (UART1_GetFlagStatus(UART1_FLAG_RXNE)) {  // Check if data is available
        
    uint8_t temp = UART1->DR;
    cat_fsm_status status = cat_receive_fsm(&at, temp);
    if (status == CAT_FSM_RECEIVE_DONE) {
            RX_FLAG_END_LINE = 1;
    } 
    array_receive[count_array_receive] = temp;
  }
}

uint8_t Usart_CheckFlag_Fsm (void )
{
	if(RX_FLAG_END_LINE == 1)
	{
		RX_FLAG_END_LINE = 0;
		return 1;
	}
	return 0;
}

void UART1_Send_Char(char c)
{
    while (!(UART1->SR & UART1_SR_TXE)); // Wait until TX buffer is empty
    UART1->DR = c; // Send character
}

// Function to send a string over UART
void UART1_Send_String(const char *str)
{
    while (*str)
    {
        UART1_Send_Char(*str++);
    }
}
void UART1_Send_Int(int num)
{
    char buffer[10];
    int i = 0;

    if (num == 0)
    {
        UART1_Send_Char('0');
        return;
    }

    while (num > 0)
    {
        buffer[i++] = (num % 10) + '0'; // Extract last digit and convert to char
        num /= 10;
    }

    while (i > 0)
    {
        UART1_Send_Char(buffer[--i]); // Send digits in correct order
    }
}
void UART1_Send_Float(float num, uint8_t decimal_places)
{
    if (num < 0)
    {
        UART1_Send_Char('-');  // Send negative sign
        num = -num; // Convert to positive
    }

    int whole_part = (int)num;  // Extract integer part
    int decimal_part = (int)((num - whole_part) * (10 * decimal_places)); // Extract decimal part

    UART1_Send_Int(whole_part); // Send integer part
    UART1_Send_Char('.');       // Send decimal point
    UART1_Send_Int(decimal_part); // Send decimal part
}
