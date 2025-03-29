#include <uart.h>
#include "atc_user.h"
void assert_failed(uint8_t* file, uint32_t line){}

int main(){
  UART_Configuration(9600);
  atc_init();

}