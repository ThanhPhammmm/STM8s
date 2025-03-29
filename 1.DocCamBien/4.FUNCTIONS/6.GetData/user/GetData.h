#ifndef __GETDATA_H
#define __GETDATA_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
#include <CreateFrame.h>
      
uint8_t Message_Get_Port(frame_Message_t datain_frame);
uint8_t Message_Get_Type_Sensor(frame_Message_t datain_frame);
float Message_Get_Data_Sensor_Lux(frame_Message_t datain_frame);
uint16_t Message_Get_Data_Sensor_Res(frame_Message_t datain_frame);

#ifdef __cplusplus
}
#endif

#endif