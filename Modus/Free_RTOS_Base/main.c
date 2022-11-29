#include "main.h"
#define LED_GPIO PIN_USER_LED

void blinky(void *arg) {
	(void) arg;
	/* Initialize the LED GPIO pin */
	cyhal_gpio_init(LED_GPIO, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG,
			0);
	for (;;) {
		/* Toggle the LED periodically */
		cyhal_gpio_toggle(LED_GPIO);
		vTaskDelay(500);
	}
}

int main(void)
   {
       BaseType_t retval;
       cy_rslt_t result;
       /* Initialize the device and board peripherals */
       result = cybsp_init() ;
       if (result != CY_RSLT_SUCCESS)
       {
           CY_ASSERT(0);
       }
       __enable_irq();

       retval = xTaskCreate(blinky, "blinky", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
       if (pdPASS == retval)
       {
           vTaskStartScheduler();
       }
       for (;;)
       {
           /* vTaskStartScheduler never returns */
       }
   }

/* [] END OF FILE */
