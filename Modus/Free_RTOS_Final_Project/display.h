#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "../main_board_pins.h"

void display_init(void);
void display_all_dig_off(void);
void display_all_dig_on(void);
void display_loading(uint8_t number);
void display_digit(uint8_t location, uint8_t number);

#endif
