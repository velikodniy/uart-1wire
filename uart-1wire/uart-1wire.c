#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"

#include <math.h>

void interactive(void);

int main(void)
{
    char cmd;
    
    UART_init();
    
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
    char cmd;
    while(1) {
        putchar('>');
        cmd = getchar();
        putchar('\n');
        switch(cmd) {
        case 'q':
            return;
        default:
            puts_P(PSTR("Unknown command"));
        }
    }
}

// Device processing
/*
#define MAXDEVICES                8            // Maximum number of devices on line

void print_address(unsigned char* address) {
    printf("%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X", address[0],address[1],address[2],address[3],address[4],address[5],address[6],address[7]);
}

unsigned char    nDevices;    // количество сенсоров
unsigned char    owDevicesIDs[MAXDEVICES][8];    // Их ID

unsigned char search_ow_devices(void) // поиск всех устройств на шине
{
    unsigned char    i;
    unsigned char    id[OW_ROMCODE_SIZE];
    unsigned char    diff, sensors_count;

    sensors_count = 0;

    for( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && sensors_count < MAXDEVICES ; )
    {
        OW_FindROM( &diff, &id[0] );

        if( diff == OW_PRESENCE_ERR ) break;

        if( diff == OW_DATA_ERR )    break;

        for (i=0;i<OW_ROMCODE_SIZE;i++)
        owDevicesIDs[sensors_count][i] = id[i];
        
        sensors_count++;
    }
    return sensors_count;

}
unsigned char    themperature[3]; // в этот массив будет записана температура
int main1(void)
{
    stdout = &usart_str; // указываем, куда будет выводить printf

    DDRB = 0b00000010; PORTB = 0b00000010;
    DDRC = 0b00000000; PORTC = 0b00000000;
    DDRD = 0b00000010; PORTD = 0b00000000;

    USART_init(); // включаем uart

    timerDelayInit();

    nDevices = search_ow_devices(); // ищем все устройства

    printf("---------- Found %d devices ----------", nDevices);


    for (unsigned char i=0; i<nDevices; i++) // теперь сотируем устройства и запрашиваем данные
    {
        // узнать устройство можно по его груповому коду, который расположен в первом байте адресса
        switch (owDevicesIDs[i][0])
        {
            case OW_DS18B20_FAMILY_CODE: { // если найден термодатчик DS18B20
                printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
                printf(" - Thermometer DS18B20"); // печатаем тип устройства
                DS18x20_StartMeasure(owDevicesIDs[i]); // запускаем измерение
                timerDelayMs(800); // ждем минимум 750 мс, пока конвентируется температура
                unsigned char    data[2]; // переменная для хранения старшего и младшего байта данных
                DS18x20_ReadData(owDevicesIDs[i], data); // считываем данные
                
                float t = DS18x20_ConvertToThemperatureFl(data); // преобразовываем температуру в человекопонятный вид
                printf(": %3.2f C", t);
            } break;
            case OW_DS18S20_FAMILY_CODE: { // если найден термодатчик DS18B20
                printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
                printf(" - Thermometer DS18S20"); // печатаем тип устройства
            } break;

            case OW_DS1990_FAMILY_CODE: { // если найден электронный ключ DS1990
                printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
                printf(" - Serial button DS1990"); // печатаем тип устройства
            } break;
            case OW_DS2430_FAMILY_CODE: { // если найдена EEPROM
                printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
                printf(" - EEPROM DS2430"); // печатаем тип устройства
            } break;
            case OW_DS2413_FAMILY_CODE: { // если найден ключ
                printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
                printf(" - Switch 2413"); // печатаем тип устройства
            } break;
        }

    }
*/

