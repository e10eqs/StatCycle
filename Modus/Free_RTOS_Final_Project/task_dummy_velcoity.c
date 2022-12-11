#include "task_dummy_velocity.h"

void Task_Dummy_Velocity(void *pvParameters) {

	enum pvtState_e state = NO_FIX;
	Display d = { NUM, { 0, 0, 0, 0 } };
	enum states new_display_state = NUM;
	static int data = 0;

	while (1) {
		state = getPVT();
		data += 5;
		data = data % 255;
		xQueueSend(Queue_Speed, &data, portMAX_DELAY);
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
				snprintf(file_name, sz, "%d%d-%02d%02d%d.txt", pvt.hour, pvt.min, pvt.month, pvt.day, pvt.year);
				fr = f_open(Fil, file_name, FA_CREATE_ALWAYS | FA_WRITE);
 				free(file_name);
			}
			double speed = pvt.groundSpeed / 447.04;
			f_printf(Fil, "Hour: %d, Minute: %d, Second: %d Latitude: %ld Longitude: %ld, SPEED: %ld\r\n", pvt.hour, pvt.min, pvt.sec, pvt.latitude, pvt.longitude, speed);
			d.current_state = NUM;
			for (int i = 3; i >= 0; i--) {
				d.display[i] = fmod(speed, 10);
				speed /= 10;
			}
			xQueueSend(Queue_Display, &d, portMAX_DELAY);

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
