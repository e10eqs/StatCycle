
#ifndef PUSH_BUTTON_H_
#define PUSH_BUTTON_H_

#include "cyhal.h"

// Pin definitions for the ECE453 Staff Dev board
#define PIN_PUSH_BUTTON				P5_0 /* ADD CODE */
#define INT_PRIORITY_PUSH_BUTTON	0

// Exported Global Variables
extern uint8_t led_mode;

/* Public Function API */
void push_button_init();

#endif
