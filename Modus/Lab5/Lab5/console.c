/*
 * console.c
 *
 *  Created on: Jan 21, 2022
 *      Author: Joe Krachey
 */

#include "console.h"

#define ENABLE_UART_RX_IRQ	1

extern cyhal_uart_t cy_retarget_io_uart_obj;
volatile bool ALERT_CONSOLE_RX = false;

char pcInputString[ DEBUG_MESSAGE_MAX_LEN ];
int8_t cInputIndex = 0;

/*******************************************************************************
* Function Name: ece453_console_event_handler
********************************************************************************
* Summary:
* UART Handler used to receive characters from the console.
*
* Parameters:
*  void
*
* Return:
*
*
*******************************************************************************/
void console_event_handler(void *handler_arg, cyhal_uart_event_t event)
{
    (void) handler_arg;
    uint32_t value;
    cy_rslt_t status;
    char c;

        if ((event & CYHAL_UART_IRQ_TX_ERROR) == CYHAL_UART_IRQ_TX_ERROR)
    {
        /* An error occurred in Tx */
        /* Insert application code to handle Tx error */
    }
    else if ((event & CYHAL_UART_IRQ_RX_NOT_EMPTY) == CYHAL_UART_IRQ_RX_NOT_EMPTY)
    {

		// Received a character.
    	// ADD CODE to receive a character using the HAL UART API
    	cyhal_uart_getc(&cy_retarget_io_uart_obj, &value, 0);


    	c = (char)value;

    	if(status == CY_RSLT_SUCCESS)
    	{
    		// Echo out the character
        	// ADD CODE to transmit a character using the HAL UART API
    		cyhal_uart_putc(&cy_retarget_io_uart_obj, value);

    		// If the ISR detects that the user has pressed the ENTER key,
			// Send a task notification to Task_Console
			if (c == '\n' || c == '\r')
			{
				ALERT_CONSOLE_RX = true;

			}
			else
			{
				if (c == '\b')
				{
					// backspace character, so delete the previous character in the array
					cInputIndex--;
					pcInputString[cInputIndex] =0;
				}
				else
				{
					pcInputString[cInputIndex] = c;
					cInputIndex++;
				}
			}

    	}

    }
}

/*******************************************************************************
* Function Name: ece453_irq_init
********************************************************************************
* Summary:
* Enables and Registers all interrupt sources for the ECE453 Base project
*
* Parameters:
*  void
*
* Return:
*
*
*******************************************************************************/
static void console_init_irq(void)
{
    // Enable Console Rx Interrupts
	/* The UART callback handler registration */
	cyhal_uart_register_callback(
			&cy_retarget_io_uart_obj,		// retarget-io uart object
			console_event_handler, 			// Handler Name
			NULL);							// Handler arguments

	/* Enable required UART events */
	cyhal_uart_enable_event(
			&cy_retarget_io_uart_obj, 							// retarget-io uart object
			(cyhal_uart_event_t)(CYHAL_UART_IRQ_RX_NOT_EMPTY), 	// Events to enable/disable
			INT_PRIORITY_CONSOLE, 								// Priority
			true);												// Enable events

}

/*******************************************************************************
* Set up the console retarget IO
*******************************************************************************/
static void console_init_retarget(void)
{

    /* Initialize retarget-io to use the debug UART port, 8N1 */
	cy_retarget_io_init(
			PIN_CONSOLE_TX,
			PIN_CONSOLE_RX,
			115200);	// Baud Rate

}

/*******************************************************************************
* Function Name: console_init
********************************************************************************
* Summary:
* 	Enables the SCB used for console interface.  Rx interrupts are turned on
*
* Parameters:
*  void
*
* Return:
*
*
*******************************************************************************/
void console_init(void)
{
	console_init_retarget();

#if ENABLE_UART_RX_IRQ
	console_init_irq();
#endif

}






