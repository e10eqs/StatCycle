/*
 *  Created on: Jan 18, 2022
 *      Author: Joe Krachey
 */

#ifndef LEDS_H__
#define LEDS_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

// Pin definitions for the ECE453 Staff Dev board
#define PIN_USER_LED P5_5	/* ADD CODE */


/* Initializes the IO functions */
void leds_init(void);

#endif 
