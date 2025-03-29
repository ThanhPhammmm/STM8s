#include <slaved_port.h>
#include <NTC.h>
#include <RTC.h>

uint8_t array_confirm_port[10];

float dummy_value1 = 0.0;

uint16_t token_value1 = 0;

//Day-Month-Date-Year-Hour-Minute-Second
uint8_t RTC_buffer[7];

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
  uint8_t length = Message_Frame_Response_LUX(SLAVE_1, SENSOR_LUX, dummy_value1, requestData_LUX);
  dummy_value1++;
  if(dummy_value1 > 99){
    dummy_value1 = 0.00;
  }
  //UART_Send_Array(requestData_LUX, length);
  UART_Send_Array_IT(requestData_LUX, length);
}
void Send_Sensor_Data_RES_Slave1() {
  uint8_t length = Message_Frame_Response_RES(SLAVE_1, SENSOR_RES, token_value1, requestData_RES);
  token_value1++;
  if(token_value1 > 99){
    token_value1 = 0;
  }
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
  uint8_t length = Message_Frame_Response_RTC(SLAVE_2, SENSOR_RTC, RTC_buffer, requestData_RTC);
  //UART_Send_Array(requestData_RTC, length);
  UART_Send_Array_IT(requestData_RTC, length);
}
void Send_Sensor_Data_NTC_Slave2() {
  float temperature = DHT11_ReadTemperature();
  uint8_t length = Message_Frame_Response_NTC(SLAVE_2, SENSOR_NTC, temperature, requestData_NTC);
  
  //UART_Send_Array(requestData_LUX, length);
  UART_Send_Array_IT(requestData_NTC, length);
}
