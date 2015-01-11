#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU                   8000000L    // CPU Frequency, Hz

// UART settings
#define BAUD                    9600        // UART Baud Rate, BPS
#define UART_USE_2X             0           // Use 2x UART speed
#define UART_STDIN_ECHO         1           // Enable echo for stdin (eg. in scanf)
#define UART_FIX_LINE_ENDING    1           // Replace: \n -> \r\n

// 1-wire settings
#define OW_PORT_LETTER(x)       x##B        // Letter of 1-wire port (format: x##LETTER)
#define OW_BIT                  0           // Bit of 1-wire port

// ds18x20 settings
#define DS18X20_CHECK_CRC       1           // Check CRC

#endif /* CONFIG_H_ */
