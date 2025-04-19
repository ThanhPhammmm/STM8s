#ifndef __BUTTON_H
#define __BUTTON_H

#ifdef __cplusplus
    extern "C" {
#endif

#include <stm8s.h>      
#include <gpio.h>
#include <stddef.h>      
#include <MenuLCD.h>

typedef struct{
  uint8_t status;
  uint8_t status_old;
  uint8_t flag_press;
  uint8_t flag_change;

  uint8_t count_press;
  unsigned int count_time_hold;
  unsigned int time_out;
  uint8_t status_result;

  uint8_t pin;
}typein_t;    

extern volatile uint8_t start_flag;
extern uint8_t select;
extern Menu_t *currentMenu;
extern Menu_t MainMenu;

void Button_ReadInput(uint8_t input, typein_t *datain);
void Button_Run(typein_t *button_check);
void Button_Init(void);

#ifdef __cplusplus
}
#endif

#endif  