#ifndef __DETECT_H
#define __DETECT_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
#include <CreateFrame.h>      

uint16_t Message_Detect_TypeMessage(uint8_t* datain, frame_Message_t* dataout);
uint8_t Get_port(frame_Message_t datain);
uint8_t Get_Sensor(frame_Message_t datain);

#ifdef __cplusplus
}
#endif

#endif