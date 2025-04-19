#include <RTC.h>
#include <system.h>

static uint8_t Binary_To_Bcd(uint8_t value)
{
    uint8_t high_nibble;
    uint8_t low_nibble;
    uint8_t bcd;

    bcd = value;
    if(value >= 10)
    {
        high_nibble = value / 10;
        low_nibble = value % 10;
        bcd = (high_nibble << 4) | low_nibble;
    }

    return bcd;
}
static uint8_t Bcd_To_Binary(uint8_t value)
{
    uint8_t high_nibble = (uint8_t)((value >> 4) * 10);
    uint8_t low_nibble = value & (uint8_t)0x0F;

    return (high_nibble + low_nibble);
}
void DS1307_Init(){
  DS1307_write(0,DS1307_ADDR_SEC);
}

void DS1307_write(uint8_t value, uint8_t reg_addr) {
    while (I2C->SR3 & I2C_SR3_BUSY);  // Ch? khi bus b?n
    // Send START condition
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // Send DS1307 address with Write mode (0xD0)
    I2C_Send7bitAddress(DS1307_ADDR, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send register address
    I2C_SendData(reg_addr);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send data to write
    I2C_SendData(value);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send STOP condition
    I2C_GenerateSTOP(ENABLE);
}

uint8_t DS1307_read(uint8_t reg_addr) {
  
    uint8_t data;
    
    while (I2C->SR3 & I2C_SR3_BUSY);  // Ch? khi bus b?n

    // G?i di?u ki?n START
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // G?i d?a ch? DS1307 v?i bit Write (0xD0)
    I2C_Send7bitAddress(DS1307_ADDR, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // G?i d?a ch? thanh ghi c?n d?c
    I2C_SendData(reg_addr);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // G?i STOP d? DS1307 chu?n b? d? li?u
    I2C_GenerateSTOP(ENABLE);

    // G?i di?u ki?n START m?i (Repeated Start)
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // G?i d?a ch? DS1307 v?i bit Read (0xD1)
    I2C_Send7bitAddress(DS1307_ADDR, I2C_DIRECTION_RX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
    data = I2C_ReceiveData();

    // **T?t ACK tru?c khi nh?n byte cu?i**
    I2C_AcknowledgeConfig(I2C_ACK_NONE);
    
    // G?i STOP
    I2C_GenerateSTOP(ENABLE);

    return data;
}

void Set_Current_Time(RTC_Time_t *RTC_Time){
	uint8_t seconds = Binary_To_Bcd(RTC_Time->seconds );
	seconds &= ~(1<<7);
	DS1307_write(seconds,DS1307_ADDR_SEC);

	uint8_t minutes = Binary_To_Bcd(RTC_Time->minutes);
	DS1307_write(minutes,DS1307_ADDR_MIN);

	uint8_t hours = Binary_To_Bcd(RTC_Time->hours);

	if(RTC_Time->time_format == AM_TYPE){
		hours &= ~(1<<6);
		hours |= (1<<6);

		hours &= ~(1<<5);
	}
	else if(RTC_Time->time_format == PM_TYPE){
		hours &= ~(1<<6);
		hours |= (1<<6);

		hours &= ~(1<<5);
		hours |= (1<<5);
	}
	else if(RTC_Time->time_format == ALL_DAY_TYPE){
		hours &= ~(1<<6);
	}
	DS1307_write(hours,DS1307_ADDR_HOUR);
}
void Set_Current_Date(RTC_Date_t *RTC_Date){
	uint8_t day = Binary_To_Bcd(RTC_Date->day);
	DS1307_write(day,DS1307_ADDR_DAY);

	uint8_t date = Binary_To_Bcd(RTC_Date->date);
	DS1307_write(date,DS1307_ADDR_DATE);

	uint8_t month = Binary_To_Bcd(RTC_Date->month);
	DS1307_write(month,DS1307_ADDR_MONTH);

	uint8_t year = Binary_To_Bcd(RTC_Date->year);
	DS1307_write(year,DS1307_ADDR_YEAR);
}
void Get_Current_Time(RTC_Time_t *RTC_Time){
    uint8_t seconds = DS1307_read(DS1307_ADDR_SEC);
    seconds &= ~(1<<7);
    RTC_Time->seconds = seconds;

    uint8_t min = DS1307_read(DS1307_ADDR_MIN);
    RTC_Time->minutes = min;

    uint8_t hours = DS1307_read(DS1307_ADDR_HOUR);
    if(hours & (1<<6)){
            //12_format
            if(hours & (1<<5)){
                    RTC_Time->time_format = PM_TYPE;
            }
            else{
                    RTC_Time->time_format = AM_TYPE;
            }
            hours &= ~(0x03<<5);
    }
    else{
            //24_format
            RTC_Time->time_format = ALL_DAY_TYPE;
    }
    RTC_Time->hours = hours;
    
    TimeToArray(RTC_Time);
}

void Get_Current_Date(RTC_Date_t *RTC_Date){
  RTC_Date->day = DS1307_read(DS1307_ADDR_DAY);
  RTC_Date->date = DS1307_read(DS1307_ADDR_DATE);
  RTC_Date->month = DS1307_read(DS1307_ADDR_MONTH);
  RTC_Date->year = DS1307_read(DS1307_ADDR_YEAR);
  
  DateToArray(RTC_Date);
}
void TimeToArray(RTC_Time_t *RTC_Time){
  RTC_buffer[4] = RTC_Time->hours;
  RTC_buffer[5] = RTC_Time->minutes;
  RTC_buffer[6] = RTC_Time->seconds;
}
void DateToArray(RTC_Date_t *RTC_Date){
  RTC_buffer[0] = RTC_Date->day;
  RTC_buffer[1] = RTC_Date->date;
  RTC_buffer[2] = RTC_Date->month;
  RTC_buffer[3] = RTC_Date->year;
}