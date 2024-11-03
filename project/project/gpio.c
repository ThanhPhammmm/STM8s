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
volatile uint8_t value;
void Gpio_Ouput_Mode_Port_C_Pin_6_Pin_5(){
  GPIOC->DDR |= (1 << 6);
  GPIOC->CR1 |= (1 << 6);
  GPIOC->CR2 |= (1 << 6); 
  
  GPIOC->DDR |= (1 << 5);
  GPIOC->CR1 |= (1 << 5);
  GPIOC->CR2 |= (1 << 5); 
}
void Gpio_Input_Mode_Port_C_Pin_7(){
  GPIOC->DDR &= ~(1 << 7);
  GPIOC->CR1 |= (1 << 7);
  GPIOC->CR2 &= ~(1 << 7);
}
void Gpio_Input_Mode_Port_A(uint8_t pin){
  GPIOA->DDR &= ~(1 << pin);
}
uint8_t Gpio_Read_Input_Port_A(unsigned int pin) {
  value = ((GPIOA->IDR) >> pin) & (0x1);
    return value;
}