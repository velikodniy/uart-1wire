#include "config.h"
#include <util/delay.h>
#include "onewire.h"
#include "ds18x20.h"

//Start temperature conversion
uint8_t DS18x20_StartMeasure(OW_ROM_t rom)
{
    if (OW_SelectROM(rom) == OW_FAIL)
        return DS18x20_FAIL;
    OW_WriteByte(THERM_CMD_CONVERTTEMP);
    return DS18x20_OK;
}

// Read data from sensor's scratchpad
uint8_t DS18x20_ReadData(OW_ROM_t rom, uint8_t *buffer)
{
    if (OW_SelectROM(rom) == OW_FAIL)
        return DS18x20_FAIL;
    OW_WriteByte(THERM_CMD_RSCRATCHPAD);

    buffer[0] = OW_ReadByte(); // Read TL
    buffer[1] = OW_ReadByte(); // Read TH
    
#if !DS18X20_SKIP_CRC
    uint8_t buffer_crc[9] = {buffer[0], buffer[1]};
    for (uint8_t i=2; i<9; i++)
        buffer_crc[i] = OW_ReadByte();
    if (OW_CRC8(buffer_crc, 9))
        return DS18x20_FAIL;  // Error if CRC is incorrect
#endif

    return DS18x20_OK;
}

// Convert data to Celsius degrees
// temp must be uint8_t[2] array.
// temp[0] - integer part
// temp[1] - fractional part (*10^-2)
// Examples:
//      +17.12 -> [ 17,  12]
//      -17.12 -> [-17, -12]
uint8_t DS18x20_DataConvert(uint8_t* data, int8_t* temp)
{
    // Store temperature integer part
    temp[0] = data[0] >> 4;             // Decode LS byte
    temp[0] |= (data[1] & 0x07) << 4;   // Decode MS byte

    // Compute fractional part (*10^-2)
    uint16_t frac;
    frac = data[0] & 0x0f;
    // Multiplying by 6.25 = 50 / 8
    frac *= 50;
    frac >>= 3;
    // Store value
    temp[1] = frac;

    // Check sign
    if (data[1] & 0xF8) {
        temp[0] = temp[0] - 127;
        temp[1] -= 100;
    }

    return 1;   // FIXME
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
    if (OW_SelectROM(rom) == OW_FAIL)
        return DS18x20_FAIL;
    
    OW_WriteByte(THERM_CMD_WSCRATCHPAD);
    OW_WriteByte(data[0]);
    OW_WriteByte(data[1]);
    OW_WriteByte(data[2]);
    
    return DS18x20_OK;
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
