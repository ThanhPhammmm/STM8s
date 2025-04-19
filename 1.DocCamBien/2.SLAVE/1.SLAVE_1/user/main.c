#include <stm8s.h>
#include <CreateFrame.h>
#include <button.h>
#include <uart.h>
#include <i2c.h>
#include <slaved_port.h>
#include <tim.h>
#include <system.h>
#include <lcd16x2.h>
#include <RES.h>
#include <LUX.h>

void assert_failed(uint8_t* file, uint32_t line){}

extern uint8_t array_receive[20];

frame_Message_t detected_frame;

//SLAVE1_LUX
#define slave1_sensorLux_WaitingSetPort         0
#define slave1_sensorLux_Pending                1
#define slave1_sensorLux_Checking               2
#define slave1_sensorLux_WaitingAsk             3
#define slave1_sensorLux_Switching              4

//SLAVE1_RES
#define slave1_sensorRES_WaitingSetPort         5
#define slave1_sensorRES_Pending                6
#define slave1_sensorRES_Checking               7
#define slave1_sensorRES_WaitingAsk             8
#define slave1_sensorRES_Switching              9

uint8_t slave1_lux = 1;
uint8_t slave1_res = 1;

volatile uint8_t slaveX_sensorX = slave1_sensorLux_WaitingSetPort;

volatile uint8_t receive_flag = 1;
float lux_value = 0;
uint16_t adc_value = 0;

int main(void){
  CLK_HSI_16Mhz_config();
  UART_Configuration(9600);
  //I2C_Init_BH1750();
  I2c_Init();
  ADC_Init();
  //Flash_Erase(EEPROM_START, EEPROM_SIZE);
  
  //Configure PD3 as output (Onboard LED);
//  GPIOB->DDR |= (1 << 5);  // Set PD3 as output
//  GPIOB->CR1 |= (1 << 5);  // Push-pull mode
  
  enableInterrupts();
    
  while(1){
//    BH1750_Write(0x10);  // Start high-resolution continuous mode
//    lux_value = BH1750_ReadLight();
//    adc_value = Read_ADC(); // Read ADC value from PD2 (AIN3)

    //UART_Time_Out();
    switch (slaveX_sensorX) {
      
        // --- Slave 1 LUX Handling ---
        case slave1_sensorLux_WaitingSetPort:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Set_Slave_Slave_1()) {
                    slaveX_sensorX = slave1_sensorLux_Pending;
                } else {
                    receive_flag = 1;
                }
            }
            break;

        case slave1_sensorLux_Pending:
            Send_Ack_Slave1_Sensor_Lux();
            receive_flag = 1;
            slaveX_sensorX = slave1_sensorLux_Checking;
            break;

        case slave1_sensorLux_Checking:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Ask_Data_Slave1_SensorLux()) {
                    slaveX_sensorX = slave1_sensorLux_WaitingAsk;
                    receive_flag = 1;
                } 
                else {
                    receive_flag = 1;
                }
            } 
            else {
                slaveX_sensorX = slave1_sensorLux_Pending;
            }
            break;

        case slave1_sensorLux_WaitingAsk:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Ask_Data_Slave1_SensorLux()) {
                    Send_Sensor_Data_LUX_Slave1();
                    receive_flag = 1;
                    slaveX_sensorX = slave1_sensorLux_Switching;
                } else {
                    receive_flag = 1;
                }
            }
            break;

        case slave1_sensorLux_Switching:
            if (Wait_For_UART_Frame_1s()) {
                uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
                if (type_message == TYPE_MESSAGE_ASK_DATA) {
                    //slaveX_sensorX = slave1_sensorLux_WaitingAsk;
                    slaveX_sensorX = slave1_sensorRES_WaitingAsk;
                } 
                else if (type_message == TYPE_MESSAGE_SET_SLAVE) {
                    //slaveX_sensorX = slave1_sensorRES_WaitingSetPort;
                }
                  receive_flag = 1;
            }
            break;

        // --- Slave 1 RES Handling ---
        case slave1_sensorRES_WaitingSetPort:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Set_Slave_Slave_1()) {
                    slaveX_sensorX = slave1_sensorRES_Pending;
                    slave1_res = 0;
                } else {
                    receive_flag = 1;
                }
            }
            break;

        case slave1_sensorRES_Pending:
            Send_Ack_Slave1_Sensor_RES();
            receive_flag = 1;
            slaveX_sensorX = slave1_sensorRES_Checking;
            break;

        case slave1_sensorRES_Checking:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Ask_Data_Slave1_SensorRES()) {
                    slaveX_sensorX = slave1_sensorRES_WaitingAsk;
                    receive_flag = 1;
                } else {
                    receive_flag = 1;
                }
            } else {
                slaveX_sensorX = slave1_sensorRES_Pending;
            }
            break;

        case slave1_sensorRES_WaitingAsk:
            if (Wait_For_UART_Frame_1s()) {
                if (Wait_For_Ask_Data_Slave1_SensorRES()) {
                    Send_Sensor_Data_RES_Slave1();
                    receive_flag = 1;
                    slaveX_sensorX = slave1_sensorRES_Switching;
                    //GPIOB->ODR ^= (1 << 5);  // Toggle bit 5
                }
            }
            break;

        case slave1_sensorRES_Switching:
            if (Wait_For_UART_Frame_1s()) {
                uint16_t type_message = Message_Detect_TypeMessage(array_receive, &detected_frame);
                if (type_message == TYPE_MESSAGE_ASK_DATA) {
                    slaveX_sensorX = slave1_sensorRES_WaitingAsk;
                } else if (type_message == TYPE_MESSAGE_SET_SLAVE) {
                    slaveX_sensorX = slave1_sensorLux_WaitingSetPort;
                }
                  receive_flag = 1;
            }
            break;

    }

  }
}