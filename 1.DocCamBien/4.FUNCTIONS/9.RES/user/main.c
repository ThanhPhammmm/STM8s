#include "stm8s.h"
#include "uart.h"

void assert_failed(uint8_t* file, uint32_t line){}

void ADC_Init() {
    ADC1->CSR = 0x03; // Select ADC channel 3 (PD2/AIN3)
    ADC1->CR1 = 0x71; // Enable ADC, fADC = fCPU/2, Continuous Mode
    ADC1->CR2 = 0x00; // Right-aligned data
}

uint16_t Read_ADC() {
    ADC1->CR1 |= ADC1_CR1_ADON;  // Start ADC conversion
    while (!(ADC1->CSR & ADC1_CSR_EOC)); // Wait for conversion complete
    ADC1->CSR &= ~ADC1_CSR_EOC; // Clear EOC flag

    uint8_t high = ADC1->DRH;  // Read high byte first
    uint8_t low  = ADC1->DRL;  // Read low byte after

    return ((high << 2) | (low & 0x03)); // Correct 10-bit ADC value
}

void main() {
    UART_Configuration(9600);
    uint16_t adc_value;
    ADC_Init();

    while (1) {
        adc_value = Read_ADC(); // Read ADC value from PD2 (AIN3)
        UART1_Send_Int(adc_value);
    }
}
