#include <avr/io.h>
#include "uart.h"

#define F_CPU			8000000L
#define UART_BAUDRATE	9600
#define UART_UBRR		(F_CPU/16/UART_BAUDRATE-1)

inline void UART_init() {
	UBRRH = UART_UBRR >> 8;
	UBRRL = UART_UBRR;
	
	// RX enabled, TX enabled
	UCSRB = (1<<RXEN) | (1<<TXEN);
	
	// Set frame format: 8 data, 1 stop bit, parity check disabled
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}

// Send char
void UART_putc(char c)
{
	// Wait until USART data register (UDR) is empty
	while((UCSRA & (1 << UDRE)) == 0);
	// Send data
	UDR = c;
}

// Send string
void UART_puts(char* s) {
	while(*s)
		UART_putc(*s++);
}

// Receive char
char UART_getc(void)
{
	// Wait for data to be received
	while ((UCSRA & (1 << RXC)) == 0);
	// Get and return received data from buffer
	return UDR;
}
