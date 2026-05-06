#define _POSIX_C_SOURCE 199309L // Avoids error indicators when using CLOCK_MONOTONIC
#include "hcsr04.h"
#include <time.h>
#include <unistd.h>

// Initilize the HCSR04 sensor with the specified trigger and echo pins
// returns an initialized HCSR04 sensor struct
HCSR04 hcsr04_init(int trig_pin, int echo_pin) {
    HCSR04 sensor;
    sensor.chip = gpiod_chip_open_by_name("gpiochip0");
    sensor.trig = gpiod_chip_get_line(sensor.chip, trig_pin);
    sensor.echo = gpiod_chip_get_line(sensor.chip, echo_pin);
    gpiod_line_request_output(sensor.trig, "trig", 0);
    gpiod_line_request_input(sensor.echo, "echo");
    return sensor;
}

// Read the distance from the HCSR04 sensor
// returns the distance in centimeters, or -1 if an error occurred
double read_distance(HCSR04 *sensor) {
    struct timespec pulse_start, pulse_end, current;

    gpiod_line_set_value(sensor->trig, 1);
    sleep(1);
    gpiod_line_set_value(sensor->trig, 0);

    clock_gettime(CLOCK_MONOTONIC, &pulse_start);
    while (gpiod_line_get_value(sensor->echo) == 0){
        clock_gettime(CLOCK_MONOTONIC, &current);
        if ((current.tv_sec - pulse_start.tv_sec) > 1) return -1; // timeout
        clock_gettime(CLOCK_MONOTONIC, &pulse_start);
    }
    while (gpiod_line_get_value(sensor->echo) == 1){
        clock_gettime(CLOCK_MONOTONIC, &current);
        if ((current.tv_sec - pulse_start.tv_sec) > 1) return -1; // timeout
        clock_gettime(CLOCK_MONOTONIC, &pulse_end);
    }

    double elapsed = (pulse_end.tv_sec - pulse_start.tv_sec) +
                    (pulse_end.tv_nsec - pulse_start.tv_nsec) / 1e9;
    return elapsed * 17150;
}

// Close the HCSR04 sensor
// Releases the resources associated with the HCSR04 sensor.
void hcsr04_close(HCSR04 *sensor) {
    gpiod_line_release(sensor->trig);
    gpiod_line_release(sensor->echo);
    gpiod_chip_close(sensor->chip);
}
