#include<button.h>

void Button_ReadInput(uint8_t input, typein_t *datain)
{
  datain->status = input;
  if (datain->status == 0)
  {
    if (datain->status != datain->status_old)
    {
      datain->count_press++;
    }
    if (datain->count_time_hold < 5000)
    {
      datain->flag_press = 1;
    }
    else if(datain->count_time_hold > 6000){
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
    if (datain->time_out >= 2200)
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
  }
  datain->status_old = datain->status;
}

//extern uint8_t reset_flag;

void Button_Run(typein_t *button_check)
{
  uint8_t state = Gpio_Read_Input_Port_A(button_check->pin);
  Button_ReadInput(state, button_check);

  if (button_check->status_result == 1) // nhan 1 lan
  {
    if (button_check->flag_change == 1) // co bat khi khac trang thai truoc do
    {
      if(button_check->pin == 3){//up
        
      }
      button_check->status_result = 0;
      button_check->flag_change = 0;
    }
  }
  else if (button_check->status_result == 2)
  {
    if (button_check->flag_change == 1)
    {
        button_check->status_result = 0;
        button_check->flag_change = 0;
    }
  }
  else if(button_check->status_result == 10){

    button_check->status_result = 0;
    button_check->flag_change = 0;
  }
}
void Button_Init(void) {
    // Configure PA3 as input.
    GPIOA->DDR &= ~(1 << 3);
    // Enable the internal pull-up on PA3.
    GPIOA->CR1 |= (1 << 3);
    // Disable interrupts on PA3 for now.
    GPIOA->CR2 &= ~(1 << 3);
}