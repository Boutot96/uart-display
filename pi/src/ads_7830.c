#include "ads_7830.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>


ADS_7830 ads_7830_init(int address) {
    ADS_7830 adc;
    adc.fd = open("/dev/i2c-1", O_RDWR);
    adc.address = address;
    return adc;
}


int read_adc(ADS_7830 *adc) {
    int value;

    int fd = open("/dev/i2c-1", O_RDWR);
    ioctl(fd, I2C_SLAVE, adc->address);

    // Read the ADC value
    // This is a simplified example; you may need to adjust based on your specific ADC
    read(fd, &value, sizeof(value));

    close(fd);
    return value;
}

void ads_7830_close(ADS_7830 *adc) {
    close(adc->fd);
}