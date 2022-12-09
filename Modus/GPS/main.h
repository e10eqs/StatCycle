/*
 * main.h
 *
 *  Created on: Aug 16, 2022
 *      Author: Joe Krachey
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "console.h"
#include "i2c.h"
#include "gps.h"
#include "display.h"
#include "push_button.h"

#include "./generic/ff.h"		/* Declarations of FatFs API */

extern FRESULT fr;
extern FIL Fil;


#endif /* MAIN_H_ */
