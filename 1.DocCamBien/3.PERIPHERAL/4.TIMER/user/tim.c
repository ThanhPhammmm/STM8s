#include<tim.h>

void TIM1_IT_Config(uint8_t type, uint8_t state){
  if(state == ON){
    TIM1->IER |= (1 << type);
  }
  else if(state == OFF){
    TIM1->IER &= ~(1 << type);
  }
}
void TIM1_BaseInit(uint16_t prescaler, uint8_t type, uint16_t period, uint8_t repitition){
  TIM1->ARRH = (uint8_t)(period >> 8);
  
  TIM1->ARRL = (uint8_t)(period);
  
  TIM1->PSCRH = (uint8_t)(prescaler >> 8);
  
  TIM1->PSCRL = (uint8_t)(prescaler);
    
  TIM1->RCR = repitition;
  
  TIM1_IT_Config(UPDATE_EVENT, ON);
  
  TIM1->CR1 |= (1 << 7) | (type << 4);
  
  TIM1->CR1 |= (1 << 0);
}
void TIM1_CLEAR_IT_PENDING(uint8_t type){
  TIM1->SR1 &= ~(1 << type);
}
void TIM1_Config(void) {
    // Enable TIM1 clock
    CLK->PCKENR2 |= (1 << CLK_PERIPHERAL_TIMER1);  

    // Set Prescaler to 16,000 (Fcpu / 16000)
    TIM1->PSCRH = (16000 - 1) >> 8;  // Prescaler High Byte
    TIM1->PSCRL = (16000 - 1) & 0xFF;  // Prescaler Low Byte (Prescaler = 16000)

    // Set Auto-reload value for 1s interrupt (Assuming 16MHz system clock)
    TIM1->ARRH = (1000 - 1) >> 8;   // Auto-Reload High Byte
    TIM1->ARRL = (1000 - 1) & 0xFF; // Auto-Reload Low Byte (1000 counts)

    // Enable Update Interrupt
    TIM1->IER |= TIM1_IER_UIE;

    // Enable TIM1 Counter
    TIM1->CR1 |= TIM1_CR1_CEN;
}