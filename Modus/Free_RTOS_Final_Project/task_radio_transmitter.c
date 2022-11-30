#include "task_radio_transmitter.h"



//helper function that writes a data value into a buffer
void set_buffer(int value, bool* buffer)
{
	int value_copy = value;
	int idx = 0;
	for(int i = 2; i < 257; i = i*2){
		buffer[idx++] = (bool)(value_copy % i);
		value_copy = value_copy - (value_copy % i);
	}
}

//transmitter time
void Task_Radio_Transmitter(void *pvParameters) {

	//init gpio
	cyhal_gpio_init(TRANSMITTER_OUT, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1);

	//set up vars
	bool buffer[8];
	uint8_t data;

	while(1) {

		xQueueReceive(Queue_Speed, &data, portMAX_DELAY);
		set_buffer(data, buffer);

		//STARTBIT_1:
		cyhal_gpio_write(TRANSMITTER_OUT, 1);
		vTaskDelay(pdMS_TO_TICKS(PULSE_WIDTH));

		//sTARTBIT_2:
		cyhal_gpio_write(TRANSMITTER_OUT, 1);
		vTaskDelay(pdMS_TO_TICKS(PULSE_WIDTH));

		//BUFFEROUT:
		for(int i = 0; i < 8; i++){
			cyhal_gpio_write(TRANSMITTER_OUT, buffer[i++]);
			vTaskDelay(pdMS_TO_TICKS(PULSE_WIDTH));
		}

		//STOPBIT_1:
		cyhal_gpio_write(TRANSMITTER_OUT, 1);
		vTaskDelay(pdMS_TO_TICKS(PULSE_WIDTH));

		//STOPBIT_2:
		cyhal_gpio_write(TRANSMITTER_OUT, 1);
		vTaskDelay(pdMS_TO_TICKS(PULSE_WIDTH));

		//wait for next transmission
		cyhal_gpio_write(TRANSMITTER_OUT, 0);
		vTaskDelay(pdMS_TO_TICKS(TRANSMISSION_TIMEOUT));
	}
}

/* [] END OF FILE */
