#ifndef __GPIO__
#define __GPIO__

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>


#define HIGH    1
#define LOW     0
#define ON      1
#define OFF     0
#define PUSH    0
#define RELEASE 1

void Gpio_Ouput_Mode_Port_C_Pin_6_Pin_5();
void Gpio_Input_Mode_Port_C_Pin_7();
void Gpio_Input_Mode_Port_A(uint8_t pin);
uint8_t Gpio_Read_Input_Port_A(unsigned int pin);

#ifdef __cplusplus
}
#endif

#endif