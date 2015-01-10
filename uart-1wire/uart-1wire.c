#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

int main(void)
{
	UART_init();
	
	printf("Echo:\n");
	
	char c;
	while(1) {
		UART_putc('>');
		c = UART_getc();
		UART_putc(c);
		UART_putc('\n');
		UART_putc('\r');
	}
}
