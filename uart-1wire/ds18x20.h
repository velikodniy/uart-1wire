#ifndef DS18x20_H
#define DS18x20_H

#include "onewire.h"


// Command codes
#define THERM_CMD_CONVERTTEMP       0x44
#define THERM_CMD_RSCRATCHPAD       0xBE
#define THERM_CMD_WSCRATCHPAD       0x4E
#define THERM_CMD_CPYSCRATCHPAD     0x48
#define THERM_CMD_RECEEPROM         0xB8
#define THERM_CMD_RPWRSUPPLY        0xB4
#define THERM_CMD_ALARMSEARCH       0xEC

// Constants
#define THERM_DECIMAL_STEPS_12BIT   625 //.0625


uint8_t DS18x20_StartMeasure(uint8_t* rom);                 //if rom==0 then skip rom
uint8_t DS18x20_ReadData(uint8_t *rom, uint8_t *buffer);    //if rom==0 then skip rom
void DS18x20_DataConvert(uint8_t* data, int8_t* temp);

#endif
