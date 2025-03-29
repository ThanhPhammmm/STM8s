#ifndef __SYSTEM_H
#define __SYSTEM_H

#ifdef __cplusplus
    extern "C" {
#endif

#include <stm8s.h>    
      
void Delay_ms(uint16_t ticks);
void CLK_HSI_16Mhz_config(void);

#ifdef __cplusplus
}
#endif

#endif   
