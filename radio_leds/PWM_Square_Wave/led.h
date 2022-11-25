#ifndef __LED_H__
#define __LED_H__

#include "cyhal.h"

#define FREQUENCY 2000
#define NUMLEDS 4
#define NUM_MODES 4

enum colors{
	RED,
	GREEN,
	BLUE,
	WHITE
};

void pwm_init_all();

void pwm_duty_cycle(int color, float duty_cycle);

void pwm_duty_cycle_blue(int speed);

void sweep(int speed);

void pulse_all(int speed);

void pulse_color(int speed);

void pulse_party(int speed);

//void (*mode[NUM_MODES])(int) = {sweep, pulse_all, pulse_color, pulse_party};

#endif
