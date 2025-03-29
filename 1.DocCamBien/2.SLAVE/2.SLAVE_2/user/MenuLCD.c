#include <MenuLCD.h>

uint8_t select = 1;

Menu_t MainMenu;
Menu_t SLAVE1_MENU;
Menu_t SLAVE2_MENU;
Menu_t RTC;

Menu_t RTC ={
  &SLAVE2_MENU,
  
  NULL,
  
  NULL
};
Menu_t SLAVE2_MENU = {
  &MainMenu,

  NULL,
  
  &RTC
};
Menu_t SLAVE1_MENU = {
  &MainMenu,
  
  NULL,
  
  NULL
};
Menu_t MainMenu = {
  NULL,
   
  &SLAVE1_MENU,
  
  &SLAVE2_MENU
};

Menu_t *currentMenu = &MainMenu;

void Menu_Display_Select(Menu_t *menu, uint8_t select){
  LCD_SendCommand(0x01);  // Clear screen

  if (currentMenu == &MainMenu && start_flag == 1) {
    // Main Menu - show options for SLAVE1 and SLAVE2
    LCD_Set_Cursor(1, 5);
    LCD_SendString("1.SLAVE1");
    //LCD_SendString("HUYEN XINH");
    LCD_Set_Cursor(2, 5);
    LCD_SendString("1.SLAVE2");
    //LCD_SendString("HUYEN THUI");
    LCD_Set_Cursor(select, 4);
    LCD_SendString(">");
  }
}

// Define a function to display sensor data (LUX, RES, RTC, NTC)
void Send_Data_To_LCD(float data_sensor1, uint16_t data_sensor2, uint8_t *data_sensor3, float data_sensor4) {
  LCD_SendCommand(0x01);  // Clear screen
  if(currentMenu == &MainMenu){
    Menu_Display_Select(&MainMenu, select);
  }
  else if(currentMenu == &SLAVE1_MENU){
    LCD_Set_Cursor(1, 1);
    LCD_SendString("LUX Data: ");
    //LCD_SendString("Thanh khong yeu");
    LCD_Display_Float(data_sensor1);  // Display LUX sensor data
    LCD_Set_Cursor(2, 1);
    LCD_SendString("RES Data: ");
    //LCD_SendString("ai ngoai Huyen");
    LCD_Send16BitNumber(data_sensor2, 2);  // Display RES sensor data
  }
  // NTC submenu - show NTC sensor data
  else if(currentMenu == &SLAVE2_MENU){
    LCD_Set_Cursor(1, 1);
    LCD_SendString("NTC: ");
    LCD_Display_Float(data_sensor4);  // Display NTC sensor data
    //LCD_SendString("Lop 5A1 tieu hoc");
    LCD_Set_Cursor(2, 1);
    LCD_SendString("->Watch RTC");
    //LCD_SendString("Thang Quan gioi");
  }
  else if(currentMenu == &RTC){
    Send_RTC_To_LCD(data_sensor3);
  }
}