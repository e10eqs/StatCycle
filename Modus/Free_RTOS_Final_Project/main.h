#ifndef MAIN_H_
#define MAIN_H_

#include "cyhal.h"
#include "cybsp.h"
#include "cy_pdl.h"
#include "../main_board_pins.h"
#include "display.h"
#include "i2c.h"
#include "gps.h"
#include "ff.h"

/* RTOS header files */
#include "FreeRTOS.h"
#include "task.h"
#include <semphr.h>
#include "task_redraw_display.h"
#include "task_radio_transmitter.h"
#include "task_dummy_velocity.h"
#include "task_state_machine.h"
#include "buttons.h"
#include "task_gps_sd_card.h"
#include "task_ble.h"

enum states {LOADING, FLASH, NUM, OFF};

typedef struct {
	enum states current_state;
	uint8_t display[4];
} Display;

enum buttons {START_RIDE, PAIRING, NONE};

extern QueueHandle_t Queue_Display;
extern QueueHandle_t Queue_Speed;
extern QueueHandle_t Queue_Buttons;

extern TaskHandle_t task_Redraw_Display_handle;
extern TaskHandle_t task_Dummy_Velocity_handle;
extern TaskHandle_t task_Radio_Transmitter_handle;
extern TaskHandle_t task_button_queue_send_handle;
extern TaskHandle_t task_gps_sd_card_handle;
extern TaskHandle_t task_ble_findme_process_handle;
extern TaskHandle_t task_state_machine_handle;

extern FATFS FatFs;		/* FatFs work area needed for each volume */
extern FIL Fil;			/* File object needed for each open file */
extern FRESULT fr;

#endif /* MAIN_H_ */
