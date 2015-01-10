#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU					8000000L	// CPU Frequency, Hz

// UART settings
#define BAUD					9600		// UART Baud Rate, BPS
#define UART_USE_2X				0			// Use 2x UART speed
#define UART_STDIN_ECHO			1			// Disable echo for stdin (eg. in scanf)
#define UART_FIX_LINE_ENDING	1			// Replace: \n -> \r\n

// 1-wire settings
#define MAXDEVICES				8			// Maximum number of devices on line
#define UART_AS_OneWire			0			// Emulate 1-wire with UART
#define OW_TWO_PINS				0			// Use 2 pins

#endif /* CONFIG_H_ */