#ifndef DS18x20_H
#define DS18x20_H

#include "onewire.h"

// Return codes
#define DS18x20_OK      1
#define DS18x20_FAIL    0

// Command codes
#define THERM_CMD_CONVERTTEMP       0x44
#define THERM_CMD_RSCRATCHPAD       0xBE
#define THERM_CMD_WSCRATCHPAD       0x4E
#define THERM_CMD_CPYSCRATCHPAD     0x48
#define THERM_CMD_RECEEPROM         0xB8
#define THERM_CMD_RPWRSUPPLY        0xB4
#define THERM_CMD_ALARMSEARCH       0xEC


uint8_t DS18x20_StartMeasure(OW_ROM_t rom);                 //if rom==0 then skip rom
uint8_t DS18x20_ReadData(OW_ROM_t rom, uint8_t *buffer);    //if rom==0 then skip rom
uint8_t DS18x20_DataConvert(uint8_t* data, int8_t* temp);
uint8_t DS18x20_SetResolution(OW_ROM_t rom, uint8_t res);      // res = 9, 10, 11, or 12
void    DS18x20_WaitForMeasure(uint8_t res);

#endif
