#ifndef __RES_H
#define __RES_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
      
void ADC_Init(void);      
uint16_t Read_ADC();     

#ifdef __cplusplus
}
#endif

#endif