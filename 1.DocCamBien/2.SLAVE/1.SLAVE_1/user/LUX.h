#ifndef __LUX_H
#define __LUX_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
      
void I2C_Init_BH1750();    
void BH1750_Write(uint8_t command);
float BH1750_ReadLight();

#ifdef __cplusplus
}
#endif

#endif