#include <GetData.h>

uint8_t Get_slave(frame_Message_t datain){
  return datain.Data[0];
}
uint8_t Get_Sensor(frame_Message_t datain){
  return datain.Data[1];
}