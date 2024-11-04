void I2c_Init(void);
void I2c_Generate_Start();
void I2c_Generate_Stop();
void I2c_Send_7bits_Address(uint8_t address);
void I2c_Send_Data(uint8_t data);
uint32_t Get_Flag_Status_Of_Bus(void);
uint32_t Check_Event_Start();
uint32_t Check_Event_Send_7bits_Address();
uint32_t Check_Event_Send_Data();
void I2c_Write(uint8_t address, uint8_t data);

#include <i2c.c>