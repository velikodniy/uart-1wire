#include "config.h"
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include "onewire.h"

// Pull (mode=1) or release (mode=0) line
void OW_Set(unsigned char mode)
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
unsigned char OW_CheckIn(void)
{
    return bit_is_set(OW_PIN, OW_BIT);
}

unsigned char OW_Reset(void)
{
    unsigned char status;
    OW_Set(1);
    _delay_us(480);
    
    OW_Set(0);
    _delay_us(60);
    //Store line value and wait until the completion of 480uS period
    status = OW_CheckIn();
    _delay_us(420);
    
    //Return the value read from the presence pulse (0=OK, 1=WRONG)
    return !status;
    //    return 1 if found
    //    return 0 if not found
}

void OW_WriteBit(unsigned char bit)
{
    //Pull line low for 1uS
    OW_Set(1);
    _delay_us(1);
    //If we want to write 1, release the line (if not will keep low)
    if(bit) OW_Set(0); 
    //Wait for 60uS and release the line
    _delay_us(60);
    OW_Set(0);
}

unsigned char OW_ReadBit(void)
{
    unsigned char bit=0;
    //Pull line low for 1uS
    OW_Set(1);
    _delay_us(1);
    //Release line and wait for 14uS
    OW_Set(0);
    _delay_us(14);
    //Read line value
    if(OW_CheckIn()) bit=1;
    //Wait for 45uS to end and return read value
    _delay_us(45);
    return bit;
}

void OW_WriteByte(unsigned char byte)
{
    for (unsigned char i=0; i<8; i++)
        OW_WriteBit(bit_is_set(byte, i));
}

unsigned char OW_ReadByte(void)
{
    unsigned char n=0;
    for (unsigned char i=0; i<8; i++)
        if (OW_ReadBit())
            sbi(n, i);
    return n;
}

unsigned char OW_SearchROM(unsigned char diff, unsigned char *id)
{     
    unsigned char i, j, next_diff;
    unsigned char b;

    if(!OW_Reset()) 
        return OW_PRESENCE_ERR;       // error, no device found

    OW_WriteByte(OW_CMD_SEARCHROM);     // ROM search command
    next_diff = OW_LAST_DEVICE;      // unchanged on last device
    
    i = OW_ROMCODE_SIZE * 8;         // 8 bytes
    do {    
        j = 8;                        // 8 bits
        do { 
            b = OW_ReadBit();            // read bit
            if( OW_ReadBit() ) {
                // read complement bit
                if( b )                 // 11
                    return OW_DATA_ERR;  // data error
            } else { 
                if( !b ) { // 00 = 2 devices
                    if( diff > i || ((*id & 1) && diff != i) ) { 
                        b = 1;               // now 1
                        next_diff = i;       // next pass 0
                    }
                }
            }
            OW_WriteBit( b );               // write bit
            *id >>= 1;
            if( b )
                *id |= 0x80;            // store bit
            i--;
        } while( --j );
        id++;                            // next byte
    } while( i );
    return next_diff;                  // to continue search
}

void OW_FindROM(unsigned char *diff, unsigned char id[])
{
    while(1) {
        *diff = OW_SearchROM( *diff, &id[0] );
        if (*diff == OW_PRESENCE_ERR ||
            *diff == OW_DATA_ERR     ||
            *diff == OW_LAST_DEVICE  ) return;
        
        //if ( id[0] == DS18B20_ID || id[0] == DS18S20_ID ) 
        return; // FIXME: ?????
    }
}

unsigned char OW_ReadROM(unsigned char *buffer)
{
    if (!OW_Reset())
        return 0;
    OW_WriteByte(OW_CMD_READROM);
    for (unsigned char i=0; i<8; i++)
        buffer[i] = OW_ReadByte();
    return 1;
}

unsigned char OW_MatchROM(unsigned char *rom)
{
    if (!OW_Reset()) return 0;
    OW_WriteByte(OW_CMD_MATCHROM);    
    for(unsigned char i=0; i<8; i++)
        OW_WriteByte(rom[i]);
    return 1;
}
