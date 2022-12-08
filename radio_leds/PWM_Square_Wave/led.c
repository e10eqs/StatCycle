#include "led.h"
#include "cyhal.h"

cyhal_pwm_t leds[NUMLEDS];

//wgbr
int pins[NUMLEDS] = { P5_6, P10_2, P12_6, P10_6 };

uint8_t led_mode;
cy_rslt_t rslt;

void (*mode[NUM_MODES])(
		uint8_t) = {pwm_duty_cycle_red, pwm_duty_cycle_green, pwm_duty_cycle_blue, pwm_duty_cycle_white, sweep, pulse_all, pulse_color, pulse_party
		};
bool buffer[8];

/* Timer object used */
cyhal_timer_t timer_obj;
static void isr_radio_receiver(void *callback_arg, cyhal_timer_event_t event) {
	(void) event;
	enum state_t {
		WAITING, STARTBIT, BUFFERIN, STOPBIT_1, STOPBIT_2, INVALID, VALID
	};
	static uint8_t speed = 0;
	static int state = WAITING;
	static uint i = 0;

	static uint8_t j = 0;
	(*mode[led_mode % NUM_MODES])(j++ %255);
	bool in = cyhal_gpio_read(P10_4);
	switch (state) {
	case WAITING:
		i = 0;
		state = in ? STARTBIT : WAITING;
		break;
	case STARTBIT:
		state = in ? BUFFERIN : WAITING;
		break;
	case BUFFERIN:
		buffer[i++] = in;
		state = (i == 8) ? STOPBIT_1 : BUFFERIN;
		break;
	case STOPBIT_1:
		state = in ? STOPBIT_2 : INVALID;
		break;
	case STOPBIT_2:
		state = in ? VALID : INVALID;
		break;
	case INVALID:
		state = WAITING;
		break;
	case VALID:
		speed = 0;
		for (int i = 0; i < 8; i++) {
			speed |= buffer[i] << i;
		}
		cyhal_gpio_write(P5_5, speed);
		state = WAITING;
		break;
	}
}

void led_interrupt() {
	cy_rslt_t rslt;
	const cyhal_timer_cfg_t timer_cfg = { .compare_value = 0, /* Timer compare value, not used */
	.period = 100, /* Defines the timer period */
	.direction = CYHAL_TIMER_DIR_UP, /* Timer counts up */
	.is_compare = false, /* Don't use compare mode */
	.is_continuous = true, /* Run the timer indefinitely */
	.value = 0 /* Initial value of counter */
	};
	/* Initialize the timer object. Does not use pin output ('pin' is NC) and
	 * does not use a pre-configured clock source ('clk' is NULL). */
	rslt = cyhal_timer_init(&timer_obj, NC, NULL);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	/* Apply timer configuration such as period, count direction, run mode, etc. */
	rslt = cyhal_timer_configure(&timer_obj, &timer_cfg);
	/* Set the frequency of timer to 10000 Hz */
	rslt = cyhal_timer_set_frequency(&timer_obj, RADIO_FREQUENCY);
	/* Assign the ISR to execute on timer interrupt */
	cyhal_timer_register_callback(&timer_obj, isr_radio_receiver, NULL);
	/* Set the event on which timer interrupt occurs and enable it */
	cyhal_timer_enable_event(&timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 0,
			true);
	/* Start the timer with the configured settings */
	rslt = cyhal_timer_start(&timer_obj);
}

void pwm_init_all() {
	volatile cy_rslt_decode_t result;
	for (int color = 0; color < NUMLEDS; color++) {
		result.raw = cyhal_pwm_init(&leds[color], pins[color], NULL);
		CY_ASSERT(CY_RSLT_SUCCESS == result.raw);
	}
}

void pwm_stop_all() {
	for (int color = 0; color < NUMLEDS; color++) {
		cy_rslt_t rslt = cyhal_pwm_stop(&leds[color]);
		CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	}
}

float set_speed_range(uint8_t speed) {
	return 50 - ((speed * 50 / 255));
}

void pwm_duty_cycle_red(uint8_t speed) {
	pwm_stop_all();
	float set_speed = set_speed_range(speed);
	cy_rslt_t rslt = cyhal_pwm_set_duty_cycle(&leds[RED], set_speed, FREQUENCY);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	rslt = cyhal_pwm_start(&leds[RED]);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

void pwm_duty_cycle_blue(uint8_t speed) {
	pwm_stop_all();
	float set_speed = set_speed_range(speed);
	cy_rslt_t rslt = cyhal_pwm_set_duty_cycle(&leds[BLUE], set_speed,
			FREQUENCY);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	rslt = cyhal_pwm_start(&leds[BLUE]);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

void pwm_duty_cycle_green(uint8_t speed) {
	pwm_stop_all();
	float set_speed = set_speed_range(speed);
	cy_rslt_t rslt = cyhal_pwm_set_duty_cycle(&leds[GREEN], set_speed,
			FREQUENCY);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	rslt = cyhal_pwm_start(&leds[GREEN]);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

void pwm_duty_cycle_white(uint8_t speed) {
	pwm_stop_all();
	float set_speed = set_speed_range(speed);
	cy_rslt_t rslt = cyhal_pwm_set_duty_cycle(&leds[WHITE], set_speed,
			FREQUENCY);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	rslt = cyhal_pwm_start(&leds[WHITE]);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

uint8_t set_speed_frequency(uint8_t speed) {
	/*interrupt every .01 seconds
	 want it to sweep at different rates depending on speed
	 speed /25 will be at most 10 and at least 0, so I think that's decent I'll take 11 - speed / 25
	 so that a higher speed will result in a shorter period
	 */
	return 11 - speed / 25;
}

void sweep(uint8_t speed) {
	static uint8_t call_count;
	static int color = 0;
	static uint8_t intensity = 0;
	pwm_stop_all();

	//update intensity with speed
	if (call_count++ % set_speed_frequency(speed) == 0) {
		intensity += 5;
		cyhal_pwm_set_duty_cycle(&leds[color], intensity % 100, FREQUENCY);
		cyhal_pwm_start(&leds[color]);
		if (intensity % 100 == 0) {
			color = color++ % NUMLEDS;
		}
	}
}

void pulse_all(uint8_t speed) {
	static uint8_t call_count;
	static uint8_t intensity = 0;
	//update intensity with speed
	if (call_count++ % set_speed_frequency(speed) == 0) {
		intensity += 5;
		for (int color = 0; color < NUMLEDS; color++) {
			cyhal_pwm_stop(&leds[color]);
			cyhal_pwm_set_duty_cycle(&leds[color], intensity % 100, FREQUENCY);
			cyhal_pwm_start(&leds[color]);
		}
	}
}

void pulse_color(uint8_t speed) {
	int color = BLUE;
	static int call_count = 0;
	static uint8_t intensity = 0;
	//update intensity with speed
	if (call_count++ % set_speed_frequency(speed) == 0) {
		intensity += 5;
		cyhal_pwm_stop(&leds[color]);
		//fun line! intensity will eventually overflow but we don't care because we'll still have
		// a value out of 100 that is always positive
		cyhal_pwm_set_duty_cycle(&leds[color], intensity % 100, FREQUENCY);
		cyhal_pwm_start(&leds[color]);
	}
}

void pulse_party(uint8_t speed) {
	static int call_count = 0;
	static uint8_t intensity = 0;
	int rand_color = rand() % 4;
	//update intensity with speed
	if (call_count++ % set_speed_frequency(speed) == 0) {
		intensity += 5;
		cyhal_pwm_stop(&leds[rand_color]);
		cyhal_pwm_set_duty_cycle(&leds[rand_color], intensity % 100, FREQUENCY);
		cyhal_pwm_start(&leds[rand_color]);
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
