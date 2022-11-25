//
// /* Timer object used */
// cyhal_timer_t timer_obj;
// static void isr_radio_receiver(void *buffer, void *valid, cyhal_timer_event_t event)
// {
//     (void) buffer;
//     (void) valid;
//     (void) event;
//
//     enum state_t = {WAITING, STARTBIT, BUFFERIN, STOPIT_1, STOPBIT_2, INVALID, VALID};
//     static state_t state = 0;
//     static int i = 0;
//
//     //need to define valid, and in
//     bool in = cyhal_gpio_read(P10_4);
//     switch(state){
//        WAITING:
//            i = 0;
//            state = in ? STARTBIT : WAITING;
//            break;
//        STARTBIT:
//            state = in ? BUFFERIN : WAITING;
//            break;
//        BUFFERIN:
//            buffer[i++] = in;
//            state = (i == 7) ? STOPBIT_1 : BUFFERIN;
//            break;
//        STOPBIT_1:
//            state = in ? STOPBIT_2 : INVALID;
//            break;
//        STOPBIT_2:
//            state = in ? VALID : INVALID;
//            break;
//        INVALID:
//            valid = 0;
//            state = WAITING;
//            break;
//        VALID:
//            valid = 1;
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
