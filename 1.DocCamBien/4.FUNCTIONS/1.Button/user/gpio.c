#include <gpio.h>
//void Gpio_Init(GPIO_TypeDef *GPIOx){
//  GPIOx->DDR |= (1 << 5);
//  GPIOx->CR2 |= (1 << 5); 
//}
volatile uint8_t value;
void Gpio_Ouput_Mode_Port_B_Pin_5(){
  GPIOB->DDR |= (1 << 5);
  GPIOB->CR1 |= (1 << 5); 
}
void Gpio_Input_Mode_Port_A(uint8_t pin){
  GPIOA->DDR &= ~(1 << pin);
}
uint8_t Gpio_Read_Input_Port_A(unsigned int pin) {
  value = ((GPIOA->IDR) >> pin) & (0x1);
  return value;
}