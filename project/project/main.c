#include <main.h>
#include <uart.c>
#include <gpio.c>
#include <system.c>
#include <adc.c>
#include <spi.c>
#include <lcd16x2.c>
#include <MenuLCD.c>

uint8_t txData = 0xAB;  // Example data to transmit
uint8_t rxData = 0;     // Variable to hold received data
uint8_t flag = 0;
INTERRUPT_HANDLER(SPI_IRQHandler, 10){
  while (!(SPI->SR & (1 << 0)));  // Wait for receive to complete
  Received_data = SPI->DR; 
  Data_is_ready = 1;
}
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18){
  while(!(UART1->SR & (1 << 5)));
  received_data = UART1->DR; 
  data_is_ready = 1;
}
int main( void ){
  __enable_interrupt();
  CLK_HSI_16Mhz_config();
//  Uart1_Init(9600);
//  Adc1_Init();
//  Spi_config();
  I2c_Init();
  LCD_Init();
  Gpio_input_mode_port_A(3);
  Gpio_input_mode_port_A(2);
  Gpio_input_mode_port_A(1);
//  Gpio_ouput_mode_port_C_pin_6_pin_5();
//  Gpio_input_mode_port_C_pin_7();
//  LCD_Light_BackGround(1);
//  LCD_Light_BackGround(1);
//  LCD_set_cursor(1, 1);  // Move cursor to first line
//  LCD_SendString("Yeu em");
//  LCD_set_cursor(2, 1);  // Move cursor to second line
//  LCD_SendString("Yeu em");
//  LCD_Light_BackGround(1);
  Menu *menu = &MainMenu;
  
  Button_name ButtonA1;
  ButtonA1.BUTTON_pin = 1;
  ButtonA1.timePress = 0;
  ButtonA1.isPress = 0;

  Button_name ButtonA2;
  ButtonA2.BUTTON_pin = 2;
  ButtonA2.timePress = 0; 
  ButtonA2.isPress = 0;

  Button_name ButtonA3;
  ButtonA3.BUTTON_pin = 3;
  ButtonA3.timePress = 0;
  ButtonA3.isPress = 0;

  uint32_t select = 1;
  LCD_SendCommand(0x01);
  Delay_ms(2);
  Menu_Display(menu, select);
  while(1){
//    GPIOB->ODR ^= (1 << 5);
//    Uart1_transmit_number(12345);
//    uint8_t c = Uart1_interrupt_receive();
//    Uart1_transmit_char(c);
//    Uart1_transmit_string("\r\n");
    
//    uint16_t value = Adc1_read(3);
//    Uart1_transmit_number(value);
//    Uart1_transmit_string("\r\n");
//    Delay_ms(500);
//    Spi_transmit_char(txData);
//    while (!(SPI->SR & (1 << 0)));  // Wait for receive to complete
//    rxData = Spi_interrupt_receive();

    // Check if data matches
//    if (rxData == txData) {
//    // Successful loopback test
//      Uart1_transmit_number(rxData);
//      Uart1_transmit_string("\r\n");
//      Delay_ms(10);
//    }
//    txData++;
    
    while(Button_read(&ButtonA3) == SINGLE_CLICK){//down
      select = (select == 2) ? 1 : select + 1;
      Menu_Display(menu, select);
      Delay_ms(70);
    }
    while(Button_read(&ButtonA2) == SINGLE_CLICK){//up
      select = (select == 1) ? 2 : select - 1;
      Menu_Display(menu, select);
      Delay_ms(70);
    }
    while(Button_read(&ButtonA1) == SINGLE_CLICK){//enter
      Delay_ms(500);
      if(Button_read(&ButtonA1) == SINGLE_CLICK){
        menu = (menu->previousMenu == NULL) ? menu : menu->previousMenu;
        select = 1;
        Menu_Display(menu, select);
        Delay_ms(50);
      }
      else{        
        switch(select){
          case 1: menu = (menu->MenuList1 == NULL) ? menu : menu->MenuList1;
            break;
          case 2: menu = (menu->MenuList2 == NULL) ? menu : menu->MenuList2;
            break;
        }
        select = 1;
        Menu_Display(menu, select);
        Delay_ms(50);
      }
    }
  }
}


