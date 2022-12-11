#include "task_redraw_display.h"

void Task_Redraw_Display(void *pvParameters) {

	uint8_t draw_count = 0;
	uint8_t draw_loading = 0;
	Display d = { NUM, { 1, 2, 3, 4 } };

	while (1) {

		xQueueReceive(Queue_Display, &d, 0);

		switch (d.current_state) {
		case LOADING:
			display_loading(draw_loading++ % 6);
			vTaskDelay(pdMS_TO_TICKS(200));
			break;
		case FLASH:
			for (int i = 0; i < 5; i++) {
				display_digit_all(0);
				vTaskDelay(pdMS_TO_TICKS(250));
				display_all_dig_off();
				vTaskDelay(pdMS_TO_TICKS(250));
			}
			//d.current_state = OFF;
			break;
		case NUM:
			;
			uint8_t loc = draw_count % 4;
			uint8_t num = d.display[draw_count % 4];
			display_digit(loc, num);
			draw_count++;
			vTaskDelay(pdMS_TO_TICKS(3));
			break;
		case OFF:
			display_all_dig_off();
			break;
		}

	}
}
