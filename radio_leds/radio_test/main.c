/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Empty PSoC6 Application
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

bool buffer[8];
bool start;
bool done;

 /* Timer object used */
 cyhal_timer_t timer_obj;
 static void isr_radio_transmitter(void *callback_arg, cyhal_timer_event_t event)
 {
     (void) event;

     enum state_t {WAITING, STARTBIT_1, STARTBIT_2, BUFFEROUT, STOPBIT_1, STOPBIT_2};
     static int state = WAITING;
     static int i = 0;

     switch(state){
     case WAITING:
            i = 0;
            done = 0;
            cyhal_gpio_write(P10_4, 0);
            state = start ? STARTBIT_1 : WAITING;
            break;
     case STARTBIT_1:
            cyhal_gpio_write(P10_4, 1);
            state = STARTBIT_2;
            break;
     case STARTBIT_2:
            cyhal_gpio_write(P10_4, 1);
            state = BUFFEROUT;
            break;
     case BUFFEROUT:
            cyhal_gpio_write(P10_4, buffer[i++]);
            state = (i == 8) ? STOPBIT_1 : BUFFEROUT;
            break;
     case STOPBIT_1:
            cyhal_gpio_write(P10_4, 1);
            state = STOPBIT_2;
            break;
     case STOPBIT_2:
            cyhal_gpio_write(P10_4, 1);
            start = 0;
            state = WAITING;
            break;
     }
 }

 void cyhal_timer_event_interrupt()
 {
     cy_rslt_t rslt;
     const cyhal_timer_cfg_t timer_cfg =
     {
         .compare_value = 0,                 /* Timer compare value, not used */
         .period = 100,                      /* Defines the timer period */
         .direction = CYHAL_TIMER_DIR_UP,    /* Timer counts up */
         .is_compare = false,                /* Don't use compare mode */
         .is_continuous = true,              /* Run the timer indefinitely */
         .value = 0                          /* Initial value of counter */
     };
     /* Initialize the timer object. Does not use pin output ('pin' is NC) and
      * does not use a pre-configured clock source ('clk' is NULL). */
     rslt = cyhal_timer_init(&timer_obj, NC, NULL);
     CY_ASSERT(CY_RSLT_SUCCESS == rslt);
     /* Apply timer configuration such as period, count direction, run mode, etc. */
     rslt = cyhal_timer_configure(&timer_obj, &timer_cfg);
     /* Set the frequency of timer to 10000 Hz */
     rslt = cyhal_timer_set_frequency(&timer_obj, 10000);
     /* Assign the ISR to execute on timer interrupt */
     cyhal_timer_register_callback(&timer_obj, isr_radio_transmitter, NULL);
     /* Set the event on which timer interrupt occurs and enable it */
     cyhal_timer_enable_event(&timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 1, true);
     /* Start the timer with the configured settings */
     rslt = cyhal_timer_start(&timer_obj);
 }

 void set_buffer(int value)
 {
 	int value_copy = value;
 	int idx = 0;
 	for(int i = 2; i < 257; i = i*2){
 		buffer[idx++] = (bool)(value_copy % i);
 		value_copy = value_copy - (value_copy % i);
 	}
 }

int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and b1oard peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    __enable_irq();

    cyhal_gpio_init(P10_4, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1);
    cyhal_gpio_init(P5_5, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    cyhal_timer_event_interrupt();

    int i = 0;
    while(1){
    	set_buffer(i++ % 2);
    	start = 1;
    	cyhal_system_delay_ms(250);
    }
}

/* [] END OF FILE */
