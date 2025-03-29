#include <port.h>

uint8_t array_set_slave[10];
uint8_t requestFrame[10];  // Template request

void Request_Slave(slave_e slave_id) {
    uint8_t length = Message_Frame_Create_Set_Slave(slave_id, array_set_slave);
    
    //UART_Send_Array(array_set_port, length);
    UART_Send_Array_IT(array_set_slave, length);
}
uint8_t Wait_For_Ack(slave_e slave_id, type_Sensor_e sensor_type) {
    uint8_t type = Message_Detect_TypeMessage(array_receive, &detected_frame);
    uint8_t received_id = Get_slave(detected_frame);
    uint8_t received_sensor = Get_Sensor(detected_frame);
    if ((received_id == slave_id) && (received_sensor == sensor_type) && (type == TYPE_MESSAGE_CONFIRM_SLAVE)) {
        return 1; // Acknowledged
    }
    return 0; // No response
}
void Request_Slave_Data(slave_e slave_id, type_Sensor_e sensor_type){
    uint8_t length = Message_Frame_Create_Ask_Data_Sensor(slave_id, sensor_type, requestFrame);
    
    //UART_Send_Array(requestFrame, length);
    UART_Send_Array_IT(requestFrame, length);
}
uint8_t Wait_For_Respond(slave_e slave_id, type_Sensor_e sensor_type){
  uint8_t type = Message_Detect_TypeMessage(array_receive, &detected_frame);
  uint8_t name = Get_slave(detected_frame);
  uint8_t sensor = Get_Sensor(detected_frame);
  if((type == TYPE_MESSAGE_RESPOND_DATA) && (name == slave_id) && (sensor == sensor_type)){
    return 1;
  }
  return 0;
}
float Receive_Slave_Data_LUX() {
    float data = Convert_bytes_into_float(array_receive[8], array_receive[9], array_receive[10], array_receive[11]);
    return data;
}
uint16_t Receive_Slave_Data_RES() {
    uint16_t data = Convert_bytes_into_uint16_t(array_receive[8], array_receive[9]);
    return data;
}
void Receive_Slave_Data_RTC(uint8_t *temp){
  for(uint8_t i = 0;i < 7;i++){
    temp[i] = detected_frame.Data[i + 2];
  }
}
float Receive_Slave_Data_NTC(){
    float data = Convert_bytes_into_float(array_receive[8], array_receive[9], array_receive[10], array_receive[11]);
    return data;
}