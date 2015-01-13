#ifndef ONEWIRE_H
#define ONEWIRE_H

#include "config.h"
#include <avr/io.h>

// Useful macros
#define sbi(reg,bit)        reg |= (1<<bit)
#define cbi(reg,bit)        reg &= ~(1<<bit)
#define ibi(reg,bit)        reg ^= (1<<bit)

// Port setup
#define OW_DDR    OW_PORT_LETTER(DDR)
#define OW_PORT   OW_PORT_LETTER(PORT)
#define OW_PIN    OW_PORT_LETTER(PIN)

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

// Device codes
#define OW_DS1990_FAMILY_CODE    0x01
#define OW_DS2405_FAMILY_CODE    0x05
#define OW_DS2413_FAMILY_CODE    0x3A
#define OW_DS1822_FAMILY_CODE    0x22
#define OW_DS2430_FAMILY_CODE    0x14
#define OW_DS1990_FAMILY_CODE    0x01
#define OW_DS2431_FAMILY_CODE    0x2d
#define OW_DS18S20_FAMILY_CODE   0x10
#define OW_DS18B20_FAMILY_CODE   0x28
#define OW_DS2433_FAMILY_CODE    0x23

// ROM-code size including CRC
#define OW_ROMCODE_SIZE    8

typedef uint8_t OW_ROM_t [OW_ROMCODE_SIZE];

uint8_t OW_Reset(void);
void    OW_WriteBit(unsigned char bit);
uint8_t OW_ReadBit(void);
uint8_t OW_ReadByte(void);
void    OW_WriteByte(unsigned char byte);
uint8_t OW_SearchROM(unsigned char diff, unsigned char *id);
void    OW_FindROM(unsigned char *diff, unsigned char id[]);
uint8_t OW_ReadROM(unsigned char *buffer);
uint8_t OW_MatchROM(unsigned char *rom);
uint8_t OW_CRC8 (uint8_t *data, uint8_t count);
uint8_t OW_SearchDevices(OW_ROM_t devices[], uint8_t max_count);

#endif
