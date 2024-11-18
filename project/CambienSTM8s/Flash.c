void Flash_Write_Struct_Array(uint16_t start_address, flashin *dataArray, uint8_t arraySize){
    size_t dataSize = sizeof(flashin) * arraySize;

    // Unlock Flash memory
    FLASH->DUKR = 0xAE;  // Key 1
    FLASH->DUKR = 0x56;  // Key 2

    // Wait until the Flash is unlocked
    while ((FLASH->IAPSR & FLASH_IAPSR_DUL) == 0);

    // _write each byte of the struct array to Flash
    for (size_t i = 0; i < dataSize; i++) {
        *(uint8_t*)(start_address+i) = ((uint8_t*)dataArray)[i];
        //start_address ;

        // Wait until the write operation is finished
        while ((FLASH->IAPSR & FLASH_IAPSR_EOP) == 0);
    }

    // Lock Flash memory again
    FLASH->IAPSR &= ~FLASH_IAPSR_DUL;
   // flash_w[arraySize-1].next_address = start_address+1; 
}