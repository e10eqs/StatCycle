#include "main.h"

QueueHandle_t Queue_Display;
QueueHandle_t Queue_Speed;
QueueHandle_t Queue_Buttons;

TaskHandle_t task_Redraw_Display_handle;
TaskHandle_t task_Dummy_Velocity_handle;
TaskHandle_t task_Radio_Transmitter_handle;
TaskHandle_t task_button_queue_send_handle;
TaskHandle_t task_gps_sd_card_handle;
TaskHandle_t task_ble_findme_process_handle;
TaskHandle_t task_state_machine_handle;

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

       //init all the hardware
       display_init();
       buttons_init();
       i2c_init();
       ble_findme_init();

       Queue_Display = xQueueCreate(1, sizeof(Display));
       Queue_Speed = xQueueCreate(1, sizeof(uint8_t));
       Queue_Buttons = xQueueCreate(1, sizeof(int));

       xTaskCreate(Task_Redraw_Display, "Task_Redraw_Display", configMINIMAL_STACK_SIZE, NULL, 4, &task_Redraw_Display_handle);
       xTaskCreate(Task_Dummy_Velocity, "Task_Dummy_Velocity", configMINIMAL_STACK_SIZE, NULL, 3, &task_Dummy_Velocity_handle);
       //soon but not yet - Owen needs to add the config to the gps module on the main board
       //xTaskCreate(task_gps_sd_card, "task_gps_sd_card", configMINIMAL_STACK_SIZE, NULL, 3, &task_gps_sd_card_handle);

       xTaskCreate(Task_Radio_Transmitter, "Task_Radio_Transmitter", configMINIMAL_STACK_SIZE, NULL, 5, &task_Radio_Transmitter_handle);
       xTaskCreate(task_state_machine, "task_state_machine", 1024, NULL, 4, &task_state_machine_handle);
       xTaskCreate(task_button_queue_send, "button_send_queue", configMINIMAL_STACK_SIZE, NULL, 3, &task_button_queue_send_handle);
       xTaskCreate(task_ble_findme_process, "task_ble_findme_process", 1024, NULL, 4, &task_ble_findme_process_handle);

       vTaskStartScheduler();

       for (;;)
       {
           /* vTaskStartScheduler never returns */
       }
   }

/* [] END OF FILE */
