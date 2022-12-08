#include "push_button.h"

/* Timer object used */
cyhal_timer_t button_timer_obj;
static void isr_debounce_pushbutton(void *callback_arg, cyhal_timer_event_t event)
{
    static uint8_t press_count = 0;
    bool in = cyhal_gpio_read(P5_0);
    if(!in){
        press_count++;
    } else {
        press_count = 0;
    }
    //debounce then increment led_mode
    if(press_count == 10){
        led_mode++;
        press_count = 0;
        cyhal_gpio_toggle(P5_5);
    }
}

void pubshbutton_interrupt()
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
    rslt = cyhal_timer_init(&button_timer_obj, NC, NULL);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Apply timer configuration such as period, count direction, run mode, etc. */
    rslt = cyhal_timer_configure(&button_timer_obj, &timer_cfg);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Set the frequency of timer to 10000 Hz */
    rslt = cyhal_timer_set_frequency(&button_timer_obj, 10000);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Assign the ISR to execute on timer interrupt */
    cyhal_timer_register_callback(&button_timer_obj, isr_debounce_pushbutton, NULL);
    /* Set the event on which timer interrupt occurs and enable it */
    cyhal_timer_enable_event(&button_timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 0, true);
    /* Start the timer with the configured settings */
    rslt = cyhal_timer_start(&button_timer_obj);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

static void push_button_io_init(void)
{
	cy_rslt_t rslt = cyhal_gpio_init(P5_0, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULL_NONE , 0);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    rslt = cyhal_gpio_init(P5_5, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

void push_button_init(void)
{
    push_button_io_init();
    pubshbutton_interrupt();
}
