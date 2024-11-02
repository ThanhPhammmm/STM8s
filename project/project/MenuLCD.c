#include <stddef.h>

typedef enum {
  NO_CLICK = 0,
  SINGLE_CLICK = 1,
  LONG_CLICK = 2,
  DOUBLE_CLICK = 3,
} Button_State;
typedef struct {
  uint8_t BUTTON_pin;
  Button_State State;
  uint8_t isPress;
  uint8_t timePress;
  uint16_t timeDouble;
}Button_name;
Button_State Button_read(Button_name *Button){
  Button->State = NO_CLICK;
  Button->timePress = 0;
  Button->isPress = 0;
  Button->timeDouble = 0;
  while((Gpio_read_input_port_A(Button->BUTTON_pin) == PUSH)){
    Button->isPress = 1;
    Button->timePress++;
  }
  if(Button->isPress){
    while(Gpio_read_input_port_A(Button->BUTTON_pin) == RELEASE){
      Button->timeDouble++;
      if(Button->timeDouble > 30){
        if((Button->timePress > 30) && (Button->timePress <= 500)){
          Button->State = SINGLE_CLICK;
          Button->isPress = 0;
          Button->timePress = 0;
          Button->timeDouble = 0;
          return Button->State;
        }
      }
    }
    while(Gpio_read_input_port_A(Button->BUTTON_pin) == PUSH){
      Button->State = DOUBLE_CLICK;
      Button->isPress = 0;
      Button->timePress = 0;
      Button->timeDouble = 0;
      return Button->State;
    }
  }
  return NO_CLICK;
}
typedef struct Linker{
  struct Linker *previousMenu;
  
  char List1[16];
  struct Linker *MenuList1;
  
  char List2[16];
  struct Linker *MenuList2;
} Menu;
void Menu_Display(Menu *menu, uint32_t select){
  LCD_set_cursor(1, 4);  // Move cursor to first line
  LCD_SendString(menu->List1);
  
  LCD_set_cursor(2, 4);  // Move cursor to second line
  LCD_SendString(menu->List2);
  
  LCD_set_cursor(select, 4);
  LCD_SendString(">");
}
Menu MainMenu;
Menu SubMenu2;
Menu SubMenu1;
Menu SubMenu6 = {
  &SubMenu2,
  
  " Example 13", 
  NULL,
  
  " Example 14", 
  NULL
};Menu SubMenu5 = {
  &SubMenu2,
  
  " Example 11", 
  NULL,
  
  " Example 12", 
  NULL
};Menu SubMenu4 = {
  &SubMenu1,
  
  " Example 9 ", 
  NULL,
  
  " Example 10", 
  NULL
};Menu SubMenu3 = {
  &SubMenu1,
  
  " Example 7 ", 
  NULL,
  
  " Example 8 ", 
  NULL
};
Menu SubMenu1 = {
  &MainMenu,

  " Example 3 ", 
  &SubMenu3,
  
  " Example 4 ", 
  &SubMenu4
};
Menu SubMenu2 = {
  &MainMenu,
  
  " Example 5 ", 
  &SubMenu5,
  
  " Example 6 ", 
  &SubMenu6
};
Menu MainMenu = {
  NULL,
  
  " Example 1 ", 
  &SubMenu1,
  
  " Example 2 ", 
  &SubMenu2
};