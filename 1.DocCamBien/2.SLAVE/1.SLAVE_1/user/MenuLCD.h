#ifndef __MENULCD_H
#define __MENULCD_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
#include <lcd16x2.h>
#include <stddef.h>      

typedef struct Linker{
  struct Linker *previousMenu;
  
  struct Linker *followingMenu1;
  
  struct Linker *followingMenu2;
} Menu_t;

extern float data_sensor1;
extern uint16_t data_sensor2;
extern uint8_t data_sensor3[7];
extern float data_sensor4;

extern uint8_t start_flag;

void Menu_Display_Select(Menu_t *menu, uint8_t select);
void Send_Data_To_LCD(float data_sensor1, uint16_t data_sensor2, uint8_t *data_sensor3, float data_sensor4);

#ifdef __cplusplus
}
#endif

#endif   
