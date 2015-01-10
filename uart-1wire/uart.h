#ifndef UART_H_
#define UART_H_

void UART_init();
void UART_putc(char c);
void UART_puts(char* s);
char UART_getc(void);

#endif /* UART_H_ */