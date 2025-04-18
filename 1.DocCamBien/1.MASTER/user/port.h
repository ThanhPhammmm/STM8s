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
      
typedef enum{
  Sending_SetSlave_to_slave1_sensorLux,
  Waiting_ConfirmSlave_from_slave1_sensorLux,
  Sending_AskData_to_slave1_sensorLux,
  Waiting_RespondData_from_slave1_sensorLux,

  Sending_SetSlave_to_slave1_sensorRES,
  Waiting_ConfirmSlave_from_slave1_sensorRES,
  Sending_AskData_to_slave1_sensorRES,
  Waiting_RespondData_from_slave1_sensorRES,

  Sending_SetSlave_to_slave2_sensorRTC,
  Waiting_ConfirmSlave_from_slave2_sensorRTC,
  Sending_AskData_to_slave2_sensorRTC,
  Waiting_RespondData_from_slave2_sensorRTC,

  Sending_SetSlave_to_slave2_sensorNTC,
  Waiting_ConfirmSlave_from_slave2_sensorNTC,
  Sending_AskData_to_slave2_sensorNTC,
  Waiting_RespondData_from_slave2_sensorNTC
}slave_state_e;

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