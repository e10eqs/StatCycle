#include "main.h"

QueueHandle_t Queue_Display;
QueueHandle_t Queue_Speed;

int main(void)
   {
       cy_rslt_t result;
       /* Initialize the device and board peripherals */
       result = cybsp_init() ;
       if (result != CY_RSLT_SUCCESS)
       {
           CY_ASSERT(0);
       }
       __enable_irq();
       display_init();

       Queue_Display = xQueueCreate(1, sizeof(Display));
       Queue_Speed = xQueueCreate(1, sizeof(uint8_t));

       xTaskCreate(Task_Redraw_Display, "Task_Redraw_Display", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

       //xTaskCreate(Task_Update_Display, "Task_Update_Display", configMINIMAL_STACK_SIZE, NULL, 4, NULL);
       //xTaskCreate(Task_Radio_Transmitter, "Task_Radio_Transmitter", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

       vTaskStartScheduler();

       for (;;)
       {
           /* vTaskStartScheduler never returns */
       }
   }

/* [] END OF FILE */
