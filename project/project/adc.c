#include <main.h>

void Adc1_Init(void){
  ADC1->CR1 |= (1 << 0);
  
  ADC1->CR2 |= (1 << 3);
}

uint16_t Adc1_read(uint8_t channel){
  ADC1->CSR = channel;
  ADC1->CR1 |= (1 << 0);

  uint16_t temp = 0;
  
  while(!(ADC1->CSR & (1 << 7)));
  
  ADC1->CSR &= ~(1 << 7);
  
  temp = ADC1->DRL;
  temp |= (ADC1->DRH << 8);
  
  return temp;
}