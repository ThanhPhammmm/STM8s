#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>

#define WAIT_RECEIVE 0
#define RECEIVING 1
#define END_RECEIVE 2

void UART_Configuration (uint32_t baudrate);
void UART_Send_Array (uint8_t* array, uint16_t length); 
void UART_Printf_String (char *message); 
void UART_Interrupt_Receive_String ();
void UART_Time_Out();
uint8_t UART_Check_New_Message();
void UART_Clear_All_Flag();

INTERRUPT_HANDLER(UART1_RX_IRQHandler,18);

#ifdef __cplusplus
}
#endif

#endif