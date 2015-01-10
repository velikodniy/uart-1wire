#include <avr/io.h>
#include "uart.h"
#include "config.h"

#include <stdio.h>

#define UART_UBRR		(F_CPU/16/UART_BAUDRATE-1)

// UART streams (eg. for printf)
FILE uart_stdout = FDEV_SETUP_STREAM(&UART_putc_stream, NULL, _FDEV_SETUP_WRITE);
FILE uart_stdin = FDEV_SETUP_STREAM(NULL, &UART_getc_stream, _FDEV_SETUP_READ);

inline void UART_init() {
	UBRRH = UART_UBRR >> 8;
	UBRRL = UART_UBRR;
	
	// RX enabled, TX enabled
	UCSRB = (1<<RXEN) | (1<<TXEN);
	
	// Set frame format: 8 data, 1 stop bit, parity check disabled
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	
	// Standard streams redirection
	stdout = &uart_stdout;
	stdin = &uart_stdin;
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

// Put char into stream (for FDEV_SETUP_STREAM)
int UART_putc_stream(char c, FILE *stream) {
	if (c == '\n')
		UART_putc('\r');
	UART_putc(c);
	return 0;
}

// Get char from stream (for FDEV_SETUP_STREAM)
int UART_getc_stream(FILE *stream) {
	return UART_getc();
}
