#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus

    extern "C" {
#endif

#include "atc_user.h"      
#include <stm8s_uart1.h>



      
      
void UART_Configuration (uint32_t baudrate);
void UART_Send_Array (uint8_t* array, uint16_t length); 
void UART_Printf_String (char *message); 
void UART_Send_Array(uint8_t* array, uint16_t length);
void UART_Printf_String (char *message);
void UART1_Send_Char(char c);
void UART1_Send_String(const char *str);
void UART1_Send_Int(int num);
void UART1_Send_Float(float num, uint8_t decimal_places);
INTERRUPT_HANDLER(UART1_RX_IRQHandler,18);

uint8_t Usart_CheckFlag_Fsm (void );

#ifdef __cplusplus
}
#endif

#endif    