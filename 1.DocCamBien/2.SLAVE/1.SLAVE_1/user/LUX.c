#include <LUX.h>

#define BH1750_ADDRESS  (0x23 << 1)  // BH1750 default I2C address
 
void I2C_Init_BH1750() {
    I2C_DeInit();
    I2C_Init(100000, BH1750_ADDRESS, I2C_DUTYCYCLE_2, 
         I2C_ACK_CURR, I2C_ADDMODE_7BIT, CLK_GetClockFreq() / 1000000);
    I2C_Cmd(ENABLE);
}
void BH1750_Write(uint8_t command) {
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(BH1750_ADDRESS, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(command);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(ENABLE);
}
float BH1750_ReadLight() {
    uint8_t high, low;
    float lux;

    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(BH1750_ADDRESS, I2C_DIRECTION_RX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
    high = I2C_ReceiveData();  // Read high byte

    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
    low = I2C_ReceiveData();   // Read low byte

    I2C_GenerateSTOP(ENABLE);

    lux = ((high << 8) | low) / 1.2f;
    return lux;
}