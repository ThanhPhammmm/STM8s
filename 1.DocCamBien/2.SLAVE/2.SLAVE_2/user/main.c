#include <stm8s.h>
#include <CreateFrame.h>
#include <button.h>
#include <uart.h>
#include <i2c.h>
#include <slaved_port.h>
#include <tim.h>
#include <system.h>
#include <lcd16x2.h>
#include <NTC.h>
#include <RTC.h>

void assert_failed(uint8_t *file, uint32_t line) {}

extern uint8_t array_receive[20];

frame_Message_t detected_frame;

//SLAVE2_RTC
#define slave2_sensorRTC_WaitingSetPort         10
#define slave2_sensorRTC_Pending                11
#define slave2_sensorRTC_Checking               12
#define slave2_sensorRTC_WaitingAsk             13
#define slave2_sensorRTC_Switching              14

//SLAVE2_NTC
#define slave2_sensorNTC_WaitingSetPort         15
#define slave2_sensorNTC_Pending                16
#define slave2_sensorNTC_Checking               17
#define slave2_sensorNTC_WaitingAsk             18
#define slave2_sensorNTC_Switching              19

uint8_t slave2_rtc = 1;
uint8_t slave2_ntc = 1;

volatile uint8_t slaveX_sensorX = slave2_sensorRTC_WaitingSetPort;

uint8_t receive_flag = 1;

RTC_Time_t current_time;
RTC_Date_t current_date;

INTERRUPT_HANDLER(TIM1_UPD_OVF_IRQHandler, 11)
{
  Get_Current_Time(&current_time);
  Get_Current_Date(&current_date);
        TIM1_CLEAR_IT_PENDING(UPDATE_EVENT);

}

int main(void){
  CLK_HSI_16Mhz_config();
  UART_Configuration(9600);
  I2c_Init();
  //Flash_Erase(EEPROM_START, EEPROM_SIZE);
  
  //Configure PD3 as output (Onboard LED);
//  GPIOB->DDR |= (1 << 5);  // Set PD3 as output
//  GPIOB->CR1 |= (1 << 5);  // Push-pull mode
  TIM2_Init();
  DHT11_Init();
  DS1307_Init();

  current_time.minutes = 0;
  current_time.hours = 0;
  current_time.seconds = 0;
  current_time.time_format = ALL_DAY_TYPE;

  current_date.date = MONDAY;
  current_date.day = 1;
  current_date.month = 1;
  current_date.year = 4;
  
  Set_Current_Time(&current_time);
  Set_Current_Date(&current_date);
  //GPIO_Init(DHT11_PORT, DHT11_PIN, GPIO_MODE_IN_FL_NO_IT);
  //TIM1_BaseInit(15999, COUNT_UP, 600 - 1, 0);  // 1s period
  while(1){
    
    //UART_Time_Out();
//    Get_Current_Time(&current_time);
//    Get_Current_Date(&current_date);
    switch (slaveX_sensorX) {
      
// --- Slave 2 RTC Handling ---
        case slave2_sensorRTC_WaitingSetPort:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Set_Slave_Slave_2()) {
                    slaveX_sensorX = slave2_sensorRTC_Pending;
                } 
                else {
                    receive_flag = 1;
                }
            }
            break;

        case slave2_sensorRTC_Pending:
            Send_Ack_Slave2_Sensor_RTC();
            receive_flag = 1;
            slaveX_sensorX = slave2_sensorRTC_Checking;
            break;

        case slave2_sensorRTC_Checking:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Ask_Data_Slave2_SensorRTC()) {
                    slaveX_sensorX = slave2_sensorRTC_WaitingAsk;
                    receive_flag = 1;
                } 
                else {
                    receive_flag = 1;
                }
            } 
            else {
                slaveX_sensorX = slave2_sensorRTC_Pending;
            }
            break;

        case slave2_sensorRTC_WaitingAsk:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Ask_Data_Slave2_SensorRTC()) {
//                    Get_Current_Time(&current_time);
//                    Get_Current_Date(&current_date);
                    Send_Sensor_Data_RTC_Slave2();
                    receive_flag = 1;
                    slaveX_sensorX = slave2_sensorRTC_Switching;
                }
                else{
                    receive_flag = 1;                  
                }
            }
            break;

        case slave2_sensorRTC_Switching:
            if (Wait_For_UART_Frame_1s()) {
                uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
                if (type_message == TYPE_MESSAGE_ASK_DATA) {
                    slaveX_sensorX = slave2_sensorRTC_WaitingAsk;
                } 
                else if (type_message == TYPE_MESSAGE_SET_SLAVE) {
                    slaveX_sensorX = slave2_sensorNTC_WaitingSetPort;
                }
                receive_flag = 1;
            }
            break;

        // --- Slave 2 NTC Handling ---
        case slave2_sensorNTC_WaitingSetPort:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Set_Slave_Slave_2()) {
                    slaveX_sensorX = slave2_sensorNTC_Pending;
                } else {
                    receive_flag = 1;
                }
            }
            break;

        case slave2_sensorNTC_Pending:
            Send_Ack_Slave2_Sensor_NTC();
            receive_flag = 1;
            slaveX_sensorX = slave2_sensorNTC_Checking;
            break;

        case slave2_sensorNTC_Checking:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Ask_Data_Slave2_SensorNTC()) {
                    slaveX_sensorX = slave2_sensorNTC_WaitingAsk;
                    receive_flag = 1;
                } else {
                    receive_flag = 1;
                }
            } else {
                slaveX_sensorX = slave2_sensorNTC_Pending;
            }
            break;

        case slave2_sensorNTC_WaitingAsk:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Ask_Data_Slave2_SensorNTC()) {
                    Send_Sensor_Data_NTC_Slave2();
                    receive_flag = 1;
                    slaveX_sensorX = slave2_sensorNTC_Switching;
                    //GPIOB->ODR ^= (1 << 5);  // Toggle bit 5
                }
                else{
                    receive_flag = 1;                 
                }
            }
            break;

        case slave2_sensorNTC_Switching:
            if (Wait_For_UART_Frame_1s()) {
                uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
                if (type_message == TYPE_MESSAGE_ASK_DATA) {
                    slaveX_sensorX = slave2_sensorNTC_WaitingAsk;
                } else if (type_message == TYPE_MESSAGE_SET_SLAVE) {
                    slaveX_sensorX = slave2_sensorRTC_WaitingSetPort;
                }
                receive_flag = 1;
            }
            break;
    }

  }
}