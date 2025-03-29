#ifndef __FSM_H
#define __FSM_H

#ifdef __cplusplus
    extern "C" {
#endif
      
#include <stm8s.h>
#include <CreateFrame.h>
      
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

	FSM_STATE_CHANGE_VALUE_END_SLAVE 		= ( 2 + LENGTH_MESSAGE_SET_SLAVE),
	FSM_STATE_CHANGE_VALUE_END_TYPE_SENSOR 		= ( 2 + LENGTH_MESSAGE_CONFIRM),
	FSM_STATE_CHANGE_VALUE_END_DATA_LUX 		= ( 2 + LENGTH_MESSAGE_RESPOND_LUX),
	FSM_STATE_CHANGE_VALUE_END_DATA_RTC 		= ( 2 + LENGTH_MESSAGE_RESPOND_RTC),
	FSM_STATE_CHANGE_VALUE_END_DATA_NTC 		= ( 2 + LENGTH_MESSAGE_RESPOND_NTC),
	FSM_STATE_CHANGE_VALUE_END_DATA_RES 		= ( 2 + LENGTH_MESSAGE_RESPOND_RES),
}fsm_Value_Next_Step_e;

extern uint8_t count_array_receive;

void Clear_All_State_Fsm(void);
void FSM_Get_Message(uint8_t datain, uint8_t *dataout);
uint8_t Check_Fsm_Flag_New_Message();

#ifdef __cplusplus
}
#endif

#endif