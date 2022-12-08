#include "task_state_machine.h"

void task_state_machine(void *pvParameters)
{
	enum state_t {WAITING, RIDE_TIME, BLE_PAIRING, BLE_TRANSACTION};
	static int state = WAITING;

	vTaskSuspend(task_Radio_Transmitter_handle);
	vTaskSuspend(task_Dummy_Velocity_handle);
	vTaskSuspend(task_ble_findme_process_handle);

	while(1) {
		int button;
		switch(state){
			case WAITING:
				; //looks silly but is necessary. C doesn't allow declarations after labels
				Display loading_wait = {LOADING, {0,0,0,0}};
				xQueueSend(Queue_Display, &loading_wait, portMAX_DELAY);
				xQueueReceive(Queue_Buttons, &button, portMAX_DELAY);
				if(button == START_RIDE) {
					button = NONE;
					state = RIDE_TIME;
				}
				else if(button == PAIRING) {
					button = NONE;
					state = BLE_PAIRING;
				}
				break;
			case RIDE_TIME:
				//start gps, radio, and 7 seg task
			    fr = f_open(&Fil, "fake_gps_data.txt", FA_CREATE_ALWAYS | FA_WRITE);
				vTaskResume(task_Dummy_Velocity_handle);
				vTaskResume(task_Radio_Transmitter_handle);
				xQueueReceive(Queue_Buttons, &button, portMAX_DELAY);
				if(button == START_RIDE) {
					//end tasks and go back to waiting
					f_close(&Fil);
					vTaskSuspend(task_Radio_Transmitter_handle);
					vTaskSuspend(task_Dummy_Velocity_handle);
					button = NONE;
					state = WAITING;
				}
				break;
			case BLE_PAIRING:
				;
				Display flash_pairing = {FLASH, {0,0,0,0}};
				xQueueSend(Queue_Display, &flash_pairing, portMAX_DELAY);
				//start pairing task
				vTaskResume(task_ble_findme_process_handle);
				ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
				Display paired = {NUM, {8,8,8,8}};
				xQueueSend(Queue_Display, &flash_pairing, portMAX_DELAY);
				state = BLE_TRANSACTION;
				break;
			case BLE_TRANSACTION:
				xQueueReceive(Queue_Buttons, &button, portMAX_DELAY);
				if(button == PAIRING){
					vTaskSuspend(task_ble_findme_process_handle);
					Display loading_wait = {LOADING, {0,0,0,0}};
					xQueueSend(Queue_Display, &loading_wait, portMAX_DELAY);
					button = NONE;
					state = WAITING;
				}
		}
	}
}
