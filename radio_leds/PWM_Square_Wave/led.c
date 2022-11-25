#include "led.h"
#include <stdlib.h>

cyhal_pwm_t leds[4];

//rgbw
int pins[4] = {P10_6, P10_2, P12_6, P5_6};

void pwm_init_all()
{
	cy_rslt_t rslt;
    for(int color = 0; color < 3; color++){
    	rslt = cyhal_pwm_init(&leds[color], pins[color], NULL);
        CY_ASSERT(CY_RSLT_SUCCESS == rslt);

        rslt = cyhal_pwm_set_duty_cycle(&leds[color], 100, FREQUENCY);
        CY_ASSERT(CY_RSLT_SUCCESS == rslt);

        rslt = cyhal_pwm_start(&leds[color]);
        CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    }
}

void pwm_duty_cycle_blue(int speed)
{
	cyhal_pwm_set_duty_cycle(&leds[BLUE], 100 - ((speed / 255) * 100), FREQUENCY);
	cyhal_pwm_start(&leds[BLUE]);
}

void reset()
{
    for(int color = 0; color < NUMLEDS; color++){
    	cyhal_pwm_set_duty_cycle(&leds[color], 100, FREQUENCY);
    }
}

void sweep(int speed)
{
    static int call_count;
    static int color = 0;
    static int intensity = 0;

    //update intensity with speed
    if(call_count++ % (FREQUENCY / (4 * (speed / 255)))  == 0) {
        reset();
        cyhal_pwm_set_duty_cycle(&leds[color], intensity++ % 100, FREQUENCY);
        if(intensity % 100 == 0) {
            color = color++ % NUMLEDS;
        }
    }
}

void pulse_all(int speed)
{
    static int call_count;
    static int intensity = 0;
    //update intensity with speed
    if(call_count++ % (FREQUENCY / (4 * (speed / 255)))  == 0) {
        reset();
        for(int color = 0; color < NUMLEDS; color++){
        	cyhal_pwm_set_duty_cycle(&leds[color], intensity++ % 100, FREQUENCY);
        }
    }
}

void pulse_color(int speed)
{
	int color = BLUE;
	static int call_count = 0;
    static int intensity = 0;
    //update intensity with speed
    if(call_count++ % (FREQUENCY / (4 * (speed / 255)))  == 0) {
        reset();
        //fun line! intensity will eventually overflow but we don't care because we'll still have 
        // a value out of 100 that is always positive
        cyhal_pwm_set_duty_cycle(&leds[color], intensity++ % 100, FREQUENCY);
    }
}

void pulse_party(int speed)
{
	static int call_count = 0;
    static int intensity = 0;
    int rand_color = rand() % 4;
    //update intensity with speed
    if(call_count++ % (FREQUENCY / (4 * (speed / 255)))  == 0) {
        reset();
        cyhal_pwm_set_duty_cycle(&leds[rand_color], intensity++ % 100, FREQUENCY);
    }
}

/*
Ok but what modes could we have?

MODE:
    SR: all others on but sweep red
    SG: all others on but sweep green
    SB: all others on but sweep blue
    SW: all others on but sweep white
    Party: random color every second
    pulse sweep: sweep through colors while pulsing dim to bright


*/
