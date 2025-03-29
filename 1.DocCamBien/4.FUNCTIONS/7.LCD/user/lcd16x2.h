#ifndef __LCD16X2_H
#define __LCD16X2_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include<stm8s.h> 

void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_SendString(char* str);
void LCD_Init(void);
void LCD_Set_Cursor(uint32_t row, uint32_t column);
void LCD_Light_BackGround(uint8_t State);
void Send_Data_To_LCD(float data_sensor1, uint16_t data_sensor2, uint8_t *data_sensor3, float data_sensor4);
void LCD_Display_Float(float value);
void LCD_Send16BitNumber(uint16_t num, uint8_t min_digits);

#ifdef __cplusplus
}
#endif

#endif   