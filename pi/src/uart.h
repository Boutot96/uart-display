#ifndef UART_H
#define UART_H

int uart_init(const char *device, int baud);
void uart_close(int fd);
int uart_write(int fd, const char *data);
int uart_read(int fd, char *data);

#endif // UART_H