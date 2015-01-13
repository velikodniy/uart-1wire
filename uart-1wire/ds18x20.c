#include "config.h"
#include <util/delay.h>
#include "onewire.h"
#include "ds18x20.h"

uint8_t DS18x20_StartMeasure(uint8_t* rom)
{
    //Reset, skip ROM and start temperature conversion
    if (!OW_Reset()) return 0;
    
    if (rom) OW_MatchROM(rom);
    else OW_WriteByte(OW_CMD_SKIPROM);
    
    OW_WriteByte(THERM_CMD_CONVERTTEMP);
    return 1;
}

uint8_t DS18x20_ReadData(uint8_t *rom, uint8_t *buffer)
{
    //Reset, skip ROM and send command to read Scratchpad
    if (!OW_Reset()) return 0;
    
    if (rom) OW_MatchROM(rom);
    else OW_WriteByte(OW_CMD_SKIPROM);
    
    OW_WriteByte(THERM_CMD_RSCRATCHPAD);
    
#ifndef DS18X20_SKIP_CRC
    uint8_t    buff[10] = {1,2,3,4,5,6,7,8,9};
    for (uint8_t i=0; i<9; i++) buff[i] = OW_ReadByte();
    buffer[0] = buff[0]; buffer[1] = buff[1];
    if (OW_CRC8(buff, 9)) return 0;    // Error if CRC is incorrect
#else 
    //Read Scratchpad (only 2 first bytes)
    buffer[0] = OW_ReadByte(); // Read TL
    buffer[1] = OW_ReadByte(); // Read TH    
#endif

    return 1;
}

// Convert data to Celsius degrees
// temp must be uint8_t[2] array.
// temp[0] - integer part
// temp[1] - fractional part (*10^-2)
uint8_t DS18x20_DataConvert(uint8_t* data, int8_t* temp)
{
    // Store temperature integer part
    temp[0] = data[0] >> 4;             // Decode LS byte
    temp[0] |= (data[1] & 0x07) << 4;   // Decode MS byte

    // Check sign
    if (data[1] & 0xF8)
        temp[0] = - temp[0];
    
    // Compute fractional part (*10^-2)
    uint16_t frac;
    frac = data[0] & 0x0f;
    // Multiplying by 6.25 = 50 / 8
    frac *= 50;
    frac >>= 3;
    // Store value
    temp[1] = frac;
    return 1;
}

