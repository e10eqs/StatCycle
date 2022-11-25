/*******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PWM Square Wave code example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*******************************************************************************
* (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.
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


#include "cybsp.h"
#include "led.h"
#include "cy_pdl.h"

cyhal_pwm_t usr_led;
cy_rslt_t rslt;

void (*mode[NUM_MODES])(int) = {pwm_duty_cycle_blue, sweep, pulse_all, pulse_color, pulse_party};
bool buffer[8];
int led_mode = 0;
cyhal_pwm_t blue_led;
/* Timer object used */
cyhal_timer_t timer_obj;
static void isr_radio_receiver(void *callback_arg, cyhal_timer_event_t event)
{
    (void) event;
    enum state_t {WAITING, STARTBIT, BUFFERIN, STOPBIT_1, STOPBIT_2, INVALID, VALID};
    static int speed = 0;
    static int state = WAITING;
    static int i = 0;

//    (*mode[led_mode]) (speed);
    bool in = cyhal_gpio_read(P10_4);
    cyhal_gpio_write(P12_6, speed);
    switch(state){
        case WAITING:
            i = 0;
            state = in ? STARTBIT : WAITING;
            break;
        case STARTBIT:
        	state = in ? BUFFERIN : WAITING;
        	break;
        case BUFFERIN:
            buffer[i++] = in;
            state = (i == 8) ? STOPBIT_1 : BUFFERIN;
            break;
        case STOPBIT_1:
            state = in ? STOPBIT_2 : INVALID;
            break;
        case STOPBIT_2:
             state = in ? VALID : INVALID;
             break;
        case INVALID:
            state = WAITING;
            break;
        case VALID:
        	speed = 0;
        	for(int i = 0; i < 8; i++){
        		speed |= buffer[i] << i;
    		}
        	cyhal_gpio_write(P5_5, speed);
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
    cyhal_timer_register_callback(&timer_obj, isr_radio_receiver, NULL);
    /* Set the event on which timer interrupt occurs and enable it */
    cyhal_timer_enable_event(&timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 0, true);
    /* Start the timer with the configured settings */
    rslt = cyhal_timer_start(&timer_obj);
}


int main(void)
{
    /* API return code */
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (CY_RSLT_SUCCESS != result)
    {
        /* Halt the CPU while debugging */
        CY_ASSERT(false);
    }
    /* Enable global interrupts */
    __enable_irq();

   // pwm_init_all();
    cyhal_gpio_init(P12_6, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);

    cyhal_gpio_init(P10_4, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1);

    cyhal_timer_event_interrupt();

    cyhal_gpio_init(P5_5, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1);
}


/* [] END OF FILE */
