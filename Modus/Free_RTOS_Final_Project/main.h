#ifndef MAIN_H_
#define MAIN_H_

#include "cyhal.h"
#include "cybsp.h"
#include "../main_board_pins.h"
#include "display.h"

/* RTOS header files */
#include "FreeRTOS.h"
#include "task.h"
#include <semphr.h>
#include "task_redraw_display.h"
#include "task_update_display.h"
#include "task_radio_transmitter.h"

typedef struct {
	bool loading;
	uint8_t display[4];
} Display;

extern QueueHandle_t Queue_Display;
extern QueueHandle_t Queue_Speed;

#endif /* MAIN_H_ */
