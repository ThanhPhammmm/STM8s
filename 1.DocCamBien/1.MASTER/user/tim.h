#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
      
#define COUNT_UP        0
#define COUNT_DOWN      1
#define UPDATE_EVENT    0
#define ON              1
#define OFF             0

void TIM1_BaseInit(uint16_t prescaler, uint8_t type, uint16_t period, uint8_t repitition);
void TIM1_IT_Config(uint8_t type, uint8_t state);
void TIM1_CLEAR_IT_PENDING(uint8_t type);
void TIM2_BaseInit(uint16_t prescaler, uint8_t type, uint16_t period);
void TIM2_CLEAR_IT_PENDING(uint8_t type);
#ifdef __cplusplus
}
#endif

#endif  