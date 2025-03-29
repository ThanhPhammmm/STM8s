#ifndef __NTC_H
#define __NTC_H

#ifdef __cplusplus

    extern "C" {
#endif
      
#include <stm8s.h>      
      
      
void DHT11_Start(void);      
uint8_t DHT11_CheckResponse(void);
uint8_t DHT11_ReadByte(void);
uint8_t DHT11_ReadTemperature();
void TIM2_Init();
void DHT11_Init(void);

#define DHT11_PORT  GPIOC
#define DHT11_PIN   GPIO_PIN_6

#ifdef __cplusplus
}
#endif

#endif    