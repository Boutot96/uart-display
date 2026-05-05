#include "distance_read.h"
#include <time.h>
#include <unistd.h>


HCSR04 hcsr04_init(int trig_pin, int echo_pin) {
    HCSR04 sensor;
    sensor.chip = gpiod_chip_open_by_name("gpiochip0");
    sensor.trig = gpiod_chip_get_line(sensor.chip, trig_pin);
    sensor.echo = gpiod_chip_get_line(sensor.chip, echo_pin);
    gpiod_line_request_output(sensor.trig, "trig", 0);
    gpiod_line_request_input(sensor.echo, "echo");
    return sensor;
}

double read_distance(HCSR04 *sensor) {
    struct timespec pulse_start, pulse_end, current;

    gpiod_line_set_value(sensor->trig, 1);
    usleep(10);
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

void hcsr04_close(HCSR04 *sensor) {
    gpiod_line_release(sensor->trig);
    gpiod_line_release(sensor->echo);
    gpiod_chip_close(sensor->chip);
}
