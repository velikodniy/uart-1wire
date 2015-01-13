#include "config.h"
#include <util/crc16.h>
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

#ifdef DS18X20_CHECK_CRC
// Optimized Dallas (now Maxim) iButton 8-bit CRC calculation.
uint8_t crc8 (uint8_t *data, uint8_t count) {
    uint8_t crc = 0x00; // Initial value
    for (uint8_t i = 0; i < count; i++)
        crc = _crc_ibutton_update(crc, data[i]);
    return crc;
}
#endif 

uint8_t DS18x20_ReadData(uint8_t *rom, uint8_t *buffer)
{
    //Reset, skip ROM and send command to read Scratchpad
    if (!OW_Reset()) return 0;
    
    if (rom) OW_MatchROM(rom);
    else OW_WriteByte(OW_CMD_SKIPROM);
    
    OW_WriteByte(THERM_CMD_RSCRATCHPAD);
    
#ifdef DS18X20_CHECK_CRC
    uint8_t    buff[10] = {1,2,3,4,5,6,7,8,9};
    for (uint8_t i=0; i<9; i++) buff[i] = OW_ReadByte();
    buffer[0] = buff[0]; buffer[1] = buff[1];
    if (crc8(buff, 9)) return 0;    // Error if CRC is incorrect
#else 
    //Read Scratchpad (only 2 first bytes)
    buffer[0] = OW_ReadByte(); // Read TL
    buffer[1] = OW_ReadByte(); // Read TH    
#endif

    return 1;
}

void DS18x20_ConvertToThemperature(uint8_t* data, uint8_t* themp)
{
    //Store temperature integer digits and decimal digits
    themp[0] = data[0]>>4;
    themp[0] |= (data[1]&0x07)<<4;
    //Store decimal digits
    themp[1] = data[0]&0xf;
    themp[1] *= 6;    
    if (data[1]>0xFB){
        themp[0] = 127-themp[0];
        themp[0] |= 0b10000000; // for negative temperature (alse see below)
    } 
}

float DS18x20_ConvertToThemperatureFl(uint8_t* data)
{
    float    Temperature;
    uint8_t    digit, decimal;
    //Store temperature integer digits and decimal digits
    digit = data[0]>>4;
    digit |= (data[1]&0x07)<<4;
    //Store decimal digits
    decimal = data[0]&0xf;
    decimal *= 6;    
    
    if (data[1]>0xFB) digit = 127-digit;
    if (decimal<100) Temperature = digit + ((float)decimal/100);
        else Temperature = digit + ((float)decimal/1000);
    if (data[1]>0xFB) Temperature = -Temperature;
    /*
    if (data[1]>0xFB){
        digit = 127-digit;
        digit |= 0b10000000; // for negative temperature
    } */
    return Temperature;
}
