#include <stm8s.h>
#include <CreateFrame.h>
#include <button.h>
#include <uart.h>
#include <i2c.h>
#include <port.h>
#include <tim.h>
#include <system.h>
#include <lcd16x2.h>

void assert_failed(uint8_t *file, uint32_t line) {}

frame_Message_t detected_frame;

typein_t ButtonA1 = {.status_old = 1, .pin = 1};
typein_t ButtonA2 = {.status_old = 1, .pin = 2};
typein_t ButtonA3 = {.status_old = 1, .pin = 3};

float data_sensor1 = 0;
uint16_t data_sensor2 = 0;
uint8_t data_sensor3[7];
float data_sensor4 = 0;

uint8_t receive_confirm_slave1_sensorLux = 0;
uint8_t receive_confirm_slave1_sensorRES = 0;

uint8_t receive_data_slave1_sensorLux = 0;
uint8_t receive_data_slave1_sensorRES = 0;

uint8_t receive_confirm_slave2_sensorRTC = 0;
uint8_t receive_confirm_slave2_sensorNTC = 0;

uint8_t receive_data_slave2_sensorRTC = 0;
uint8_t receive_data_slave2_sensorNTC = 0;

uint8_t slave1_lux = 1;
uint8_t slave1_res = 1;
uint8_t slave2_rtc = 1;
uint8_t slave2_ntc = 1;

volatile uint8_t receive_flag = 0;

volatile uint8_t button_flag = 0; // Flag to process button outside ISR
volatile uint8_t lcd_flag = 0;

volatile uint8_t start_flag = 0;

INTERRUPT_HANDLER(TIM1_UPD_OVF_IRQHandler, 11)
{
    TIM1_CLEAR_IT_PENDING(UPDATE_EVENT);
    TIM1_ITConfig(TIM1_IT_UPDATE, DISABLE); // Disable timer interrupt temporarily

    button_flag = 1; // Set flag to process button in main loop
}
INTERRUPT_HANDLER(TIM2_UPD_OVF_IRQHandler, 13)
{
    TIM2_CLEAR_IT_PENDING(UPDATE_EVENT);
    //TIM2_ITConfig(TIM2_IT_UPDATE, DISABLE); // Disable timer interrupt temporarily
    lcd_flag = 1;
}
int main(void){
  CLK_HSI_16Mhz_config();
  
//  GPIOB->DDR |= (1 << 5);  // Set PD3 as output
//  GPIOB->CR1 |= (1 << 5);  // Push-pull mode
  
  I2c_Init();
  UART_Configuration(9600);
  LCD_Init();
  
  while(Gpio_Read_Input_Port_D(4) == 1);
  LCD_Set_Cursor(1,1);
  LCD_SendString("SLAVE 1 is ready");
  while(Gpio_Read_Input_Port_D(4) == 1);
  LCD_Set_Cursor(2,1);
  LCD_SendString("SLAVE 2 is ready");
  
  Delay_ms(2000);

  LCD_SendCommand(0x01);
  
  Button_Init();
  
// TIM1 for Button Press Handling (every 10ms)
  TIM1_BaseInit(159, COUNT_UP, 1, 0);  // 10ms

// TIM2 for LCD Display Update (every 1 second)
  TIM2_BaseInit(15999, COUNT_UP, 43 - 1); // 1 second interval
  enableInterrupts();
  
  volatile slave_state_e Master_works_with_slaveX_sensorX = Sending_SetSlave_to_slave1_sensorLux;
  
  while(1){
    if (button_flag)
    {
      button_flag = 0;  // Clear flag
      Button_Run(&ButtonA3);
      Button_Run(&ButtonA2);
      TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); // Enable timer interrupt temporarily
    }
    if(lcd_flag == 1){
      Send_Data_To_LCD(data_sensor1, data_sensor2, data_sensor3, data_sensor4);
      lcd_flag = 0;
      //TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE); // Enable timer interrupt temporarily
    }
    if(start_flag == 1){
      switch (Master_works_with_slaveX_sensorX) {
        //slave1_sensorLux
        case Sending_SetSlave_to_slave1_sensorLux:

          if(slave1_lux == 1){
            Request_Slave(SLAVE_1);
            receive_flag = 1;
            Master_works_with_slaveX_sensorX = Waiting_ConfirmSlave_from_slave1_sensorLux;
            slave1_lux = 0;
          }
          break;

        case Waiting_ConfirmSlave_from_slave1_sensorLux:

          if (Wait_For_UART_Frame_1s()) {
            receive_confirm_slave1_sensorLux = 1;
            if (Wait_For_Ack(SLAVE_1, SENSOR_LUX)) {
              Master_works_with_slaveX_sensorX = Sending_AskData_to_slave1_sensorLux;
            } 
            else {
              receive_flag = 1;
            }
          } 
          else if (receive_confirm_slave1_sensorLux == 0) {
            Master_works_with_slaveX_sensorX = Sending_SetSlave_to_slave1_sensorLux;
            slave1_lux = 1;
          }
          break;

        case Sending_AskData_to_slave1_sensorLux:

          Request_Slave_Data(SLAVE_1, SENSOR_LUX);
          receive_flag = 1;
          Master_works_with_slaveX_sensorX = Waiting_RespondData_from_slave1_sensorLux;
          break;

        case Waiting_RespondData_from_slave1_sensorLux:

          if (Wait_For_UART_Frame_1s()) {
            receive_data_slave1_sensorLux = 1;
            if (Wait_For_Respond(SLAVE_1, SENSOR_LUX)) {
              data_sensor1 = Receive_Slave_Data_LUX();
              //Send_Data_To_LCD(data_sensor1, data_sensor2, data_sensor3, data_sensor4);
              //Master_works_with_slaveX_sensorX = Sending_SetSlave_to_slave1_sensorRES;
              Master_works_with_slaveX_sensorX = Sending_AskData_to_slave1_sensorRES;
            } 
            else {
              receive_flag = 1;
            }
          } 
          else if (receive_data_slave1_sensorLux == 0) {
            Master_works_with_slaveX_sensorX = Sending_AskData_to_slave1_sensorLux;
          }
          break;
          
        //slave1_sensorRes
        case Sending_AskData_to_slave1_sensorRES:

          Request_Slave_Data(SLAVE_1, SENSOR_RES);
          receive_flag = 1;
          Master_works_with_slaveX_sensorX = Waiting_RespondData_from_slave1_sensorRES;
          break;

        case Waiting_RespondData_from_slave1_sensorRES:

          if (Wait_For_UART_Frame_1s()) {
            receive_data_slave1_sensorRES = 1;
            if (Wait_For_Respond(SLAVE_1, SENSOR_RES)) {
              data_sensor2 = Receive_Slave_Data_RES();
              //Send_Data_To_LCD(data_sensor1, data_sensor2, data_sensor3, data_sensor4);
              Master_works_with_slaveX_sensorX = Sending_SetSlave_to_slave2_sensorRTC;
              //memset(array_receive, 0, sizeof(array_receive));
            } 
            else {
              receive_flag = 1;
            }
          } 
          else if (receive_data_slave1_sensorRES == 0) {
            Master_works_with_slaveX_sensorX = Sending_AskData_to_slave1_sensorRES;
          }
          break;
/*-----------------------------------------------------------------------------------------
          
          
          
          
          
-------------------------------------------------------------------------------------------*/
        //slave2_sensorRTC
        case Sending_SetSlave_to_slave2_sensorRTC:
          if(slave2_rtc == 1){
            Request_Slave(SLAVE_2);
            receive_flag = 1;
            Master_works_with_slaveX_sensorX = Waiting_ConfirmSlave_from_slave2_sensorRTC;
            slave2_rtc = 0;
          }
          break;

        case Waiting_ConfirmSlave_from_slave2_sensorRTC:
          if (Wait_For_UART_Frame_1s()) {
            receive_confirm_slave2_sensorRTC = 1;
            if (Wait_For_Ack(SLAVE_2, SENSOR_RTC)) {
              Master_works_with_slaveX_sensorX = Sending_AskData_to_slave2_sensorRTC;
            } 
            else {
              receive_flag = 1;
            }
          } 
          else if (receive_confirm_slave2_sensorRTC == 0) {
            Master_works_with_slaveX_sensorX = Sending_SetSlave_to_slave2_sensorRTC;
            slave2_rtc = 1;
          }
          break;

        case Sending_AskData_to_slave2_sensorRTC:
          Request_Slave_Data(SLAVE_2, SENSOR_RTC);
          receive_flag = 1;
          Master_works_with_slaveX_sensorX = Waiting_RespondData_from_slave2_sensorRTC;
          break;

        case Waiting_RespondData_from_slave2_sensorRTC:
          if (Wait_For_UART_Frame_1s()) {
            receive_data_slave2_sensorRTC = 1;
            if (Wait_For_Respond(SLAVE_2, SENSOR_RTC)) {
              Receive_Slave_Data_RTC(data_sensor3);
              //Send_Data_To_LCD(data_sensor1, data_sensor2, data_sensor3, data_sensor4);
              //Master_works_with_slaveX_sensorX = Sending_SetSlave_to_slave2_sensorNTC;
              Master_works_with_slaveX_sensorX = Sending_AskData_to_slave2_sensorNTC;
            } 
            else {
              receive_flag = 1;
            }
          } 
          else if (receive_data_slave2_sensorRTC == 0) {
            Master_works_with_slaveX_sensorX = Sending_AskData_to_slave2_sensorRTC;
          }
          break;

        //slave2_sensorNTC
        case Sending_AskData_to_slave2_sensorNTC:
          Request_Slave_Data(SLAVE_2, SENSOR_NTC);
          receive_flag = 1;
          Master_works_with_slaveX_sensorX = Waiting_RespondData_from_slave2_sensorNTC;
          break;

        case Waiting_RespondData_from_slave2_sensorNTC:
          if (Wait_For_UART_Frame_1s()) {
            receive_data_slave2_sensorNTC = 1;
            if (Wait_For_Respond(SLAVE_2, SENSOR_NTC)) {
              data_sensor4 = Receive_Slave_Data_NTC();
              //Send_Data_To_LCD(data_sensor1, data_sensor2, data_sensor3, data_sensor4);
              Master_works_with_slaveX_sensorX = Sending_SetSlave_to_slave1_sensorLux;
              //memset(array_receive, 0, sizeof(array_receive));

              // Reset confirmation and data flags
              receive_confirm_slave1_sensorLux = 0;
              receive_confirm_slave1_sensorRES = 0;
              
              receive_data_slave1_sensorLux = 0;
              receive_data_slave1_sensorRES = 0;
              
              receive_confirm_slave2_sensorRTC = 0;
              receive_confirm_slave2_sensorNTC = 0;
              
              receive_data_slave2_sensorRTC = 0;
              receive_data_slave2_sensorNTC = 0;
              
              slave1_lux = 1;
              slave1_res = 1;
              slave2_rtc = 1;
              slave2_ntc = 1;        
              
              //Send_Data_To_LCD(data_sensor1, data_sensor2, data_sensor3, data_sensor4);
            } 
            else {
              receive_flag = 1;
            }
          } 
          else if (receive_data_slave2_sensorNTC == 0) {
            Master_works_with_slaveX_sensorX = Sending_AskData_to_slave2_sensorNTC;
          }
          break;
      }
    }
  }
}