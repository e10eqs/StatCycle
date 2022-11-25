//
// /* Timer object used */
// cyhal_timer_t timer_obj;
// static void isr_radio_receiver(void *callback_arg, cyhal_timer_event_t event)
// {
//     (void) callback_arg;
//     (void) event;
//
//     enum state_t = {WAITING, STARTBIT_1, STARTBIT_2, BUFFEROUT, STOPIT_1, STOPBIT_2};
//     static state_t state = 0;
//     static int i = 0;
//
//     switch(state){
//     case WAITING:
//            i = 0;
//            state = start ? STARTBIT_1 : WAITING;
//            break;
//     case STARTBIT_1:
//            cyhal_gpio_write(P10_4, 1);
//            state = STARTBIT_2;
//            break;
//     case STARTBIT_2:
//            cyhal_gpio_write(P10_4, 1);
//            state = BUFFERIN;
//            break
//     case BUFFEROUT:
//            cyhal_gpio_write(P10_4, buffer[i++]);
//            state = (i == 7) ? STOPBIT_1 : BUFFERIN;
//            break;
//     case STOPBIT_1:
//            out = 1;
//            cyhal_gpio_write(P10_4, 1);
//            state = STOPBIT_2;
//            break;
//     case STOPBIT_2:
//            cyhal_gpio_write(P10_4, 1);
//            state = WAITING;
//            break;
//     }
//
// }
//
// void cyhal_timer_event_interrupt()
// {
//     cy_rslt_t rslt;
//     const cyhal_timer_cfg_t timer_cfg =
//     {
//         .compare_value = 0,                 /* Timer compare value, not used */
//         .period = 100,                      /* Defines the timer period */
//         .direction = CYHAL_TIMER_DIR_UP,    /* Timer counts up */
//         .is_compare = false,                /* Don't use compare mode */
//         .is_continuous = true,              /* Run the timer indefinitely */
//         .value = 0                          /* Initial value of counter */
//     };
//     /* Initialize the timer object. Does not use pin output ('pin' is NC) and
//      * does not use a pre-configured clock source ('clk' is NULL). */
//     rslt = cyhal_timer_init(&timer_obj, NC, NULL);
//     CY_ASSERT(CY_RSLT_SUCCESS == rslt);
//     /* Apply timer configuration such as period, count direction, run mode, etc. */
//     rslt = cyhal_timer_configure(&timer_obj, &timer_cfg);
//     /* Set the frequency of timer to 40 Hz */
//     rslt = cyhal_timer_set_frequency(&timer_obj, 40);
//     /* Assign the ISR to execute on timer interrupt */
//     cyhal_timer_register_callback(&timer_obj, isr_timer, NULL);
//     /* Set the event on which timer interrupt occurs and enable it */
//     cyhal_timer_enable_event(&timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 3, true);
//     /* Start the timer with the configured settings */
//     rslt = cyhal_timer_start(&timer_obj);
// }
