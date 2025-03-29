#include <RES.h>

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