#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
    extern "C" {
#endif    
      
#include <stm8s.h>          
      
#define DS1307_ADDR_SEC			0x00
#define DS1307_ADDR_MIN			0x01
#define DS1307_ADDR_HOUR		0x02
#define DS1307_ADDR_DAY			0x03
#define DS1307_ADDR_DATE		0x04
#define DS1307_ADDR_MONTH		0x05
#define DS1307_ADDR_YEAR		0x06
#define DS1307_ADDR			(0x68<<1)

#define AM_TYPE				0
#define PM_TYPE				12
#define ALL_DAY_TYPE			24

#define MONDAY      1
#define TUESDAY     2
#define WEDNESDAY   3
#define THURSDAY    4
#define FRIDAY      5
#define SATURDAY    6
#define SUNDAY      7
      
typedef struct
{
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;

}RTC_Date_t;

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t time_format;

}RTC_Time_t;

static uint8_t Binary_To_Bcd(uint8_t value);
static uint8_t Bcd_To_Binary(uint8_t value);

void DS1307_Write(uint8_t value, uint8_t reg_addr);
uint8_t DS1307_Init();
void DS1307_write(uint8_t value, uint8_t reg_addr);
uint8_t DS1307_read(uint8_t reg_addr);
void Set_Current_Time(RTC_Time_t *RTC_Time);
void Set_Current_Date(RTC_Date_t *RTC_Date);
void Get_Current_Time(RTC_Time_t *RTC_Time);
void Get_Current_Date(RTC_Date_t *RTC_Date);
void NumberToString(uint8_t num, char *buf);
void TimeToArray(RTC_Time_t *RTC_Time);
void DateToArray(RTC_Date_t *RTC_Date);

extern uint8_t RTC_buffer[7];

#ifdef __cplusplus
}
#endif

#endif