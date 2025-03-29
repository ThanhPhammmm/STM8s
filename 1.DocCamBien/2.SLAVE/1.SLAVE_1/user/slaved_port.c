#include <slaved_port.h>
#include <LUX.h>
#include <RES.h>

uint8_t array_confirm_port[10];

float dummy_value1 = 0.0;

uint16_t token_value1 = 0;

float dummy_value2 = 99.0;

uint16_t token_value2 = 99;

uint8_t arr_data_temp[7] = {0x03, 0x02, 0x19, 0x04, 0x07, 0x00, 0x00};

uint8_t requestData_LUX[14];
uint8_t requestData_RES[12];
uint8_t requestData_RTC[17];
uint8_t requestData_NTC[14];

void Send_Ack_Slave1_Sensor_Lux(){
  uint8_t length = Message_Frame_Create_Confirm_Slave(SLAVE_1, SENSOR_LUX, array_confirm_port);
  
  //UART_Send_Array(array_confirm_port, length);
  UART_Send_Array_IT(array_confirm_port, length);
}

void Send_Ack_Slave1_Sensor_RES(){
  uint8_t length = Message_Frame_Create_Confirm_Slave(SLAVE_1, SENSOR_RES, array_confirm_port);
  
  //UART_Send_Array(array_confirm_port, length);
  UART_Send_Array_IT(array_confirm_port, length);
}

void Send_Ack_Slave2_Sensor_RTC(){
  uint8_t length = Message_Frame_Create_Confirm_Slave(SLAVE_2, SENSOR_RTC, array_confirm_port);
  
  //UART_Send_Array(array_confirm_port, length);
  UART_Send_Array_IT(array_confirm_port, length);
}

void Send_Ack_Slave2_Sensor_NTC(){
  uint8_t length = Message_Frame_Create_Confirm_Slave(SLAVE_2, SENSOR_NTC, array_confirm_port);
  
  //UART_Send_Array(array_confirm_port, length);
  UART_Send_Array_IT(array_confirm_port, length);
}

uint8_t Wait_For_Set_Slave_Slave_1(){
  uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
  uint8_t name_slave = Get_Slave(detected_frame);
  if ((name_slave == SLAVE_1) && (type_message == TYPE_MESSAGE_SET_SLAVE)) {
      return 1; // Acknowledged
  }
  return 0; // No response
}
uint8_t Wait_For_Set_Slave_Slave_2(){
  uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
  uint8_t name_slave = Get_Slave(detected_frame);
  if ((name_slave == SLAVE_2) && (type_message == TYPE_MESSAGE_SET_SLAVE)) {
      return 1; // Acknowledged
  }
  return 0; // No response
}

uint8_t Wait_For_Ask_Data_Slave1_SensorLux(){
  uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
  uint8_t name_port = Get_Slave(detected_frame);
  uint8_t name_sensor = Get_Sensor(detected_frame);
  if ((name_port == SLAVE_1) && (name_sensor == SENSOR_LUX) && (type_message == TYPE_MESSAGE_ASK_DATA)) {
    return 1; // Acknowledged
  }
  return 0; // No response
}

uint8_t Wait_For_Ask_Data_Slave1_SensorRES(){
  uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
  uint8_t name_port = Get_Slave(detected_frame);
  uint8_t name_sensor = Get_Sensor(detected_frame);
  if ((name_port == SLAVE_1) && (name_sensor == SENSOR_RES) && (type_message == TYPE_MESSAGE_ASK_DATA)) {
    return 1; // Acknowledged
  }
  return 0; // No response
}

uint8_t Wait_For_Ask_Data_Slave2_SensorRTC(){
  uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
  uint8_t name_port = Get_Slave(detected_frame);
  uint8_t name_sensor = Get_Sensor(detected_frame);
  if ((name_port == SLAVE_2) && (name_sensor == SENSOR_RTC) && (type_message == TYPE_MESSAGE_ASK_DATA)) {
    return 1; // Acknowledged
  }
  return 0; // No response
}
uint8_t Wait_For_Ask_Data_Slave2_SensorNTC(){
  uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
  uint8_t name_port = Get_Slave(detected_frame);
  uint8_t name_sensor = Get_Sensor(detected_frame);
  if ((name_port == SLAVE_2) && (name_sensor == SENSOR_NTC) && (type_message == TYPE_MESSAGE_ASK_DATA)) {
    return 1; // Acknowledged
  }
  return 0; // No response
}

void Send_Sensor_Data_LUX_Slave1() {
  BH1750_Write(0x10);  // Start high-resolution continuous mode
  float lux = BH1750_ReadLight();
  uint8_t length = Message_Frame_Response_LUX(SLAVE_1, SENSOR_LUX, lux, requestData_LUX);
  
  //UART_Send_Array(requestData_LUX, length);
  UART_Send_Array_IT(requestData_LUX, length);
}
void Send_Sensor_Data_RES_Slave1() {
  uint16_t adc_value = Read_ADC(); // Read ADC value from PD2 (AIN3)
  uint8_t length = Message_Frame_Response_RES(SLAVE_1, SENSOR_RES, adc_value, requestData_RES);
  //UART_Send_Array(requestData_RES, length);
  UART_Send_Array_IT(requestData_RES, length);
}

//void Send_Sensor_Data_LUX_Slave2() {
//  uint8_t length = Message_Frame_Response_LUX(SLAVE_2, SENSOR_LUX, dummy_value2, requestData_LUX);
//  dummy_value2--;
//  if(dummy_value2 == -1){
//    dummy_value2 = 99.0;
//  }
//  
//  //UART_Send_Array(requestData_LUX, length);
//  UART_Send_Array(requestData_LUX, length);
//}

//void Send_Sensor_Data_RES_Slave2() {
//  uint8_t length = Message_Frame_Response_RES(SLAVE_2, SENSOR_RES, token_value2 , requestData_RES);
//  token_value2--;
//  if(token_value2 == -1){
//    token_value2 = 99;
//  }
//  //UART_Send_Array(requestData_RES, length);
//  UART_Send_Array(requestData_RES, length);
//}

void Send_Sensor_Data_RTC_Slave2(){
  uint8_t length = Message_Frame_Response_RTC(SLAVE_2, SENSOR_RTC, arr_data_temp, requestData_RTC);
    // Increment seconds
  arr_data_temp[6]++;  
  if (arr_data_temp[6] >= 60) {  
      arr_data_temp[6] = 0;  
      arr_data_temp[5]++;  // Increment minutes
      
      if (arr_data_temp[5] >= 60) {  
          arr_data_temp[5] = 0;  
          arr_data_temp[4]++;  // Increment hours
          
          if (arr_data_temp[4] >= 24) {  
              arr_data_temp[4] = 0;  
              arr_data_temp[2]++;  // Increment day
              
              // Check for month and leap year
              uint8_t month = arr_data_temp[1];  
              uint8_t year = arr_data_temp[3] + 2000;  // Assuming year is stored as offset from 2000
              
              // Days in each month (adjust for leap year)
              uint8_t days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  
              if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))  
                  days_in_month[2] = 29;  // Leap year February
              
              if (arr_data_temp[2] > days_in_month[month]) {  
                  arr_data_temp[2] = 1;  
                  arr_data_temp[1]++;  // Increment month
                  
                  if (arr_data_temp[1] > 12) {  
                      arr_data_temp[1] = 1;  
                      arr_data_temp[3]++;  // Increment year  
                  }  
              }  
          }  
      }  
  }
  //UART_Send_Array(requestData_RTC, length);
  UART_Send_Array_IT(requestData_RTC, length);
}
void Send_Sensor_Data_NTC_Slave2() {
  uint8_t length = Message_Frame_Response_NTC(SLAVE_2, SENSOR_NTC, dummy_value2, requestData_NTC);
  dummy_value2--;
  if(dummy_value2 == -1){
    dummy_value2 = 99.0;
  }
  
  //UART_Send_Array(requestData_LUX, length);
  UART_Send_Array_IT(requestData_NTC, length);
}
