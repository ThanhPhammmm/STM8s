#include <uart.h>
#include <RTC.h>
#include <i2c.h>
#include <tim.h>

uint8_t RTC_buffer[7];
void delay_ms(uint16_t ms) {
    while (ms--) {
        for (uint16_t i = 0; i < 1600; i++) {
            nop(); // Approximate 1 ms delay
        }
    }
}
RTC_Time_t current_time;
RTC_Date_t current_date;

INTERRUPT_HANDLER(TIM1_UPD_OVF_IRQHandler, 11)
{  
  if (TIM1->SR1 & (1 << 0)) {  // Check UIF flag (bit 0)
    TIM1->SR1 &= ~(1 << 0);  // Clear UIF flag

    // Handle button press or other actions

    TIM1_CLEAR_IT_PENDING(UPDATE_EVENT);
    Get_Current_Time(&current_time);
    Get_Current_Date(&current_date);

    TimeToArray(&current_time);
    DateToArray(&current_date);

    UART_Send_Array(RTC_buffer, 7);
  }
}
void CLK_HSI_16Mhz_config(void) {
    CLK->ICKR |= (1 << 1);   
    while (!(CLK->ICKR & (1 << 0))); 
    CLK->SWR = 0xE1;
    CLK->CKDIVR = 0x00;
}
void assert_failed(uint8_t* file, uint32_t line){}
  
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
  TIM1_BaseInit(15999, COUNT_UP, 1000 - 1, 0);
enableInterrupts();

  while(1){

  }
}