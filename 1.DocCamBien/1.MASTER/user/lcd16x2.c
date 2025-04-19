#include <lcd16x2.h>

//D7D6D5D4BLERwRs
//xxxx10x1
//MSB -> LSB

static uint8_t Bcd_To_Binary(uint8_t value)
{
    uint8_t high_nibble = (uint8_t)((value >> 4) * 10);
    uint8_t low_nibble = value & (uint8_t)0x0F;

    return (high_nibble + low_nibble);
}
// Define arrays for the day-of-week and month names
char *days_of_week[] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};
char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July",
                  "August", "Sep", "Oct", "Nov", "Dec"};
uint8_t highNibble, lowNibble;

void LCD_SendCommand(uint8_t cmd) {
    
    highNibble = (cmd & 0xF0);  // Get high nibble
    lowNibble = ((cmd << 4) & 0xF0);  // Get low nibble
    
    // Send high nibble
    I2c_write(0x27 << 1, highNibble | 0x0C);  // Set enable bit (E=1)
    I2c_write(0x27 << 1, highNibble | 0x08);         // Clear enable bit (E=0)
    
    // Send low nibble
    I2c_write(0x27 << 1, lowNibble | 0x0C);  // Set enable bit (E=1)
    I2c_write(0x27 << 1, lowNibble | 0x08);         // Clear enable bit (E=0)
}
void LCD_SendData(uint8_t data) {
    
    highNibble = (data & 0xF0);  // Get high nibble
    lowNibble = ((data << 4) & 0xF0);  // Get low nibble
    
    // Send high nibble
    I2c_write(0x27 << 1, highNibble | 0x0D);  // Set RS=1 (data mode), E=1
    I2c_write(0x27 << 1, highNibble | 0x09);  // Set RS=1, E=0
    
    // Send low nibble
    I2c_write(0x27 << 1, lowNibble | 0x0D);  // Set RS=1 (data mode), E=1
    I2c_write(0x27 << 1, lowNibble | 0x09);  // Set RS=1, E=0
}
void LCD_SendString(char* str){
  while(*str){
    LCD_SendData((uint8_t)*str);
    str++;
  }
}
void LCD_Init(void) {
    Delay_ms(50);  // Wait for LCD to power up
    
    // Initialize in 4-bit mode
    LCD_SendCommand(0x30);  // Function set: 8-bit mode (called three times)
    Delay_ms(5);
    LCD_SendCommand(0x30);  
    Delay_ms(10);
    LCD_SendCommand(0x30);
    
    // Switch to 4-bit mode
    LCD_SendCommand(0x20);  // Function set: 4-bit mode
    
    // Configure display (2-line, 5x8)
    LCD_SendCommand(0x28);
    Delay_ms(1);

    //Display on, Cursor on
    LCD_SendCommand(0x0E);
    Delay_ms(1);

    //Display off
//    LCD_SendCommand(0x08);
//    Delay_ms(1);
    
    //Display clear
    LCD_SendCommand(0x01);
    Delay_ms(2);
    
    //Entry mode set
    LCD_SendCommand(0x06);
}
void LCD_Set_Cursor(uint8_t row, uint8_t column){
  column -= 1;
  switch(row){
    case 1:
      column |= 0x80;
      LCD_SendCommand(column);
      break;
    case 2:
      column |= 0xC0;
      LCD_SendCommand(column);
      break;
    default:
        break;
  }
}
//void LCD_Light_BackGround(uint8_t State){
//  uint8_t highNibble;
//
//  highNibble = (State & 0xF0);  // Get high nibble
////  lowNibble = ((State << 4) & 0xF0);  // Get low nibble
//  if(State == 1){
//    I2c_write(0x27 << 1, highNibble | 0x0C);  // E=1, Backlight=1
//    I2c_write(0x27 << 1, highNibble | 0x08);  // E=0, Backlight=1
//  }
//  else{
//    I2c_write(0x27 << 1, highNibble | 0x04);  // E=1, Backlight=1
//    I2c_write(0x27 << 1, highNibble | 0x00);  // E=0, Backlight=1
//  }
//}
void LCD_Display_Float(float value) {
    char buffer[10];  // Buffer to store formatted number

    int intPart = (int)value;  // Extract integer part
    int fracPart = (int)((value - intPart) * 100);  // Extract fractional part (2 decimal places)

    if (fracPart < 0) {
        fracPart = -fracPart;  // Ensure positive fraction
    }

    uint8_t index = 0;

    // Handle negative numbers
    if (value < 0) {
        buffer[index++] = '-';
        intPart = -intPart;  // Convert to positive for display
    }

    // Convert integer part to string
    if (intPart == 0) {
        buffer[index++] = '0';  // If integer part is 0, add '0'
    } else {
        char temp[6];  // Temporary buffer to store digits
        uint8_t tempIndex = 0;

        while (intPart > 0) {
            temp[tempIndex++] = (intPart % 10) + '0';
            intPart /= 10;
        }

        // Reverse the digits into the main buffer
        while (tempIndex > 0) {
            buffer[index++] = temp[--tempIndex];
        }
    }

    // Decimal point
    buffer[index++] = '.';

    // Convert fractional part to string (2 digits)
    buffer[index++] = (fracPart / 10) + '0';
    buffer[index++] = (fracPart % 10) + '0';

    // Null terminator
    buffer[index] = '\0';

    // Send the string to LCD
    LCD_SendString(buffer);
}
void LCD_Send16BitNumber(uint16_t num, uint8_t min_digits) {
    char buffer[6];  // Buffer for up to 5 digits + null terminator
    uint8_t i = 0;

    // Convert number to string
    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    // Ensure at least min_digits are printed, but avoid leading zeros for numbers 0-9
    while (i < min_digits) {
        buffer[i++] = '0';
    }

    buffer[i] = '\0';  // Null terminate

    // Reverse string to correct order
    for (uint8_t j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - 1 - j];
        buffer[i - 1 - j] = temp;
    }

    // Remove leading zeros for numbers 0-9
    if (buffer[0] == '0' && i == 2) {
        buffer[0] = buffer[1];
        buffer[1] = '\0';
    }

    LCD_SendString(buffer);
}

void LCD_Send16BitNumber_RTC(uint16_t num, uint8_t min_digits) {
    char buffer[6];  // Buffer for up to 5 digits + null terminator
    uint8_t i = 0;

    do {
        buffer[i++] = (num % 10) + '0';  // Extract last digit and convert to ASCII
        num /= 10;
    } while (num > 0);

    // Ensure at least min_digits are printed
    while (i < min_digits) {
        buffer[i++] = '0';  // Add leading zeros if necessary
    }

    buffer[i] = '\0';  // Null terminate

    // Reverse string before sending (because we stored digits in reverse order)
    for (uint8_t j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - 1 - j];
        buffer[i - 1 - j] = temp;
    }

    LCD_SendString(buffer);
}

void Send_RTC_To_LCD(uint8_t *data_sensor3) {
  LCD_SendCommand(0x01);

  // Print the first line: "Wednesday 25 February 2004"
  LCD_Set_Cursor(1, 1);
  LCD_SendString(days_of_week[Bcd_To_Binary(data_sensor3[0])]); // Print day-of-week
  LCD_SendData('/');
  LCD_Send16BitNumber_RTC((Bcd_To_Binary(data_sensor3[1])),2);       // Print day number
  LCD_SendData('/');
  LCD_SendString(months[Bcd_To_Binary(data_sensor3[2]) - 1]);        // Print month
  LCD_SendData('/');
  LCD_Send16BitNumber_RTC((2000 + Bcd_To_Binary(data_sensor3[3])) / 100,2);               // Print century (20)
  LCD_Send16BitNumber_RTC((2000 + Bcd_To_Binary(data_sensor3[3])) % 100,2);               // Print year (04)

  // Print the second line: "07:00:00"
  LCD_Set_Cursor(2, 1);
  LCD_Send16BitNumber_RTC(Bcd_To_Binary(data_sensor3[4]),2);
  LCD_SendData(':');
  LCD_Send16BitNumber_RTC(Bcd_To_Binary(data_sensor3[5]),2);
  LCD_SendData(':');
  LCD_Send16BitNumber_RTC(Bcd_To_Binary(data_sensor3[6]),2);
}