#include "task_dummy_velocity.h"

void Task_Dummy_Velocity(void *pvParameters) {

	uint8_t mock_data = 0;
	Display d = {NUM, {0,0,0,0}};
	volatile FRESULT fr;

	while(1) {

	uint8_t send_mock = mock_data;
	for(int i = 3; i >= 0; i--){
		d.display[i] = send_mock % 10;
		send_mock /= 10;
	}
	char* buffer = malloc(100);
	UINT bw;
	snprintf(buffer, 100, "This is dummy data iteration: %d   ", mock_data);
	fr = f_write(&Fil, buffer, strlen(buffer), &bw);
	free(buffer);
	xQueueSend(Queue_Display, &d, portMAX_DELAY);
	xQueueSend(Queue_Speed, &mock_data, portMAX_DELAY);

	mock_data++;

	vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
