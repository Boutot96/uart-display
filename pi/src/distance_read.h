#ifndef distance_read_H
#define distance_read_H

#include <gpiod.h>

typedef struct {
    struct gpiod_chip *chip;
    struct gpiod_line *trig;
    struct gpiod_line *echo;
} HCSR04;

HCSR04 hcsr04_init(int trig_pin, int echo_pin);
double read_distance(HCSR04 *sensor);
void hcsr04_close(HCSR04 *sensor);

#endif