#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "hcsr04.h"
#include "ads_7830.h"
#include "uart.h"

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
    ADS_7830 adc = ads_7830_init(0x4b);

    while (running) {
        float value = read_adc(&adc, 0);
        if (value < 0) {
            printf("Error reading ADC\n");
        } else {
            printf("ADC Value: %.2f V\n", value);
        }
        sleep(1);
    }

    ads_7830_close(&adc);
    printf("Cleaned up, exiting\n");
}



int main() {
    signal(SIGINT, handle_sigint);
    //test_hcsr04();
    //test_ads_7830();

    
    char packet[100];

    // Initialize UART, serial and baud rate
    int uart_fd = uart_init("/dev/serial0", 115200);

    // Initialize ADC and HCSR04
    ADS_7830 adc = ads_7830_init(0x4b);
    HCSR04 sensor = hcsr04_init(23, 24);

    // Run until interrupted
    while (running){
        
        float value = read_adc(&adc, 0);
        if (value < 0) {
            printf("Error reading ADC\n");
            snprintf(packet, sizeof(packet), "<ERROR>\n");
            // Send the error packet
            
        }
        else {
            printf("ADC Value: %.2f V\n", value);
            snprintf(packet, sizeof(packet), "<VOLT:%.2f>\n", value);
            
        }
        uart_send(uart_fd, packet);

        usleep(100000); // 100ms delay
        double distance = read_distance(&sensor);
        if (distance < 0) {
            printf("Timeout - no echo received\n");
            snprintf(packet, sizeof(packet), "<TIMEOUT>\n");    
            // Send the timeout packet

        } 
        else {
            printf("Distance: %.1f cm\n", distance);
            snprintf(packet, sizeof(packet), "<DIST:%.1f>\n", distance);
        }
        uart_send(uart_fd, packet);
        usleep(100000); // 100ms delay
    }
    
    hcsr04_close(&sensor);
    ads_7830_close(&adc);

    printf("Cleaned up, exiting\n");

    return 0;
}
