#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include "uart.h"
#include "config.h"
#include <util/setbaud.h>


// UART streams (eg. for printf)
FILE uart_stdout = FDEV_SETUP_STREAM(&UART_putc_stream, NULL, _FDEV_SETUP_WRITE);
FILE uart_stdin = FDEV_SETUP_STREAM(NULL, &UART_getc_stream, _FDEV_SETUP_READ);

inline void UART_init() {
	// Set baudrate
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	#if USE_2X
	UCSRA |= (1 << U2X);
	#else
	UCSRA &= ~(1 << U2X);
	#endif
	
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
	loop_until_bit_is_set(UCSRA, UDRE);
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
	loop_until_bit_is_set(UCSRA, RXC);
	// Return received data
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
	char c = UART_getc();
	#ifdef UART_STDIN_ECHO
	UART_putc(c);
	#endif
	return c;
}
