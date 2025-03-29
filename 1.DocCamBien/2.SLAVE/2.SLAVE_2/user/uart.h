#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus

    extern "C" {
#endif

#include <stm8s.h>
#include <string.h>
      
#define WAIT_RECEIVE 0
#define RECEIVING 1
#define END_RECEIVE 2
#define BUFFER_SIZE 20  // Adjust based on needs

extern uint8_t receive_flag;

void UART_Configuration (uint16_t baudrate);
void UART_Send_Array (uint8_t* array, uint8_t length); 
void UART_Send_Array_IT(uint8_t* array, uint8_t length);
void UART_Printf_String (char *message);
void UART1_Send_Char(char c);
void UART1_Send_String(const char *str);
INTERRUPT_HANDLER(UART1_RX_IRQHandler,18);
uint8_t UART_Check_New_Message();
void UART_Clear_All_Flag();
uint8_t Wait_For_UART_Frame_1s();

#ifdef __cplusplus
}
#endif

#endif    