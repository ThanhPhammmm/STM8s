#include <uart.h>
#include <FSM.h>

uint8_t array_receive[20] = "";
uint16_t count_array_receive = 0;
uint16_t count_timeout_uart = 0;
uint8_t flag_array_receive = 0;
uint8_t array_send[10] = "";

//#define SLAVE_1_ID  0x01
//#define SLAVE_2_ID  0x02
//#define TIMEOUT  10000  // Set a timeout to prevent blocking indefinitely

/**
 * @brief Ham cau hinh uart
 * 
 * @param baudrate 
 */
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
      UART1_SendData8(array[i]);
      
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
    UART1_SendData8((unsigned char) *message);									
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);	
    message++;                               
  }
}

/**
 * @brief Ham ngat nhan chuoi ky tu
 * 
 */
//void UART_Interrupt_Receive_String()
//{
//  char temp = UART1->DR;
//  if(temp != '!')
//  {
//    array_receive[count_array_receive] = temp;
//    count_array_receive++;
//  }
//  else
//  {
//    array_receive[count_array_receive] = 0;
//    count_array_receive=0;
//    flag_array_receive = 1;
//  }
//}

/**
 * @brief Ham ngat nhan mang
 * 
 */
INTERRUPT_HANDLER(UART1_RX_IRQHandler,18){
  if (UART1_GetFlagStatus(UART1_FLAG_RXNE)) {  // Check if data is available
        
    uint8_t temp = UART1->DR;

    if(flag_array_receive == WAIT_RECEIVE)
    {
        flag_array_receive = RECEIVING;
    }
      
    uint16_t length = 0;
        
    if(flag_array_receive == RECEIVING)
    {  
        array_receive[count_array_receive] = temp;
        count_array_receive++;
        FSM_Get_Message(temp, array_receive);
    }
    
    if(count_array_receive >= 6)
    {
      length = Convert_bytes_into_uint16_t(array_receive[4], array_receive[5]);
      
      if(count_array_receive == (length + 4))
      {
        count_array_receive = 0;
        flag_array_receive = END_RECEIVE;
        //flag_fsm_new_message = 1;
      }
    }
  }
  count_timeout_uart = 0;  // Reset timeout counter on valid reception
}
uint8_t UART_Check_New_Message(){
  if(Check_Fsm_Flag_New_Message() == 1 && flag_array_receive == END_RECEIVE)
  {
    UART_Clear_All_Flag();
    return 1;
  }
  return 0;
}
void UART_Clear_All_Flag(){
  Clear_All_State_Fsm();
  flag_array_receive = WAIT_RECEIVE;
  count_array_receive = 0;
  count_timeout_uart = 0;
}
void UART_Time_Out(){
  if(flag_array_receive != WAIT_RECEIVE)
  {
    count_timeout_uart ++;
    if(count_timeout_uart > 20000)
    {
      UART_Clear_All_Flag();
    }
  }
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
