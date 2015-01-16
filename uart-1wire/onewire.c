#include "config.h"
#include <avr/sfr_defs.h>
#include <util/crc16.h>
#include <util/delay.h>
#include "onewire.h"

// Command codes
#define OW_CMD_SEARCHROM    0xF0
#define OW_CMD_READROM      0x33
#define OW_CMD_MATCHROM     0x55
#define OW_CMD_SKIPROM      0xCC

#define OW_SEARCH_FIRST     0xFF        // start new search
#define OW_PRESENCE_ERR     0xFF
#define OW_DATA_ERR         0xFE
#define OW_LAST_DEVICE      0x00        // last device found
                         // 0x01..0x40     continue searching

// Useful macros
#define sbi(reg,bit)        reg |= (1<<bit)
#define cbi(reg,bit)        reg &= ~(1<<bit)
#define ibi(reg,bit)        reg ^= (1<<bit)

// Pull (mode=1) or release (mode=0) line
void OW_Set(uint8_t mode)
{
    if (mode) {
        cbi(OW_PORT, OW_BIT); 
        sbi(OW_DDR, OW_BIT);
    } else {
        cbi(OW_PORT, OW_BIT); 
        cbi(OW_DDR, OW_BIT);
    }
}

// Get line value
uint8_t OW_CheckIn(void)
{
    return bit_is_set(OW_PIN, OW_BIT);
}

// Reset line
uint8_t OW_Reset(void)
{
    uint8_t status;
    OW_Set(1);
    _delay_us(480);
    
    OW_Set(0);
    _delay_us(60);
    
    //Store line value and wait until the completion of 480us period
    status = OW_CheckIn();
    _delay_us(420);
    
    //Return the value read from the presence pulse (0=OK, 1=WRONG)
    return status==0 ? OW_OK : OW_FAIL;
}

void OW_WriteBit(uint8_t bit)
{
    //Pull line low for 1us
    OW_Set(1);
    _delay_us(1);
    
    //If we want to write 1, release the line (if not will keep low)
    if(bit) OW_Set(0); 
    
    //Wait for 60us and release the line
    _delay_us(60);
    OW_Set(0);
}

uint8_t OW_ReadBit(void)
{
    uint8_t bit=0;
    
    //Pull line low for 1us
    OW_Set(1);
    _delay_us(1);
    
    //Release line and wait for 14us
    OW_Set(0);
    _delay_us(14);
    
    //Read line value
    if(OW_CheckIn()) bit=1;
    
    //Wait for 45us to end and return read value
    _delay_us(45);
    return bit;
}

void OW_WriteByte(uint8_t byte)
{
    for (uint8_t i=0; i<8; i++)
        OW_WriteBit(bit_is_set(byte, i));
}

uint8_t OW_ReadByte(void)
{
    uint8_t byte=0;
    for (uint8_t i=0; i<8; i++)
        if (OW_ReadBit())
            sbi(byte, i);
    return byte;
}

uint8_t OW_SearchROM(uint8_t diff, OW_ROM_t rom)
{     
    uint8_t i, j, next_diff;
    uint8_t b;

    if(!OW_Reset()) 
        return OW_PRESENCE_ERR;         // error, no device found

    OW_WriteByte(OW_CMD_SEARCHROM);     // ROM search command
    next_diff = OW_LAST_DEVICE;         // unchanged on last device
    
    i = OW_ROMCODE_SIZE * 8;            // 8 bytes
    do {    
        j = 8;                          // 8 bits
        do { 
            b = OW_ReadBit();           // read bit
            if (OW_ReadBit()) {
                // read complement bit
                if (b)                 
                    return OW_DATA_ERR; // data error
            } else { 
                if (!b) {               // 0 = 2 devices
                    if (diff > i || ((*rom & 1) && diff != i)) { 
                        b = 1;               // now 1
                        next_diff = i;       // next pass 0
                    }
                }
            }
            OW_WriteBit (b);           // write bit
            *rom >>= 1;
            if (b)
                *rom |= 0x80;           // store bit
            i--;
        } while (--j);
        rom++;                          // next byte
    } while (i);
    
    return next_diff;                   // to continue search
}

void OW_FindROM(uint8_t *diff, OW_ROM_t rom)
{
    do {
        *diff = OW_SearchROM (*diff, rom);
    } while (!( *diff == OW_PRESENCE_ERR ||
                *diff == OW_DATA_ERR     ||
                *diff == OW_LAST_DEVICE  ));
}

uint8_t OW_ReadROM(uint8_t *buffer)
{
    if (!OW_Reset())
        return OW_FAIL;
    OW_WriteByte(OW_CMD_READROM);
    for (uint8_t i = 0; i < 8; i++)
        buffer[i] = OW_ReadByte();
    return OW_OK;
}

uint8_t OW_MatchROM(OW_ROM_t rom)
{
    if (!OW_Reset())
        return OW_FAIL;
        
    OW_WriteByte(OW_CMD_MATCHROM);    
    for(uint8_t i = 0; i < 8; i++)
        OW_WriteByte(rom[i]);
    return OW_OK;
}

// Select specified ROM (select all if rom=0)
uint8_t OW_SelectROM(OW_ROM_t rom) {
    if (!OW_Reset()) return OW_FAIL;
    if (rom) OW_MatchROM(rom);
    else OW_WriteByte(OW_CMD_SKIPROM);
    return OW_OK;
}

// Optimized Dallas (now Maxim) iButton 8-bit CRC calculation.
uint8_t OW_CRC8 (uint8_t *data, uint8_t count) {
    uint8_t crc = 0x00; // Initial value
    for (uint8_t i = 0; i < count; i++)
    crc = _crc_ibutton_update(crc, data[i]);
    return crc;
}

// Search all devices
uint8_t OW_SearchDevices(OW_ROM_t devices[], uint8_t max_count)
{
    uint8_t id[OW_ROMCODE_SIZE];
    uint8_t i, status;

    status = OW_SEARCH_FIRST;
    for (i = 0; status != OW_LAST_DEVICE && i < max_count; i++) {
        OW_FindROM(&status, id);

        if (status == OW_PRESENCE_ERR || status == OW_DATA_ERR)
            break;

        for (uint8_t j = 0; j < OW_ROMCODE_SIZE; j++)
            devices[i][j] = id[j];
    }

    return i;
}
