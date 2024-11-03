#include <main.h>
#include <uart.c>
#include <gpio.c>
#include <system.c>
#include <adc.c>
#include <spi.c>
#include <lcd16x2.c>
#include <MenuLCD.c>

extern Menu_t *menu;
extern uint32_t select;

typein_t ButtonA1 = {.status_old = 1, .pin = 1};

typein_t ButtonA2 = {.status_old = 1, .pin = 2};

typein_t ButtonA3 = {.status_old = 1, .pin = 3};


int main( void ){
  CLK_HSI_16Mhz_Config();
  Gpio_Input_Mode_Port_A(1);
  Gpio_Input_Mode_Port_A(2);
  Gpio_Input_Mode_Port_A(3);
  I2c_Init();
  LCD_Init();
     
  LCD_SendCommand(0x01);
  Delay_Ms(100);
  Menu_Display(menu, select);
  while(1){
      Button_Run(&ButtonA1);
      Button_Run(&ButtonA3);
      Button_Run(&ButtonA2);
  }
}


