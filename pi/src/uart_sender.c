#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "distance_read.h"

static volatile int running = 1;

void handle_sigint(int sig) {
    running = 0;
}

int main() {
    signal(SIGINT, handle_sigint);
    
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
    return 0;
}
