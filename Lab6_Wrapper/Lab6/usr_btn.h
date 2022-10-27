/*
 * usr_btn.h
 *
 *  Created on: Sep 9, 2022
 *      Author: Joe Krachey
 */

#ifndef USR_BTN_H_
#define USR_BTN_H_

#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "cybsp.h"
#include "cyhal.h"
#include "main.h"

extern uint8_t BTN_COUNT;

cy_rslt_t usr_btn_init(void);



#endif /* USR_BTN_H_ */
