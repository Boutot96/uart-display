#ifndef ADS_7830_H
#define ADS_7830_H

typedef struct {
    int fd;
    int address;
} ADS_7830;

ADS_7830 ads_7830_init(int address);
int read_adc(ADS_7830 *adc);
void ads_7830_close(ADS_7830 *adc);

#endif // ADS_7830_H