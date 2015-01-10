#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

int main(void)
{
	UART_init();
	
	UART_puts("Echo: \r\n");
	
	char c;
	while(1) {
		c = UART_getc();
		UART_putc('>');
		UART_putc(c);
		UART_putc('\n');
		UART_putc('\r');
	}
}
