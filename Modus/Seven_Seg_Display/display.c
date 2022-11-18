#include "display.h"
#include "../main_board_pins.h"

uint8_t SEVEN_SEG_LUT[] = { 0x3F,   // 0
		0x06,  // 1
		0x5B,  // 2
		0x4F,  // 3
		0x66,  // 4
		0x6D,  // 5
		0x7D,  // 6
		0x07,  // 7
		0x7F,  // 8
		0x6F,  // 9
		};

const size_t SEG_PIN_SIZE = 7;

cyhal_gpio_t SEG_PIN[] = {
PIN_SEG_EN_A,
PIN_SEG_EN_B,
PIN_SEG_EN_C,
PIN_SEG_EN_D,
PIN_SEG_EN_E,
PIN_SEG_EN_F,
PIN_SEG_EN_G,
 };
const size_t DIG_PIN_SIZE = 4;

cyhal_gpio_t DIG_PIN[] = {
PIN_DIG0_N,
PIN_DIG1_N,
PIN_DIG2_N,
PIN_DIG3_N };

void display_init(void) {
	for (int i = 0; i < DIG_PIN_SIZE; i++) {
		cyhal_gpio_init(DIG_PIN[i],                // Pin
				CYHAL_GPIO_DIR_OUTPUT,      // Direction
				CYHAL_GPIO_DRIVE_STRONG,    // Drive Mode
				true);				        // InitialValue
	}

	for (int i = 0; i < SEG_PIN_SIZE; i++) {
		cyhal_gpio_init(SEG_PIN[i],                // Pin
				CYHAL_GPIO_DIR_OUTPUT,      // Direction
				CYHAL_GPIO_DRIVE_STRONG,    // Drive Mode
				false);				        // InitialValue
	}
	cyhal_gpio_init(PIN_SEG_EN_DP,      // Pin
			CYHAL_GPIO_DIR_OUTPUT,      // Direction
			CYHAL_GPIO_DRIVE_STRONG,    // Drive Mode
			true);			        	// InitialValue
}

void display_all_dig_off(void) {
	for (int i = 0; i < DIG_PIN_SIZE; i++) {
		cyhal_gpio_write(DIG_PIN[i], true);
	}
}

void display_digit(uint8_t location, uint8_t number) {
	display_all_dig_off();

	cyhal_gpio_write(DIG_PIN[location], false);

	for (int i = 0; i < SEG_PIN_SIZE; i++) {
		if ((1 << i) & SEVEN_SEG_LUT[number]) {
			cyhal_gpio_write(SEG_PIN[i], true);
		} else {
			cyhal_gpio_write(SEG_PIN[i], false);
		}
	}

}

