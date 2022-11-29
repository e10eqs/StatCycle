#include "task_update_display.h"

void Task_Update_Display(void *pvParameters) {

	Display d = {false, {0,0,0,0}};
	uint8_t num = 0;

	while(1) {

	xQueueSend(Queue_Display, &d, portMAX_DELAY);
	d.display[num % 4] = (d.display[num % 4] + 1) % 10;
	num++;

	vTaskDelay(pdMS_TO_TICKS(500));
	}
}
