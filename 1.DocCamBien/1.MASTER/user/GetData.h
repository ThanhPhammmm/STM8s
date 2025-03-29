#ifndef __GETDATA_H
#define __GETDATA_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
#include <CreateFrame.h>
      
uint8_t Get_slave(frame_Message_t datain);
uint8_t Get_Sensor(frame_Message_t datain);

#ifdef __cplusplus
}
#endif

#endif