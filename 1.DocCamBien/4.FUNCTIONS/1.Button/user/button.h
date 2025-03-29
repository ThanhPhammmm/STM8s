#ifndef __BUTTON_H
#define __BUTTON_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include<stm8s.h> 
#include<gpio.h>
#include<uart.h>
      
typedef struct
{
    uint8_t status;
    uint8_t status_old;
    uint8_t flag_press;   // 1 while the button is pressed
    uint8_t flag_change;  // 1 when a valid event is detected

    unsigned int count_press;      // Counts the number of press events (1 for single, 2 for double)
    unsigned int count_time_hold;  // Increments while the button is continuously pressed
    unsigned int time_out;         // Increments after release until the event is finalized
    unsigned int status_result;    // Final result: 1 for single, 2 for double, 10 for long press

    unsigned int pin;              // Which pin this button uses (for Gpio_Read_Input_Port_A)
} typein_t;

void Button_ReadInput(uint8_t input, typein_t *datain);
void Button_Run(typein_t *button_check);
void Button_Init(void);

#ifdef __cplusplus
}
#endif

#endif    