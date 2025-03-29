#include "stm8s.h"
#include <uart.h>

void assert_failed(uint8_t* file, uint32_t line){}

#define DHT11_PORT  GPIOC
#define DHT11_PIN   GPIO_PIN_7

#include "stm8s.h"
void Clock_Init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  // Set HSI to 16MHz
}
void TIM2_Init(void) {
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
    TIM2->PSCR = 4;  // Prescaler = 16 (1MHz tick rate for 16MHz system clock)
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
  delay_ms(25);
  GPIO_WriteHigh(DHT11_PORT, DHT11_PIN);
  delay_us(40);

  // Set pin as INPUT for data reception
  GPIO_Init(DHT11_PORT, DHT11_PIN, GPIO_MODE_IN_FL_NO_IT);
      delay_us(5); // Ch? m?t chút tru?c khi ki?m tra ph?n h?i

}


uint8_t DHT11_CheckResponse(void) {
    uint8_t response = 0;
    uint16_t timeout = 5000;  // Timeout d? tránh treo chuong trình

    // Ch? DHT11 kéo xu?ng LOW (80µs)
    while (GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {  
        if (--timeout == 0) 
        {
          return 0;  // N?u timeout, không có ph?n h?i
        }
    }
    delay_us(80);  

    // Ch? DHT11 kéo lên HIGH (80µs)
    timeout = 2000;
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
        uint16_t timeout = 50000;  // Prevent infinite loop

        // Wait for the LOW signal (Start of Bit Transmission)
        while (!GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {
            if (--timeout == 0) return 0xFF; // Timeout error
        }

        // Wait 50µs (LOW part of the bit)
        delay_us(50);

        // Read bit value based on HIGH pulse width
        if (GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {
            delay_us(30);  // Wait additional time
            
            // If still HIGH ? Bit '1', otherwise, it's '0' by default
            if (GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {
                byte |= (1 << (7 - i));
            }

            // Wait for the LOW signal (End of Bit Transmission)
            timeout = 50000;
            while (GPIO_ReadInputPin(DHT11_PORT, DHT11_PIN)) {
                if (--timeout == 0) return 0xFF; // Timeout error
            }
        }
    }
    return byte;
}

uint8_t DHT11_Read(uint8_t *humidity, uint8_t *temperature) {
    uint8_t h_int, h_dec, t_int, t_dec, checksum;
    DHT11_Start();
        if (!DHT11_CheckResponse()){
      return 0;
}
    h_int = DHT11_ReadByte();
    h_dec = DHT11_ReadByte();
    t_int = DHT11_ReadByte();
    t_dec = DHT11_ReadByte();
    checksum = DHT11_ReadByte();
    
    uint8_t calculated_checksum = h_int + h_dec + t_int + t_dec;

    if (calculated_checksum != checksum) {
      return 0;
    }
    *humidity = h_int;
    *temperature = t_int;
    return 1;
}

void main(void) {
      Clock_Init();   // Set system clock to 16MHz

    uint8_t humidity, temperature;
    UART_Configuration(9600);
    TIM2_Init();
    DHT11_Init();
    while (1) {
        if (DHT11_Read(&humidity, &temperature)) {
            // Successfully read data
          UART1_Send_Int(humidity);
          UART1_Send_String(" ");
          UART1_Send_Int(temperature);
          UART1_Send_String(" ");
          delay_ms(500);

        } else {
            // Error reading data
        }
    }
}
