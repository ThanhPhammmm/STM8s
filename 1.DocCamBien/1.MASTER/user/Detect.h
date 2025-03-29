#ifndef __DETECT_H
#define __DETECT_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
#include <CreateFrame.h>      

uint8_t Message_Detect_TypeMessage(uint8_t* datain, frame_Message_t* dataout);

#ifdef __cplusplus
}
#endif

#endif