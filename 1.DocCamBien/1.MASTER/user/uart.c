#include <uart.h>
#include <FSM.h>

uint8_t array_receive[20] = "";
uint8_t count_array_receive = 0;
uint8_t count_timeout_uart = 0;
uint8_t flag_array_receive = 0;

void UART_Configuration(uint16_t baudrate){ 
  CLK->CKDIVR = 0;
  CLK->PCKENR1 = 0xFF;
  CLK->PCKENR2 = 0xFF;
  
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
  
  UART1->CR1  = 0x00;  // Ensure no parity, 8-bit, etc.
  UART1->CR3  = 0x00;  // Ensure no LIN mode, no half-duplex
  UART1->CR2  = (1 << 2) | (1 << 3) | (1 << 5);
}

void UART_Send_Array(uint8_t* array, uint8_t length){
    for (uint8_t i = 0; i < length; i++) 
    {
      UART1_Send_Char(array[i]);
      
      while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    }
}

volatile uint8_t txBuffer[BUFFER_SIZE];

uint8_t txIndex = 0;
uint8_t txLength = 0;

void UART_Send_Array_IT(uint8_t* array, uint8_t length) {
    if (length == 0 || txLength > 0) return; // Ignore if busy
    
    memcpy((void*)txBuffer, array, length); // Copy data
    txLength = length;
    txIndex = 0;
    
    // Enable TXE (Transmit Empty) Interrupt
    UART1_ITConfig(UART1_IT_TXE, ENABLE);
}

// UART Interrupt Handler
INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17) {
    if (UART1_GetITStatus(UART1_IT_TXE) != RESET) {
        if (txIndex < txLength) {
            UART1_SendData8(txBuffer[txIndex++]);  // Send next byte
        } 
        else {
            UART1_ITConfig(UART1_IT_TXE, DISABLE); // Disable TXE interrupt
            UART1_ClearITPendingBit(UART1_IT_TXE);
            txLength = 0; // Mark transmission complete
        }
    }
}

void Process_Received_Byte(uint8_t temp){
  if(flag_array_receive == WAIT_RECEIVE)
  {
      flag_array_receive = RECEIVING;
      count_array_receive = 0;  // Reset buffer index at the start
  }
    
  volatile uint16_t length = 0;
      
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
    }
  }
}
INTERRUPT_HANDLER(UART1_RX_IRQHandler,18){
  if (UART1_GetFlagStatus(UART1_FLAG_RXNE) && receive_flag == 1) {
    // Check if data is available
    volatile uint8_t temp = UART1->DR;
    Process_Received_Byte(temp);
    
  }
  //count_timeout_uart = 0;  // Reset timeout counter on valid reception
  //UART1_ClearITPendingBit(UART1_IT_RXNE);  // ? Clear interrupt flag
}

void UART1_Send_Char(char c)
{
    while (!(UART1->SR & UART1_SR_TXE)); // Wait until TX buffer is empty
    UART1->DR = c; // Send character
}

uint8_t UART_Check_New_Message(){
  if((Check_Fsm_Flag_New_Message() == 1) && (flag_array_receive == END_RECEIVE))
  {
    receive_flag = 0;
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

uint8_t Wait_For_UART_Frame_1s() {
    uint16_t timeout = 65000;

    while (timeout > 0) {
        if (UART_Check_New_Message()) {
            return 1;  // ? Frame received
        }
        timeout--;
    }

    return 0;  // ? Timeout reached (no frame received)
}