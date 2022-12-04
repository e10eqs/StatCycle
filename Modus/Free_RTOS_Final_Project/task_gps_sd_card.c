#include "task_gps_sd_card.h"


void task_gps_sd_card() {

	while(1) {
		getPVT();
		cyhal_system_delay_ms(1000);
		uint16_t bytesAvailable;
		bytesAvailable = getNumBytesAvailable();
	}
}
