typedef struct
{
    uint8_t port;            // Port connection
    uint8_t type_sensor;     // Type of sensor
    uint8_t status;          // Status (saved or not)
    uint8_t struct_count;    // Number of structs created
} flashin;

void Flash_Write_Struct_Array(uint16_t start_address, flashin *dataArray, uint8_t arraySize);
#include <Flash.c>