#include "task_redraw_display.h"


void Task_Redraw_Display(void *pvParameters) {

	uint8_t draw_count = 0;
	uint8_t draw_loading = 0;
	Display d = {true, {1,2,3,4}};

	while(1) {

	xQueueReceive(Queue_Display, &d, 0);

	if (d.loading) {
		display_loading(draw_loading++ % 6);
		vTaskDelay(pdMS_TO_TICKS(200));
	}
	else {
		uint8_t loc = draw_count % 4;
		uint8_t num = d.display[draw_count % 4];
		display_digit(loc, num);
		draw_count++;
		vTaskDelay(pdMS_TO_TICKS(3));
	}

	}
}
