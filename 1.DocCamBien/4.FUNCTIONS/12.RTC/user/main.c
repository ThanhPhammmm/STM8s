#include <uart.h>
#include <RTC.h>
#include <i2c.h>

uint8_t RTC_buffer[7];
void delay_ms(uint16_t ms) {
    while (ms--) {
        for (uint16_t i = 0; i < 1600; i++) {
            nop(); // Approximate 1 ms delay
        }
    }
}
void CLK_HSI_16Mhz_config(void) {
    CLK->ICKR |= (1 << 1);   
    while (!(CLK->ICKR & (1 << 0))); 
    CLK->SWR = 0xE1;
    CLK->CKDIVR = 0x00;
}
void assert_failed(uint8_t* file, uint32_t line){}

RTC_Time_t current_time;
RTC_Date_t current_date;
  
int main(){
  CLK_HSI_16Mhz_config();
  I2c_Init();
  UART_Configuration(9600);
  
  if(DS1307_Init()){
    UART1_Send_String("DS1307 is ready");
  }
  else{
    UART1_Send_String("DS1307 is failed");
  }
  
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
  
  while(1){
    Get_Current_Time(&current_time);
    Get_Current_Date(&current_date);
    
    TimeToArray(&current_time);
    DateToArray(&current_date);
    
    UART_Send_Array(RTC_buffer, 7);
    delay_ms(1000);
  }
}