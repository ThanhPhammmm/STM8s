#include <gpio.h>

volatile uint8_t value;

void Gpio_Input_Mode_Port_A(uint8_t pin){
  GPIOA->DDR &= ~(1 << pin);
}
uint8_t Gpio_Read_Input_Port_A(uint8_t pin) {
  value = ((GPIOA->IDR) >> pin) & (0x1);
  return value;
}
uint8_t Gpio_Read_Input_Port_D(uint8_t pin) {
  value = ((GPIOD->IDR) >> pin) & (0x1);
    return value;
}