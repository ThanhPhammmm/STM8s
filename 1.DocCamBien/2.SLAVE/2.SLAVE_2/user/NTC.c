#include <NTC.h>

void TIM2_Init(void) {
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
    TIM2->PSCR = 4;  // Prescaler = 16 ? 1 MHz tick rate (for 16 MHz system)
    TIM2->CNTRH = 0; 
    TIM2->CNTRL = 0;
    TIM2->CR1 = TIM2_CR1_CEN; // Enable TIM2
}
void delay_us(uint16_t us) {
    uint16_t start, count;
    uint8_t cntrl_l, cntrl_h;

    // Read TIM2 Counter value correctly (Low first, then High)
    cntrl_l = TIM2->CNTRL;
    cntrl_h = TIM2->CNTRH;
    start = (cntrl_h << 8) | cntrl_l;

    do {
        cntrl_l = TIM2->CNTRL;
        cntrl_h = TIM2->CNTRH;
        count = (cntrl_h << 8) | cntrl_l;
    } while ((count - start) < us);
}

void delay_ms(uint16_t ms) {
    while (ms--) delay_us(1000);
}
void DHT11_Init(void) {
    // C?u hình chân DATA là INPUT v?i pull-up ban d?u
    GPIO_Init(DHT11_PORT, DHT11_PIN, GPIO_MODE_IN_PU_NO_IT);

    // Ð?i ít nh?t 1 giây d? DHT11 ?n d?nh
    delay_ms(1000);
}
void DHT11_Start(void) {
  // Set pin as OUTPUT for wake-up signal
  GPIO_Init(DHT11_PORT, DHT11_PIN, GPIO_MODE_OUT_OD_LOW_FAST);
  GPIO_WriteLow(DHT11_PORT, DHT11_PIN);
  delay_ms(20);
  GPIO_WriteHigh(DHT11_PORT, DHT11_PIN);
  delay_us(39);

  // Set pin as INPUT for data reception
  GPIO_Init(DHT11_PORT, DHT11_PIN, GPIO_MODE_IN_FL_NO_IT);
}


uint8_t DHT11_CheckResponse(void) {
    uint8_t response = 0;
    uint16_t timeout = 65000;  // Timeout d? tránh treo chuong trình

    // Ch? DHT11 kéo xu?ng LOW (80µs)
    while (GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {  
        if (--timeout == 0) 
        {
          return 0;  // N?u timeout, không có ph?n h?i
        }
    }
    delay_us(80);  

    // Ch? DHT11 kéo lên HIGH (80µs)
    timeout = 65000;
    while (!GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {  
      if (--timeout == 0) {
        return 0;  
      }
    }
    delay_us(80);

    response = 1;  // N?u vu?t qua c? hai ki?m tra, c?m bi?n dã ph?n h?i
    return response;
}


uint8_t DHT11_ReadByte(void) {
    uint8_t i, byte = 0;

    for (i = 0; i < 8; i++) {
        uint16_t timeout = 65000;  // Prevent infinite loop

        // Wait for the LOW signal (Start of Bit Transmission)
        while (!GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {
            if (--timeout == 0) return 0xFF; // Timeout error
        }

        // Wait 50µs (LOW part of the bit)
        delay_us(58);

        // Read bit value based on HIGH pulse width
        if (GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {
            delay_us(35);  // Wait additional time
            
            // If still HIGH ? Bit '1', otherwise, it's '0' by default
            if (GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {
                byte |= (1 << (7 - i));
            }

            // Wait for the LOW signal (End of Bit Transmission)
            timeout = 65000;
            while (GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {
                if (--timeout == 0) return 0xFF; // Timeout error
            }
        }
    }
    return byte;
}

uint8_t DHT11_ReadTemperature() {
    uint8_t h_int, h_dec, t_int, t_dec, checksum;
    
    DHT11_Start();
    if (!DHT11_CheckResponse()){
      return -1;  // Return error value
    }
    h_int = DHT11_ReadByte();  // Humidity integer part
    h_dec = DHT11_ReadByte();  // Humidity decimal part
    t_int = DHT11_ReadByte();  // Temperature integer part
    t_dec = DHT11_ReadByte();  // Temperature decimal part
    checksum = DHT11_ReadByte(); // Checksum

    return t_int;

    // Validate checksum
//    uint8_t calculated_checksum = h_int + h_dec + t_int + t_dec;
//    if (calculated_checksum != checksum){
//      return -1; // Return error value
//    }
//    return t_int;
}
