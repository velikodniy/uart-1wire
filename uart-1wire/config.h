#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU			8000000L	// CPU Frequency, Hz
#define BAUD			9600		// UART Baud Rate, BPS
#define USE_2X			0			// Use 2x UART speed
#define UART_STDIN_ECHO	1			// Disable echo for stdin (eg. in scanf)

#endif /* CONFIG_H_ */