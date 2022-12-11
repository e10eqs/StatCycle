#ifndef __LED_H__
#define __LED_H__

#include "cyhal.h"

#define RADIO_FREQUENCY 10000
#define FREQUENCY 1000
#define NUMLEDS 4
#define NUM_MODES 12

enum colors{
	WHITE,
	GREEN,
	BLUE,
	RED
};

extern cyhal_pwm_t leds[NUMLEDS];

extern int pins[NUMLEDS];

void led_interrupt();

void pwm_init_all();

void pwm_duty_cycle(int color, float duty_cycle);

void pwm_duty_cycle_red(uint8_t speed);

void pwm_duty_cycle_blue(uint8_t speed);

void pwm_duty_cycle_green(uint8_t speed);

void pwm_duty_cycle_white(uint8_t speed);

void pulse_all(uint8_t speed);

void pulse_color(uint8_t speed);

void pulse_party(uint8_t speed);

void blink_red(uint8_t speed);

void blink_green(uint8_t speed);

void blink_blue(uint8_t speed);

void blink_white(uint8_t speed);

void blink_all(uint8_t speed);

void buckets(uint8_t speed);

#endif
