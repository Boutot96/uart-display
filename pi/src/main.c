#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "hcsr04.h"
#include "ads_7830.h"

static volatile int running = 1;

void handle_sigint(int sig) {
    running = 0;
}

void test_hcsr04() {
    HCSR04 sensor = hcsr04_init(23, 24);
    
    while (running) {
        double distance = read_distance(&sensor);
        if (distance < 0) {
            printf("Timeout - no echo received\n");
        } else {
            printf("Distance: %.1f cm\n", distance);
        }
        sleep(1);
    }
    
    hcsr04_close(&sensor);
    printf("Cleaned up, exiting\n");
}

void test_ads_7830() {
    ADS_7830 adc = ads_7830_init(0x48);

    while (running) {
        int value = read_adc(&adc);
        if (value < 0) {
            printf("Error reading ADC\n");
        } else {
            printf("ADC Value: %d\n", value);
        }
        sleep(1);
    }

    ads_7830_close(&adc);
    printf("Cleaned up, exiting\n");
}



int main() {
    signal(SIGINT, handle_sigint);
    test_hcsr04();
    
    return 0;
}
