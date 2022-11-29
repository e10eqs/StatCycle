#include "display.h"


uint8_t draw_count = 0;
uint8_t display[] = { 1, 2, 3, 4 };

int main (void)
{
//	__enable_irq();
//
//	cyhal_gpio_init(
//			PIN_USER_LED,                // Pin
//			CYHAL_GPIO_DIR_OUTPUT,      // Direction
//			CYHAL_GPIO_DRIVE_STRONG,    // Drive Mode
//			false);				        // InitialValue
//
//	cyhal_gpio_write(PIN_USER_LED, true);

	display_init();

	while (true){
		for(int i = 0; i < 4; i++){
			display_digit(i, display[i % 4]);
			cyhal_system_delay_ms(2);
		}
	}
}


