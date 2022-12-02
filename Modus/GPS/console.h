/*
 * console.h
 *
 *  Created on: Jan 21, 2022
 *      Author: Joe Krachey
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"


#define PIN_CONSOLE_TX 	P9_1
#define PIN_CONSOLE_RX	P9_0


/* Public Function API */
void console_init(void);


#endif /* CONSOLE_H_ */
