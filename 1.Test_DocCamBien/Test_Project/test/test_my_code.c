// #ifdef TEST

#include "unity.h"
#include "my_code.h"

//ceedling
// void test_create_message_set_port()
// {
//     uint8_t arr_test[9] = {0x55, 0xAA, 0x01, 0x00, 0x05, 0x00, 0x01, 0xEA, 0x0E};
//     printf("sum arr test: %x", Check_Sum(arr_test, 7));
//     uint8_t arr_out[15];

//     uint16_t length = Message_Frame_Create_Set_Port(PORT_A, arr_out);
//     for(int i = 0; i < length; i++)
//     {
//         TEST_ASSERT_EQUAL_UINT8(arr_out[i], arr_test[i]);
//     }
// }
// void test_Frame_Sensor_Confirm_Port(void)
// {
//     uint8_t count_test_arr = 0;
//     frame_Message_t frame_sensor_confirm_port, frame_sensor_confirm_port_detect;

//     uint8_t arr_frame_confirm_port[50];

//     uint8_t arr_frame_test[LENGTH_DEFAULT + LENGTH_MESSAGE_CONFIRM] = {0x55, 0xAA, 0x02, 0x00, 0x06, 0x00, 0x02, 0x01, 0x4E, 0xC8};

//     uint8_t length_frame = Message_Frame_Create_Confirm_Port(PORT_B, SENSOR_LUX, arr_frame_confirm_port);

//     printf("/*-------------------------------------------------------------------------------------------------------*/");
//     printf("\nStart test confirm\n");
//     printf("length_frame = %d\n", length_frame);

//     for (count_test_arr = 0; count_test_arr < length_frame; count_test_arr++)
//     {
//         printf("arr_frame_confirm_port[%d] = %x\n", count_test_arr, arr_frame_confirm_port[count_test_arr]);
//         TEST_ASSERT_EQUAL_UINT8(arr_frame_test[count_test_arr], arr_frame_confirm_port[count_test_arr]);
//     }
// }
// void test_Frame_Ask_Data_Sensor(void){
//     uint8_t count_test_arr = 0;
//     frame_Message_t frame_Ask_data;
//     uint8_t arr_frame_Ask_data[50];
//     uint8_t arr_frame_test[LENGTH_DEFAULT + LENGTH_MESSAGE_ASK_DATA] = {0x55, 0xAA, 0x03, 0x00, 0x06, 0x00, 0x02, 0x01, 0x4F, 0x19};
//     uint8_t length_frame = Message_Frame_Create_Ask_Data_Sensor(PORT_B, SENSOR_LUX, arr_frame_Ask_data);
//     printf("/*-------------------------------------------------------------------------------------------------------*/");
//     printf("\nStart test confirm\n");
//     printf("length_frame = %d\n", length_frame);
//     for (count_test_arr = 0; count_test_arr < length_frame; count_test_arr++)
//     {
//         printf("arr_frame_Ask_data[%d] = %x\n", count_test_arr, arr_frame_Ask_data[count_test_arr]);
//         TEST_ASSERT_EQUAL_UINT8(arr_frame_test[count_test_arr], arr_frame_Ask_data[count_test_arr]);
//     }
// }
// void test_Frame_Response_Data_Sensor(void){
//     uint8_t count_test_arr = 0;
//     frame_Message_t frame_Response_data;
//     uint8_t arr_Response_data[50];
//     uint8_t arr_frame_test[LENGTH_DEFAULT + LENGTH_MESSAGE_RESPOND_LUX] = {0x55, 0xAA, 0x04, 0x00, 0x0A, 0x00, 0x02, 0x01, 0x19, 0x04, 0x9e, 0x3f, 0x18, 0x71};
//     uint8_t length_frame = Message_Frame_Response_LUX(PORT_B, SENSOR_LUX, 1.2345, arr_Response_data);
//     printf("/*-------------------------------------------------------------------------------------------------------*/");
//     printf("\nStart test confirm\n");
//     printf("length_frame = %d\n", length_frame);
//     for (count_test_arr = 0; count_test_arr < length_frame; count_test_arr++)
//     {
//         printf("arr_Response_data[%d] = %x\n", count_test_arr, arr_Response_data[count_test_arr]);
//         TEST_ASSERT_EQUAL_UINT8(arr_frame_test[count_test_arr], arr_Response_data[count_test_arr]);
//     }
// }
// void test_convert(void){
//     printf("0x%x\n", START_BYTE);
//     uint16_t rs = Convert_bytes_into_uint16_t(0x55, 0xAA);
//     printf("%d\n",rs);
//     TEST_ASSERT_EQUAL_UINT16(rs, 43605);
// }
// void test_detect(void){
//     uint8_t tmp[14] = {0x55, 0xAA, 0x02, 0x00, 0x08, 0x00, 0x02, 0x01, 0x3F, 0x9E, 0x04, 0x19, 0x4E, 0xC8};
//     frame_Message_t dataout;
//     Message_Detect_TypeMessage(tmp, &dataout);
// }
// void test_convert(void){
//     uint32_t tmp = Convert_bytes_into_int(0x67, 0x45, 0x23, 0x01);
//     printf("%d\n", tmp);
// }
// void test_fsm_respond_data_LUX()
// {
//     printf("---------------------FSM REPOND FROM LUX----------------------------\n");
//     /* 
//         type mess: 04
//         length: 10
//         port: 02
//         type sensor: SENSOR_LUX
//         347.6 = 0x43adcccd
//     */
//     uint8_t arr_test[14] = {0x55, 0xAA, 0x04, 0x00, 0x0A, 0x00, 0x02, SENSOR_LUX, 0xCD, 0xCC, 0xAD, 0x43, 0xB5, 0x6E};
//     printf("sum arr test REPOND FROM LUX: %x\n", Check_Sum(arr_test, 12));
//     uint8_t arr_out_fsm[30];
//     uint16_t length = 14;
//     uint8_t check = 0;
    
//     for ( uint8_t i = 0; i < length; i++)
//     {
//         FSM_Get_Message(arr_test[i], arr_out_fsm);
//         printf("0x%x 0x%x\n", arr_out_fsm[i], arr_test[i]);
//     }
//     TEST_ASSERT_EQUAL_UINT8_ARRAY(arr_out_fsm, arr_test, length);
// }

// uint8_t arr_data_temp[7] = {0x03, 0x02, 0x19, 0x04, 0x07, 0x00, 0x00};
// uint8_t arr_frame_test[LENGTH_DEFAULT + LENGTH_MESSAGE_RESPOND_RTC] = {0x55, 0xAA, 0x04, 0x00, 0x0D, 0x00, 0x02, 0x02, 0x03, 0x02, 0x19, 0x04, 0x07, 0x00, 0x00, 0x4d, 0x6a};

// void test_respond_data_RTC(){
//     uint8_t arr_Response_data[50];
//     uint16_t length_data = Message_Frame_Response_RTC(PORT_B, SENSOR_RTC, arr_data_temp, arr_Response_data);
//     for(uint8_t i = 0;i < length_data;i++){
//         printf("0x%x 0x%x\n", arr_Response_data[i], arr_frame_test[i]);
//         TEST_ASSERT_EQUAL_UINT8(arr_Response_data[i], arr_frame_test[i]);
//     }
// }

uint8_t arr_frame_test[LENGTH_DEFAULT + FSM_STATE_CHANGE_VALUE_END_DATA_NTC] = {0x55, 0xAA, 0x04, 0x00, 0x0a, 0x00, 0x02, 0x02, 0x00, 0x00, 0x48, 0x42, 0x84, 0xad};

float dummy_value = 50.00;
void test_respond_data_NTC(){
    uint8_t arr_Respond_data[50];
    uint16_t length_data = Message_Frame_Response_NTC(PORT_B, SENSOR_RTC, dummy_value, arr_Respond_data);
    for(uint8_t i = 0;i < length_data;i++){
        printf("0x%x\n", arr_Respond_data[i]);
        TEST_ASSERT_EQUAL_UINT8(arr_Respond_data[i], arr_frame_test[i]);
    }
}
// #endif // TEST

