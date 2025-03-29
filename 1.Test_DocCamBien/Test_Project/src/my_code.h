#ifndef MY_CODE_H
#define MY_CODE_H

#include <stdint.h>

typedef struct
{
    uint16_t StartFrame;
    uint16_t TypeMessage;
    uint16_t Length; // tính tu Length + Data
    uint8_t  Data[10];
    uint16_t CheckFrame;
} frame_Message_t;
// Gia tri enum
typedef enum
{
    TYPE_MESSAGE_SET_PORT             = 0x0001,
    TYPE_MESSAGE_CONFIRM_PORT         = 0x0002,
    TYPE_MESSAGE_ASK_DATA             = 0x0003,
    TYPE_MESSAGE_RESPOND_DATA         = 0x0004, 
} type_Message_e;
typedef enum
{
	PORT_A = 0x01,
	PORT_B = 0x02,		
} port_e;
typedef enum
{
    SENSOR_LUX      = 0x01,
    SENSOR_RTC      = 0x02,
    SENSOR_NTC      = 0x03,
    SENSOR_RES      = 0x04,
} type_Sensor_e;
#define START_BYTE                   0XAA55 /*Ban tin start */
#define LENGTH_DEFAULT               4      /* StartByte + TypeMsg*/
#define LENGTH_MESSAGE_SET_PORT      5      /* Length_data + Port + Checksum*/
#define LENGTH_MESSAGE_CONFIRM       6      /* Length_data + Port + TypeSensor + Checksum*/
#define LENGTH_MESSAGE_ASK_DATA      6      /* Length_data + Port + TypeSensor + Checksum*/
#define LENGTH_MESSAGE_RESPOND_LUX   10     /* Length_data + Port + TypeSensor + 4 byte Data + Checksum*/
#define LENGTH_MESSAGE_RESPOND_RTC   13     /* Length_data + Port + TypeSensor + 7 byte Data + Checksum*/
#define LENGTH_MESSAGE_RESPOND_NTC   10     /* Length_data + Port + TypeSensor + 4 byte Data + Checksum*/
#define LENGTH_MESSAGE_RESPOND_RES   8      /* Length_data + Port + TypeSensor + 2 byte Data + Checksum*/
#define TRUE 1
#define FALSE 0

typedef enum
{
	FSM_STATE_START_FRAME 	= 0,
	FSM_STATE_TYPE_MESS 	= 1,
	FSM_STATE_LENGTH        = 2,
	FSM_STATE_DATA 	        = 3,
	FSM_STATE_END 		= 4,
}fsm_List_State_e;

typedef enum
{
	FSM_STATE_CHANGE_VALUE_START_FRAME	        = 0,
	FSM_STATE_CHANGE_VALUE_TYPE_MESS 	        = 2,
	FSM_STATE_CHANGE_VALUE_LENGTH 		        = 4,
	FSM_STATE_CHANGE_VALUE_DATA 		        = 6,

	FSM_STATE_CHANGE_VALUE_END_PORT 		= ( 2 + LENGTH_MESSAGE_SET_PORT),
	FSM_STATE_CHANGE_VALUE_END_TYPE_SENSOR 		= ( 2 + LENGTH_MESSAGE_CONFIRM),
	FSM_STATE_CHANGE_VALUE_END_DATA_LUX 		= ( 2 + LENGTH_MESSAGE_RESPOND_LUX),
	FSM_STATE_CHANGE_VALUE_END_DATA_RTC 		= ( 2 + LENGTH_MESSAGE_RESPOND_RTC),
	FSM_STATE_CHANGE_VALUE_END_DATA_NTC 		= ( 2 + LENGTH_MESSAGE_RESPOND_NTC),
	FSM_STATE_CHANGE_VALUE_END_DATA_RES 		= ( 2 + LENGTH_MESSAGE_RESPOND_RES),
}fsm_Value_Next_Step_e;

union Int_to_bytes_format{
  int data;
  unsigned char bytes[4];
};
union Float_to_bytes_format{
  float data;
  unsigned char bytes[4];
};
union Unit16_t_to_bytes_format{
  uint16_t data;
  unsigned char bytes[2];
};
enum STATUS{
  True = 1,
  False = 0
};
enum COLOUR{
  RED,
  BLUE,
  GREEN
};
uint8_t *Convert_int_to_bytes(int n);
uint32_t Convert_bytes_into_int(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);
uint8_t *Convert_float_to_bytes(float n);
float Convert_bytes_into_float(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);
uint8_t *Convert_uint16_t_to_bytes(uint16_t n);
uint16_t Convert_bytes_into_uint16_t(uint8_t data1, uint8_t data2);
uint16_t Check_Sum(uint8_t *buf, uint8_t len);
uint16_t Message_Frame_Create_Set_Port( port_e datain_port, uint8_t *data_out);
uint8_t Message_Frame_Create_Confirm_Port(uint8_t datain_port, uint8_t datain_sensor, uint8_t* dataout);
uint8_t Message_Frame_Create_Ask_Data_Sensor(port_e datain_port, type_Sensor_e datain_sensor, uint8_t* dataout);
uint16_t Message_Frame_Response_LUX(port_e datain_port, type_Sensor_e datain_sensor, float datain_value, uint8_t* dataout);
uint16_t Message_Detect_TypeMessage(uint8_t* datain, frame_Message_t* dataout);
uint8_t Message_Get_Port(frame_Message_t datain_frame);
uint8_t Message_Get_Type_Sensor(frame_Message_t datain_frame);
float Message_Get_Data_Sensor_Lux(frame_Message_t datain_frame);
void FSM_Get_Message(uint8_t datain, uint8_t *dataout);
void Clear_All_State_Fsm(void);
uint16_t Message_Frame_Response_RTC(port_e datain_port, type_Sensor_e datain_sensor, uint8_t *datain_value, uint8_t *dataout);
uint16_t Message_Frame_Response_NTC(port_e datain_port, type_Sensor_e datain_sensor, float datain_value, uint8_t *dataout);

#endif