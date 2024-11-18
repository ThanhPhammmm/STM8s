#include <main.h>

int main( void ){
//  __enable_interrupt();
  CLK_HSI_16Mhz_config();
  flashin sensors[2] = {
    {1, 10, 0, 5},  // Sensor 1
    {2, 20, 1, 6}   // Sensor 2
};
  Flash_Write_Struct_Array(0x4000, sensors, 2);  // Write to address 0x8000
  while(1);
}