#ifndef __SLAVED_PORT_H
#define __SLAVED_PORT_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
#include <CreateFrame.h>
#include <uart.h>
#include <Detect.h>    
#include <GetData.h>
      
extern frame_Message_t detected_frame;
extern uint8_t array_receive[20];

void Send_Ack_Slave1_Sensor_Lux();
void Send_Ack_Slave1_Sensor_RES();
void Send_Ack_Slave2_Sensor_RTC();
void Send_Ack_Slave2_Sensor_NTC();
void Send_Sensor_Data_LUX();
void Send_Sensor_Data_RES();
uint8_t Wait_For_Set_Slave_Slave_1();
uint8_t Wait_For_Set_Slave_Slave_2();
uint8_t Wait_For_Ask_Data_Slave1_SensorLux();
uint8_t Wait_For_Ask_Data_Slave1_SensorRES();
uint8_t Wait_For_Ask_Data_Slave2_SensorRTC();
uint8_t Wait_For_Ask_Data_Slave2_SensorNTC();
void Send_Sensor_Data_LUX_Slave1();
void Send_Sensor_Data_RES_Slave1();
void Send_Sensor_Data_RTC_Slave2();
void Send_Sensor_Data_NTC_Slave2();



#ifdef __cplusplus
}
#endif

#endif 