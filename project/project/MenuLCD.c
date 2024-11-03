#include <stddef.h>

uint32_t select = 1;

typedef struct Linker{
  struct Linker *previousMenu;
  
  char List1[16];
  struct Linker *MenuList1;
  
  char List2[16];
  struct Linker *MenuList2;
} Menu_t;
Menu_t MainMenu;
Menu_t SubMenu2;
Menu_t SubMenu1;
Menu_t SubMenu6 = {
  &SubMenu2,
  
  " Example 13", 
  NULL,
  
  " Example 14", 
  NULL
};
Menu_t SubMenu5 = {
  &SubMenu2,
  
  " Example 11", 
  NULL,
  
  " Example 12", 
  NULL
};
Menu_t SubMenu4 = {
  &SubMenu1,
  
  " Example 9 ", 
  NULL,
  
  " Example 10", 
  NULL
};
Menu_t SubMenu3 = {
  &SubMenu1,
  
  " Example 7 ", 
  NULL,
  
  " Example 8 ", 
  NULL
};
Menu_t SubMenu1 = {
  &MainMenu,

  " Example 3 ", 
  &SubMenu3,
  
  " Example 4 ", 
  &SubMenu4
};
Menu_t SubMenu2 = {
  &MainMenu,
  
  " Example 5 ", 
  &SubMenu5,
  
  " Example 6 ", 
  &SubMenu6
};
Menu_t MainMenu = {
  NULL,
  
  " Example 1 ", 
  &SubMenu1,
  
  " Example 2 ", 
  &SubMenu2
};
typedef struct
{
        uint8_t status;
        uint8_t status_old;
        uint8_t flag_press;
        uint8_t flag_change;

        unsigned int count_press;
        unsigned int count_time_hold;
        unsigned int time_out;
        unsigned int status_result;
        
        unsigned int pin;
} typein_t;

Menu_t *menu = &MainMenu;
void Menu_Display(Menu_t *menu, uint32_t select){
  LCD_Set_Cursor(1, 4);  // Move cursor to first line
  LCD_SendString(menu->List1);
  
  LCD_Set_Cursor(2, 4);  // Move cursor to second line
  LCD_SendString(menu->List2);
  
  LCD_Set_Cursor(select, 4);
  LCD_SendString(">");
}
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
    else
    {
      datain->flag_press = 0;
      datain->count_press = 0;
    }

    datain->time_out = 0;
    datain->count_time_hold++;
  }
  else if (datain->status == 1)
  {
    if (datain->time_out >= 2500)
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
void Button_Run(typein_t *button_check)
{
  uint8_t state = Gpio_Read_Input_Port_A(button_check->pin);
  Button_ReadInput(state, button_check);

  if (button_check->status_result == 1) // nhan 1 lan
  {
    if (button_check->flag_change == 1) // co bat khi khac trang thai truoc do
    {
      if(button_check->pin == 3){//up
        select = (select == 1) ? 2 : select - 1; 
        Menu_Display(menu, select);
      }
      else if(button_check->pin == 2){//down
        select = (select == 2) ? 1 : select + 1; 
        Menu_Display(menu, select);
      }
      else if(button_check->pin == 1){
        if(select == 1){
          menu = (menu->MenuList1 == NULL) ? menu : menu->MenuList1;
        }
        else if(select == 2){
          menu = (menu->MenuList2 == NULL) ? menu : menu->MenuList2;
        }
        select = 1;
        Menu_Display(menu, select);
      }
      button_check->status_result = 0;
      button_check->flag_change = 0;
    }
  }
  else if (button_check->status_result == 2)
  {
    if (button_check->flag_change == 1)
    {
      if(button_check->pin == 1){
        menu = (menu->previousMenu == NULL) ? menu : menu->previousMenu;
        select = 1;
        Menu_Display(menu, select);
      }
        button_check->status_result = 0;
        button_check->flag_change = 0;
    }
  }
}