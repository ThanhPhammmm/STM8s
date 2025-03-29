#include "my_code.h"
#include "stdio.h"
#include <math.h>

fsm_List_State_e fsm_state = FSM_STATE_START_FRAME;
uint16_t count_element_arr = 0;
uint16_t type_mess_fsm = 0;
uint16_t length_fsm = 0;
uint8_t flag_fsm_new_message = 0;

//extern uint16_t count_array_receive;

// fsm_List_State_e fsm_state ;
// uint16_t count_element_arr;
// uint16_t type_mess_fsm;
// uint16_t length_fsm;
// uint8_t flag_fsm_new_message;

uint8_t Message_Get_Port(frame_Message_t datain_frame)
{
  return datain_frame.Data[0];
}
uint8_t Message_Get_Type_Sensor(frame_Message_t datain_frame)
{
  return datain_frame.Data[1];
}
float Message_Get_Data_Sensor_Lux(frame_Message_t datain_frame)
{
  return Convert_bytes_into_float(datain_frame.Data[2], datain_frame.Data[3], datain_frame.Data[4], datain_frame.Data[5]);
}
//Chuyen doi int sang byte
uint8_t *Convert_int_to_bytes(int n){
  static union Int_to_bytes_format tmp;
  tmp.data = n;
  return tmp.bytes;//tra ve dia chi cua mang(dia chi cua gia tri dau tien cua mang)
}
//Chuyen doi byte sang int
uint32_t Convert_bytes_into_int(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4){
  static union Int_to_bytes_format tmp;
  tmp.bytes[0] = data1;//byte be nhat
  tmp.bytes[1] = data2;
  tmp.bytes[2] = data3;
  tmp.bytes[3] = data4;//byte lon nhat
  return tmp.data;
}
uint8_t *Convert_float_to_bytes(float n){
  static union Float_to_bytes_format tmp;
  tmp.data = n;
  return tmp.bytes;//tra ve dia chi cua mang(dia chi cua gia tri dau tien cua mang)
}
float Convert_bytes_into_float(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4){
  static union Float_to_bytes_format tmp;
  tmp.bytes[0] = data1;//byte be nhat
  tmp.bytes[1] = data2;
  tmp.bytes[2] = data3;
  tmp.bytes[3] = data4;//byte lon nhat
  return tmp.data;
}
uint8_t *Convert_uint16_t_to_bytes(uint16_t n){
  static union Unit16_t_to_bytes_format tmp;
  tmp.data = n;
  return tmp.bytes;
}
uint16_t Convert_bytes_into_uint16_t(uint8_t data1, uint8_t data2){
  static union Unit16_t_to_bytes_format tmp;
  tmp.bytes[0] = data1;//byte be nhat
  tmp.bytes[1] = data2;//byte lon nhat
  return tmp.data;
}
uint16_t Check_Sum(uint8_t *buf, uint8_t len)
{
  uint16_t crc = 0xFFFF, pos, i;
  for ( pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buf[pos];    // XOR byte into least sig. byte of crc
    for ( i = 8; i != 0; i--)     // Loop over each bit
    {
      if ((crc & 0x0001) != 0)    // If the LSB is set
      {
        crc >>= 1;                // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                        // Else LSB is not set
      {
        crc >>= 1;                // Just shift right
      }
    }
  }
  return crc;
}
uint16_t Message_Frame_Create_Set_Port( port_e datain_port, uint8_t *data_out)
{
  uint8_t *arr_temp_out;
  uint16_t count_arr_data = 0;

  frame_Message_t frame_data_in;
  frame_data_in.StartFrame = START_BYTE;
  frame_data_in.TypeMessage = TYPE_MESSAGE_SET_PORT;
  frame_data_in.Length = LENGTH_MESSAGE_SET_PORT;
  frame_data_in.Data[0] = datain_port;
  arr_temp_out = (uint8_t *) &frame_data_in;

  for(count_arr_data = 0; count_arr_data < LENGTH_DEFAULT + frame_data_in.Length - 2; count_arr_data++)
  {
    data_out[count_arr_data] = arr_temp_out[count_arr_data];
  }

  frame_data_in.CheckFrame = Check_Sum(data_out, LENGTH_DEFAULT + frame_data_in.Length - 2);

  data_out[count_arr_data] = (frame_data_in.CheckFrame & 0xFF);
  count_arr_data += 1;
  data_out[count_arr_data] = ((frame_data_in.CheckFrame >> 8) & 0xFF);
  count_arr_data += 1;

  return count_arr_data;
}
uint8_t Message_Frame_Create_Confirm_Port(uint8_t datain_port, uint8_t datain_sensor, uint8_t* dataout){
  frame_Message_t frame_confirm;
  frame_confirm.StartFrame = START_BYTE;
  frame_confirm.TypeMessage = TYPE_MESSAGE_CONFIRM_PORT;
  frame_confirm.Length = LENGTH_MESSAGE_CONFIRM;
  frame_confirm.Data[0] = datain_port;
  frame_confirm.Data[1] = datain_sensor;
  
  uint8_t* seonsor_confirm = (uint8_t*)&frame_confirm;
  
  frame_confirm.CheckFrame = Check_Sum(seonsor_confirm, (LENGTH_MESSAGE_CONFIRM + LENGTH_DEFAULT - 2));
  
  uint16_t count_confirm_data = 0;
  for(count_confirm_data = 0;count_confirm_data < (LENGTH_MESSAGE_CONFIRM + LENGTH_DEFAULT - 2);count_confirm_data++){
    dataout[count_confirm_data] = seonsor_confirm[count_confirm_data];
  }
  
  dataout[count_confirm_data] = frame_confirm.CheckFrame & 0xFF;
  dataout[count_confirm_data + 1] = ((frame_confirm.CheckFrame >> 8) & 0xFF);
  
  count_confirm_data += 2;
  return count_confirm_data;
}
uint8_t Message_Frame_Create_Ask_Data_Sensor(port_e datain_port, type_Sensor_e datain_sensor, uint8_t* dataout){
  frame_Message_t frame_ask;
  frame_ask.StartFrame = START_BYTE;
  frame_ask.TypeMessage = TYPE_MESSAGE_ASK_DATA;
  frame_ask.Length = LENGTH_MESSAGE_ASK_DATA;
  frame_ask.Data[0] = datain_port;
  frame_ask.Data[1] = datain_sensor;
  
  uint8_t* data_ask = (uint8_t*)&frame_ask;
  
  frame_ask.CheckFrame = Check_Sum(data_ask, (LENGTH_MESSAGE_ASK_DATA + LENGTH_DEFAULT - 2));
  
  uint16_t count_ask_data = 0;
  
  for(count_ask_data = 0;count_ask_data < (LENGTH_MESSAGE_ASK_DATA + LENGTH_DEFAULT - 2);count_ask_data++){
    dataout[count_ask_data] = data_ask[count_ask_data];
  }
  dataout[count_ask_data] = frame_ask.CheckFrame & 0xFF;
  dataout[count_ask_data + 1] = ((frame_ask.CheckFrame >> 8 )& 0xFF);
  count_ask_data += 2;
  return count_ask_data;
}
//LUX
uint16_t Message_Frame_Response_LUX(port_e datain_port, type_Sensor_e datain_sensor, float datain_value, uint8_t* dataout){
  uint8_t *data_convert = Convert_float_to_bytes(datain_value);
  frame_Message_t frame_response;
  frame_response.StartFrame = START_BYTE;
  frame_response.TypeMessage = TYPE_MESSAGE_RESPOND_DATA;
  frame_response.Length = LENGTH_MESSAGE_RESPOND_LUX;
  frame_response.Data[0] = datain_port;
  frame_response.Data[1] = datain_sensor;
  
  for(int i = 0;i < 4;i++){
    frame_response.Data[i + 2] = data_convert[i];
  }
  uint8_t* data_temp = (uint8_t*)&frame_response;
  uint16_t count_response_data = 0;
  for(count_response_data = 0; count_response_data <(LENGTH_MESSAGE_RESPOND_LUX + LENGTH_DEFAULT - 2);count_response_data++){
    dataout[count_response_data] = data_temp[count_response_data];
  }
  frame_response.CheckFrame = Check_Sum(data_temp, (LENGTH_MESSAGE_RESPOND_LUX + LENGTH_DEFAULT - 2));
  dataout[count_response_data] = frame_response.CheckFrame & 0xFF;
  dataout[count_response_data + 1] = (frame_response.CheckFrame >> 8) & 0xFF;
  count_response_data += 2;
  return count_response_data;
}
uint16_t Message_Detect_TypeMessage(uint8_t* datain, frame_Message_t* dataout){
  uint8_t count_data = 0;
  dataout->StartFrame = Convert_bytes_into_uint16_t(datain[count_data], datain[count_data + 1]);
  printf("0x%x\n", dataout->StartFrame);
  printf("0x%x %d\n", dataout->StartFrame, START_BYTE);
  count_data += 2;
  if(dataout->StartFrame != START_BYTE){
    return 0;
  }
  dataout->TypeMessage = Convert_bytes_into_uint16_t(datain[count_data], datain[count_data + 1]);
  printf("0x%x\n", dataout->TypeMessage);
  count_data += 2;
  dataout->Length = Convert_bytes_into_uint16_t(datain[count_data], datain[count_data + 1]);
  printf("0x%x\n", dataout->Length);
  count_data += 2;
  for(uint8_t i = 0;i < (dataout->Length - LENGTH_DEFAULT);i++){
    dataout->Data[i] = datain[count_data];
    printf("0x%x\n", dataout->Data[i]);
    count_data += 1;
  }
  dataout->CheckFrame = Convert_bytes_into_uint16_t(datain[count_data], datain[count_data + 1]);
  printf("0x%x\n", dataout->CheckFrame);
  return dataout->TypeMessage;
}


void Clear_All_State_Fsm(void){
    count_element_arr = 0;
    length_fsm = 0;
    flag_fsm_new_message = 0;
    fsm_state = FSM_STATE_START_FRAME;
    //count_array_receive = 0;
}

void FSM_Get_Message(uint8_t datain, uint8_t *dataout){
  switch(fsm_state){
  case FSM_STATE_START_FRAME:
    dataout[count_element_arr] = datain;
    count_element_arr++;
    if(count_element_arr == FSM_STATE_CHANGE_VALUE_TYPE_MESS){
      if(Convert_bytes_into_uint16_t(dataout[0], dataout[1]) != 0xAA55){
        Clear_All_State_Fsm();
      }
      else{
        fsm_state = FSM_STATE_TYPE_MESS;
      }
    }
    break;
  case FSM_STATE_TYPE_MESS:
    dataout[count_element_arr] = datain;
    count_element_arr++;
    if(count_element_arr == FSM_STATE_CHANGE_VALUE_LENGTH){
      type_mess_fsm = Convert_bytes_into_uint16_t(dataout[2], dataout[3]);
      if(type_mess_fsm == TYPE_MESSAGE_SET_PORT 
         || type_mess_fsm == TYPE_MESSAGE_CONFIRM_PORT 
         || type_mess_fsm == TYPE_MESSAGE_ASK_DATA 
         || type_mess_fsm == TYPE_MESSAGE_RESPOND_DATA){
        fsm_state = FSM_STATE_LENGTH;
      }
      else{
        Clear_All_State_Fsm();
      }
    }
    break;
  case FSM_STATE_LENGTH:
    dataout[count_element_arr] = datain;
    count_element_arr++;
    if(count_element_arr == FSM_STATE_CHANGE_VALUE_DATA){
      length_fsm = Convert_bytes_into_uint16_t(dataout[4], dataout[5]);
      fsm_state = FSM_STATE_DATA; 
    }
    break;
  case FSM_STATE_DATA:
    dataout[count_element_arr] = datain;
    count_element_arr++;
    if(count_element_arr == 7){
      if(dataout[6] != PORT_A && dataout[6] != PORT_B){
        Clear_All_State_Fsm();        
      }
    }
    else if(count_element_arr == 8){
      if( dataout[7] != SENSOR_LUX && dataout[7] != SENSOR_RTC && dataout[7] != SENSOR_NTC && dataout[7] != SENSOR_RES )
      {
        Clear_All_State_Fsm();
      }
    }
    switch(type_mess_fsm){
      case TYPE_MESSAGE_SET_PORT:
        fsm_state = FSM_STATE_END;
        break;
      case TYPE_MESSAGE_CONFIRM_PORT:
        if(count_element_arr == FSM_STATE_CHANGE_VALUE_END_TYPE_SENSOR){
          fsm_state = FSM_STATE_END;          
        }
        break;
      case TYPE_MESSAGE_ASK_DATA:
        if(count_element_arr == FSM_STATE_CHANGE_VALUE_END_TYPE_SENSOR){
          fsm_state = FSM_STATE_END;          
        }
        break;
      case TYPE_MESSAGE_RESPOND_DATA:
        if(count_element_arr > FSM_STATE_CHANGE_VALUE_END_TYPE_SENSOR){
          switch(dataout[FSM_STATE_CHANGE_VALUE_END_TYPE_SENSOR - 1]){
            case SENSOR_LUX:
              if(count_element_arr == FSM_STATE_CHANGE_VALUE_END_DATA_LUX){
                fsm_state = FSM_STATE_END;
              }
              break;
            case SENSOR_RES:
              if(count_element_arr == FSM_STATE_CHANGE_VALUE_END_DATA_RES){
                fsm_state = FSM_STATE_END;
              }
              break;
          }
        }
        break;
    }
    break;
  case FSM_STATE_END:
    dataout[count_element_arr] = datain;
    count_element_arr++;
    if(count_element_arr == (length_fsm + LENGTH_DEFAULT)){
      uint16_t Check_Sum_From_Arr = Convert_bytes_into_uint16_t( dataout[count_element_arr - 2], dataout[count_element_arr - 1]);
      uint16_t Check_Sum_Fsm = Check_Sum( dataout, count_element_arr - 2);
      if( Check_Sum_From_Arr == Check_Sum_Fsm)
      {
          flag_fsm_new_message = 1;
      }
      else 
      {
          Clear_All_State_Fsm();
      }
    }
    break;
  }
}
uint16_t Message_Frame_Response_RTC(port_e datain_port, type_Sensor_e datain_sensor, uint8_t *datain_value, uint8_t *dataout)
{
    uint8_t *data_temp;
    uint16_t count_response_data = 0;
    frame_Message_t frame_response;
    frame_response.StartFrame = START_BYTE;
    frame_response.TypeMessage = TYPE_MESSAGE_RESPOND_DATA;
    frame_response.Length = LENGTH_MESSAGE_RESPOND_RTC;
    frame_response.Data[0] = datain_port;
    frame_response.Data[1] = datain_sensor;
    for (int i = 0; i < 8; i++)
    {
        frame_response.Data[i + 2] = datain_value[i];
    }
    data_temp = (uint8_t *)&frame_response;
    frame_response.CheckFrame = Check_Sum(data_temp, (LENGTH_DEFAULT + frame_response.Length - 2));
    for (count_response_data = 0; count_response_data < (LENGTH_DEFAULT + frame_response.Length - 2); count_response_data++)
    {
        dataout[count_response_data] = data_temp[count_response_data];
    }
    dataout[count_response_data] = (frame_response.CheckFrame & 0xff);
    dataout[count_response_data + 1] = ((frame_response.CheckFrame >> 8) & 0xff);
    count_response_data += 2;

    // for(uint8_t i = 0;i < count_response_data;i++){
    //   printf("0x%x\n", dataout[i]);
    // }
    return count_response_data;
}
uint16_t Message_Frame_Response_NTC(port_e datain_port, type_Sensor_e datain_sensor, float datain_value, uint8_t *dataout)
{

    uint8_t *data_convert = Convert_float_to_bytes(datain_value);
    uint8_t *data_temp;
    uint16_t count_response_data = 0;
    frame_Message_t frame_response;
    frame_response.StartFrame = START_BYTE;
    frame_response.TypeMessage = TYPE_MESSAGE_RESPOND_DATA;
    frame_response.Length = LENGTH_MESSAGE_RESPOND_NTC;
    frame_response.Data[0] = datain_port;
    frame_response.Data[1] = datain_sensor;
    for (int i = 0; i < 4; i++)
    {
        frame_response.Data[i + 2] = data_convert[i];
    }
    data_temp = (uint8_t *)&frame_response;
    frame_response.CheckFrame = Check_Sum(data_temp, (LENGTH_DEFAULT + frame_response.Length - 2));
    for (count_response_data = 0; count_response_data < (LENGTH_DEFAULT + frame_response.Length - 2); count_response_data++)
    {
        dataout[count_response_data] = data_temp[count_response_data];
    }

    dataout[count_response_data] = (frame_response.CheckFrame & 0xff);
    dataout[count_response_data + 1] = ((frame_response.CheckFrame >> 8) & 0xff);
    count_response_data += 2;

    return count_response_data;
}