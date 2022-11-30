#ifndef TASK_RADIO_TRANSMITTER_H_
#define TASK_RADIO_TRANSMITTER_H_
#include "main.h"

#define PULSE_WIDTH 1

#define TRANSMITTER_OUT P10_4

#define TRANSMISSION_TIMEOUT 500

void Task_Radio_Transmitter(void *pvParameters);

#endif
