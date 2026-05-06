#include "ads_7830.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>


ADS_7830 ads_7830_init(int address) {
    ADS_7830 adc;
    adc.fd = open("/dev/i2c-1", O_RDWR);
    adc.address = address;
    return adc;
}


float read_adc(ADS_7830 *adc, int channel) {

    // Read the ADC value
    ioctl(adc->fd, I2C_SLAVE, adc->address);
    // Send command byte to select channel
    uint8_t command = 0x84 | (channel << 4);
    write(adc->fd, &command, 1);

    // Read single byte result
    uint8_t value;
    read(adc->fd, &value, 1);
    //int8_t result = (uint8_t)((value / 255) * 3.3);
    
    return (float)value/255.0 * 3.3;
}


void ads_7830_close(ADS_7830 *adc) {
    close(adc->fd);
}