#ifndef __PORT_H
#define __PORT_H

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

void Request_Slave(slave_e slave_id);
uint8_t Wait_For_Ack(slave_e slave_id, type_Sensor_e sensor_type);
uint8_t Wait_For_Respond(slave_e slave_id, type_Sensor_e sensor_type);
void Request_Slave_Data(slave_e slave_id, type_Sensor_e sensor_type);
float Receive_Slave_Data_LUX();
uint16_t Receive_Slave_Data_RES();
void Receive_Slave_Data_RTC(uint8_t *temp);
float Receive_Slave_Data_NTC();

#ifdef __cplusplus
}
#endif

#endif 