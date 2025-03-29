#include<button.h>

void assert_failed(uint8_t* file, uint32_t line){}

typein_t ButtonA3 = {.status_old = 1, .pin = 3};

void CLK_HSI_16Mhz_Config(void) {
    CLK->ICKR |= (1 << 0);   
    while (!(CLK->ICKR & (1 << 1))); 
    CLK->SWR = 0xE1;
    CLK->CKDIVR = 0x00;
}

int main(){
  CLK_HSI_16Mhz_Config();
  UART_Configuration(9600);
  Button_Init();
  
  while(1){
    Button_Run(&ButtonA3);
  }
}