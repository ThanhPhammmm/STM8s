#include <button.h>

void Button_Init(void) {
    GPIOA->DDR &= ~(1 << 3);  // Set PA3 as input
    GPIOA->CR1 |= (1 << 3);   // Enable pull-up resistor
    GPIOA->CR2 &= ~(1 << 3);  // Disnable interrupt on PA3
    
    GPIOA->DDR &= ~(1 << 2);  // Set PA2 as input
    GPIOA->CR1 |= (1 << 2);   // Enable pull-up resistor
    GPIOA->CR2 &= ~(1 << 2);  // Disnable interrupt on PA3
    
}
void Button_ReadInput(uint8_t input, typein_t *datain)
{
  datain->status = input;

  if (datain->status == 0)  // Button Pressed
  {
    if (datain->status != datain->status_old) // Detect falling edge
    {
      datain->count_press++;
    }

    if (datain->count_time_hold < 7)
    {
      datain->flag_press = 1;
    }
    else if (datain->count_time_hold > 10)
    {
      datain->status_result = 10;
      datain->flag_change = 1;
      datain->flag_press = 0;
      datain->count_press = 0;
    }
    datain->time_out = 0;
    datain->count_time_hold++;
  }
  else if (datain->status == 1)
  {
    if (datain->time_out >= 5)
    {
      if (datain->flag_press)
      {
        datain->flag_change = 1;
        datain->flag_press = 0;
      }
    }
    datain->count_time_hold = 0;
  }

  if (datain->flag_press)
  {
    datain->time_out++;
  }

  if (datain->flag_change)
  {
    if (datain->count_press != 0)
    {
      datain->status_result = datain->count_press;
    }
    datain->count_press = 0;
    datain->time_out = 0;
  }
  datain->status_old = datain->status;
}

void Button_Run(typein_t *button_check)
{
  uint8_t state = Gpio_Read_Input_Port_A(button_check->pin);
  Button_ReadInput(state, button_check);

  if (button_check->status_result == 1) // nhan 1 lan
  {
    if (button_check->flag_change == 1) // co bat khi khac trang thai truoc do
    {
      if(button_check->pin == 3){//enter
        if(start_flag == 1){
          if(select == 1){
            if(currentMenu->followingMenu1 != NULL){
              currentMenu = currentMenu -> followingMenu1;
            }
          }
          else if(select == 2){
            if(currentMenu->followingMenu2 != NULL){
              currentMenu = currentMenu -> followingMenu2;
            }
          }
        }
      }
      else if(button_check->pin == 2){//select
          select = select + 1;
          if(select == 3){
            select = 1;
          }
      }
      button_check->status_result = 0;
      button_check->flag_change = 0;
    }
  }
  else if (button_check->status_result == 2)
  {
    if (button_check->flag_change == 1)
    {
      if(button_check->pin == 2){
        if(currentMenu->previousMenu != NULL){
          currentMenu = currentMenu->previousMenu;
          if(currentMenu == &MainMenu){
            select = 1;
          }
        }
        button_check->status_result = 0;
        button_check->flag_change = 0;
      }
    }
  }
  else if(button_check->status_result == 10){
    if(button_check->pin == 3){//up
      if(start_flag == 0){
        start_flag = 1;
      }
    }
    button_check->status_result = 0;
    button_check->flag_change = 0;
  }
}