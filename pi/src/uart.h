#ifndef UART_H
#define UART_H

int uart_init(const char *device, int baud);
void uart_close(int fd);
void uart_send(int fd, const char *data);
void uart_read(int fd, char *data);

#endif // UART_H 