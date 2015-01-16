#ifndef ONEWIRE_H
#define ONEWIRE_H

#include "config.h"
#include <avr/io.h>

// Port setup
#define OW_DDR    OW_PORT_LETTER(DDR)
#define OW_PORT   OW_PORT_LETTER(PORT)
#define OW_PIN    OW_PORT_LETTER(PIN)

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

void    OW_WriteBit(uint8_t bit);
uint8_t OW_ReadBit(void);
void    OW_WriteByte(uint8_t byte);
uint8_t OW_ReadByte(void);

uint8_t OW_SearchDevices(OW_ROM_t devices[], uint8_t max_count);
uint8_t OW_SelectROM(OW_ROM_t rom);

uint8_t OW_CRC8 (uint8_t *data, uint8_t count);

#endif
