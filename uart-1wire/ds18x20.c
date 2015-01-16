#include "config.h"
#include <util/delay.h>
#include "onewire.h"
#include "ds18x20.h"

//Start temperature conversion
uint8_t DS18x20_StartMeasure(OW_ROM_t rom)
{
    OW_SelectROM(rom);
    OW_WriteByte(THERM_CMD_CONVERTTEMP);
    return 1;
}

uint8_t DS18x20_ReadData(OW_ROM_t rom, uint8_t *buffer)
{
    OW_SelectROM(rom);
    
    // Send command to read Scratchpad
    OW_WriteByte(THERM_CMD_RSCRATCHPAD);
    
#ifndef DS18X20_SKIP_CRC
    uint8_t    buff[10] = {1,2,3,4,5,6,7,8,9};  // FIXME
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

// Set ADC resolution
// res = 9, 10, 11, or 12
uint8_t DS18x20_SetResolution(OW_ROM_t rom, uint8_t res) {
    uint8_t data[3] = {0};
    
    // Convert res to bits
    if (res < 9 || res > 12)
        return 0;
    res -= 9;
    
    // Config register value
    data[2] = res << 5;
    data[2] |= 0x1f;
    
    // Send data
    OW_SelectROM(rom);
    
    OW_WriteByte(THERM_CMD_WSCRATCHPAD);
    OW_WriteByte(data[0]);
    OW_WriteByte(data[1]);
    OW_WriteByte(data[2]);
    
    return 1;
}

// Wait for temp. measuring to finish
// res = 9, 10, 11, or 12
void DS18x20_WaitForMeasure(uint8_t res) {
    switch(res) {
    case 12:
        _delay_ms(375);
    case 11:
        _delay_ms(187);
    case 10:
        _delay_ms(94);
    case 9:
        _delay_ms(94);
    }
}
