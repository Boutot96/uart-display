#include "uart.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

int uart_init(const char *device, int baud) {
    int fd = open(device, O_RDWR | O_NOCTTY);
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(fd, TCSANOW, &options);
    return fd;
}

void uart_send(int fd, const char *packet) {
    write(fd, packet, strlen(packet));
}

void uart_read(int fd, char *packet) {
    read(fd, packet, strlen(packet));
}

void uart_close(int fd) {
    close(fd);
}