#include "buttons.h"

volatile int pressed = 0;

/* Timer object used */
cyhal_timer_t start_ride_timer_obj;
static void isr_debounce_start_ride(void *callback_arg, cyhal_timer_event_t event)
{
	BaseType_t xHigherPriorityTaskWoken;

    static uint8_t press_count = 0x00;
    press_count = press_count << 1;
    bool in = cyhal_gpio_read(PIN_START_RIDE_PB);
    if(!in){
       press_count = press_count | 0x01;
    }
    //debounce then add start ride to queue
    if(press_count == 0x7F){
    	pressed = START_RIDE;
    	vTaskNotifyGiveFromISR(task_button_queue_send_handle, &xHigherPriorityTaskWoken);
    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void start_ride_interrupt()
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
    rslt = cyhal_timer_init(&start_ride_timer_obj, NC, NULL);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Apply timer configuration such as period, count direction, run mode, etc. */
    rslt = cyhal_timer_configure(&start_ride_timer_obj, &timer_cfg);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Set the frequency of timer to 10000 Hz */
    rslt = cyhal_timer_set_frequency(&start_ride_timer_obj, 10000);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Assign the ISR to execute on timer interrupt */
    cyhal_timer_register_callback(&start_ride_timer_obj, isr_debounce_start_ride, NULL);
    /* Set the event on which timer interrupt occurs and enable it */
    cyhal_timer_enable_event(&start_ride_timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 2, true);
    /* Start the timer with the configured settings */
    rslt = cyhal_timer_start(&start_ride_timer_obj);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

/* Timer object used */
cyhal_timer_t pairing_timer_obj;
static void isr_debounce_pairing(void *callback_arg, cyhal_timer_event_t event)
{
    BaseType_t xHigherPriorityTaskWoken;
    static uint8_t press_count = 0x00;
    press_count = press_count << 1;
    bool in = cyhal_gpio_read(PIN_BT_PAIR_PB);
    if(!in){
    	press_count = press_count | 0x01;
    }
    //debounce then add start ride to queue
    if(press_count == 0x7F){
        pressed = PAIRING;
        vTaskNotifyGiveFromISR(task_button_queue_send_handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void pairing_interrupt()
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
    rslt = cyhal_timer_init(&pairing_timer_obj, NC, NULL);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Apply timer configuration such as period, count direction, run mode, etc. */
    rslt = cyhal_timer_configure(&pairing_timer_obj, &timer_cfg);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Set the frequency of timer to 10000 Hz */
    rslt = cyhal_timer_set_frequency(&pairing_timer_obj, 10000);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Assign the ISR to execute on timer interrupt */
    cyhal_timer_register_callback(&pairing_timer_obj, isr_debounce_pairing, NULL);
    /* Set the event on which timer interrupt occurs and enable it */
    cyhal_timer_enable_event(&pairing_timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 2, true);
    /* Start the timer with the configured settings */
    rslt = cyhal_timer_start(&pairing_timer_obj);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

static void init_buttons(void)
{
	cy_rslt_t rslt = cyhal_gpio_init(PIN_START_RIDE_PB, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULL_NONE , 0);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	rslt = cyhal_gpio_init(PIN_BT_PAIR_PB, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULL_NONE , 0);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

void buttons_init(void)
{
	init_buttons();
	start_ride_interrupt();
	pairing_interrupt();
}

void task_button_queue_send(void)
{
	while(1){
		uint32_t ulEventsToProcess;
		ulEventsToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		xQueueSend(Queue_Buttons, &pressed, portMAX_DELAY);
	}
}
