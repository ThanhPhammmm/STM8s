#include <main.h>
#define HIGH    1
#define LOW     0
#define ON      1
#define OFF     0
#define PUSH    0
#define RELEASE 1
//void Gpio_Init(GPIO_TypeDef *GPIOx){
//  GPIOx->DDR |= (1 << 5);
//  GPIOx->CR2 |= (1 << 5); 
//}
void Gpio_ouput_mode_port_C_pin_6_pin_5(){
  GPIOC->DDR |= (1 << 6);
  GPIOC->CR1 |= (1 << 6);
  GPIOC->CR2 |= (1 << 6); 
  
  GPIOC->DDR |= (1 << 5);
  GPIOC->CR1 |= (1 << 5);
  GPIOC->CR2 |= (1 << 5); 
}
void Gpio_input_mode_port_C_pin_7(){
  GPIOC->DDR &= ~(1 << 7);
  GPIOC->CR1 |= (1 << 7);
  GPIOC->CR2 &= ~(1 << 7);
}
void Gpio_input_mode_port_A(uint8_t pin){
  GPIOA->DDR &= ~(1 << pin);
}
uint8_t Gpio_read_input_port_A(uint8_t pin) {
    return (GPIOA->IDR & (1 << pin)) ? 1 : 0;
}