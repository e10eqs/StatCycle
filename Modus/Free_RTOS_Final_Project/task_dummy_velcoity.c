#include "task_dummy_velocity.h"

void Task_Dummy_Velocity(void *pvParameters) {

	enum pvtState_e state = NO_FIX;
	Display d = { NUM, { 0, 0, 0, 0 } };
	enum states new_display_state = NUM;

	while (1) {
		state = getPVT();
		switch (state) {
		case NO_FIX:
			new_display_state = FLASH;
			if(new_display_state != d.current_state){
				d.current_state = new_display_state;
				xQueueSend(Queue_Display, &d, portMAX_DELAY);
			}
			break;
		case VALID_FIX:
			if (Fil == NULL) {
				Fil = malloc(sizeof(FIL));
				int sz = sizeof(char) * 200;
				char* file_name = malloc(sz);
				snprintf(file_name, sz, "%d-%02d-%02dT%02d-%02d.txt", pvt.year, pvt.month, pvt.day, pvt.hour, pvt.min);
				fr = f_open(Fil, file_name, FA_CREATE_ALWAYS | FA_WRITE);
 				free(file_name);
			}
			double speed = pvt.groundSpeed / 447.04;
			int print_speed = speed * 100;
			int radio_speed = speed;
			f_printf(Fil, "%d-%02d-%02dT%02d:%02d:%02d Latitude: %lf Longitude: %lf, SPEED: %.2lf\r\n", pvt.year, pvt.month, pvt.day, pvt.hour, pvt.min, pvt.sec, pvt.latitude /10000000.0, pvt.longitude/10000000.0, speed);
			d.current_state = NUM;
			for (int i = 3; i >= 0; i--) {
				d.display[i] = fmod(print_speed, 10);
				print_speed /= 10;
			}
			xQueueSend(Queue_Display, &d, portMAX_DELAY);
			xQueueSend(Queue_Speed, &radio_speed, portMAX_DELAY);

			break;
		case POLL_ERROR:
			d.current_state = NUM;
			for (int i = 3; i >= 0; i--) {
				d.display[i] = 8;
			}
			xQueueSend(Queue_Display, &d, portMAX_DELAY);
			break;
		case DATA_ERROR:
			d.current_state = NUM;
			for (int i = 3; i >= 0; i--) {
				d.display[i] = 9;
			}
			xQueueSend(Queue_Display, &d, portMAX_DELAY);
			break;
		}

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
