#include <i2c.h>

void I2c_Init(void)
{
  I2C->CR1 &= ~I2C_CR1_PE;  // Disable I2C before configuration

  while (I2C->SR3 & I2C_SR3_BUSY);

  I2C->FREQR = 16;          // Set I2C peripheral clock frequency

  // Configure I2C speed
  I2C->CCRL = 0x80;
  I2C->CCRH &= ~(1 << 6);

  I2C->TRISER = 17;

  // Enable Acknowledge and General Call
  I2C->CR2 |= (1 << 2);

  // Set Slave Address
  I2C->OARL = (0x27 << 1);
  I2C->OARH = (1 << 6);  // Enable addressing mode (7-bit)

  if (!(I2C->SR3 & I2C_SR3_BUSY))  // BUSY bit should be 0
  {
      // Enable I2C Peripheral
      I2C->CR1 |= I2C_CR1_PE;
  }

  while (!(I2C->CR1 & I2C_CR1_PE));  // Ensure the peripheral is ON

  //while (!(I2C->SR1 & I2C_SR1_SB));  // Wait for the start condition (optional)
}

void I2c_generate_start(){
  I2C->CR2 |= (1 << 0);
}
void I2c_generate_stop(){
  I2C->CR2 |= (1 << 1);
}
void I2c_send_7bits_address(uint8_t address){
  I2C->DR = (uint8_t)address;
}
void I2c_send_data(uint8_t data){
  I2C->DR = data;
}
uint8_t Get_flag_status_of_bus(void){
  uint8_t tmp = (I2C->SR3 & (1 << 1));
  return tmp;
}
uint8_t Check_event_start(){
  return ((I2C->SR3 & (1 << 1)) && (I2C->SR3 & (1 << 0)) && (I2C->SR1 & (1 << 0)));
}
uint8_t Check_event_send_7bits_address(){
  return ((I2C->SR3 & (1 << 1)) && (I2C->SR3 & (1 << 0))  && (I2C->SR1 & (1 << 7)) && (I2C->SR3 & (1 << 2)));
}
uint8_t Check_event_send_data(){
  return ((I2C->SR3 & (1 << 2)) && (I2C->SR3 & (1 << 1)) && (I2C->SR3 & (1 << 0)) && (I2C->SR1 & (1 << 7)) && (I2C->SR1 & (1 << 2)));
}
void I2c_write(uint8_t address, uint8_t data){
  while(Get_flag_status_of_bus());
  I2c_generate_start();
  while(!Check_event_start());
  
  I2c_send_7bits_address(address);
  while(!Check_event_send_7bits_address());
  
  I2c_send_data(data);
  while(!Check_event_send_data());

  I2c_generate_stop();
}






