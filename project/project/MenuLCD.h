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

void Menu_Display(Menu_t *menu, uint32_t select);
void Button_ReadInput(uint8_t input, typein_t *datain);
void Button_Run(typein_t *button_check);

#include <MenuLCD.c>
