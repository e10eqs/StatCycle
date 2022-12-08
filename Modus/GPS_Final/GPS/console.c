/*
 * console.c
 *
 *  Created on: Jan 21, 2022
 *      Author: Joe Krachey
 */

#include "console.h"


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
}






