#include "config.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "onewire.h"
#include "ds18x20.h"

#define MAXDEVICES 8 // Maximum number of devices on line

uint8_t     num_of_devices;
OW_ROM_t    devices[MAXDEVICES];

void interactive(void);
void print_address(uint8_t* addr);
void print_devices_list(void);

int main(void)
{
    char cmd;
    
    UART_init();
    num_of_devices = OW_SearchDevices(devices, MAXDEVICES);
    
    // Main loop
    while (1) {
        cmd = getchar();
        switch(cmd) {
        case 'i':
            interactive();
            break;
        }
    }
}

void interactive(void) {
    puts_P(PSTR("\n1wire-UART bridge"));
    puts_P(PSTR("Commands: Quit, Devices"));
    char cmd;
    while(1) {
        putchar('>');
        cmd = getchar();
        putchar('\n');
        switch(cmd) {
        case 'q':
            return;
        case 'd':
            print_devices_list();
            break;
        default:
            puts_P(PSTR("Unknown command"));
        }
    }
}

void print_address(uint8_t* addr) {
    for (uint8_t i = 0; i < 8; i++)
        printf_P(PSTR("%.2X "), addr[i]);
}

void print_devices_list(void)
{
    printf_P(PSTR("Found %d device(s)\n"), num_of_devices);

    for (uint8_t i = 0; i < num_of_devices; i++) {
        print_address(devices[i]);
        switch (devices[i][0]) {
        case OW_DS18B20_FAMILY_CODE:
            printf_P(PSTR("Thermometer DS18B20"));
            DS18x20_SetResolution(devices[i], 12);
            DS18x20_StartMeasure(devices[i]);
            DS18x20_WaitForMeasure(12);
            uint8_t data[2];
            DS18x20_ReadData(devices[i], data);
            int8_t t[2];
            DS18x20_DataConvert(data, t);
            printf_P(PSTR(": %d,%02d C"), t[0], t[1]);
            break;
        
        case OW_DS18S20_FAMILY_CODE:
            printf_P(PSTR("Thermometer DS18S20"));
            break;

        case OW_DS1990_FAMILY_CODE:
            printf_P(PSTR("Serial button DS1990"));
            break;
        
        case OW_DS2430_FAMILY_CODE:
            printf_P(PSTR("EEPROM DS2430"));
            break;
        
        case OW_DS2413_FAMILY_CODE:
            printf_P(PSTR("Switch 2413"));
            break;
            
        default:
            printf_P(PSTR("Unknown device"));
        }
        
        printf_P(PSTR("\n"));
    }
}