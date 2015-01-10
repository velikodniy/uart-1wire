#ifndef UART_H_
#define UART_H_

#include <stdio.h>

void UART_init();
void UART_putc(char c);
void UART_puts(char* s);
char UART_getc(void);
int UART_putc_stream(char c, FILE *stream);
int UART_getc_stream(FILE *stream);

#endif /* UART_H_ */
