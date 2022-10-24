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

#include "main.h"
#define ENABLE_I2C 1

/******************************************************************************
 * These MACROS where defined using information for the AT42QT2120 datasheet.
 * and the ECE453 Development Platform schematics.
 *****************************************************************************/
#define BLUE_LED_REG_ADDR				(28+4)
#define BLUE_LED_REG_VAL_ON				(0x3)
#define BLUE_LED_REG_VAL_OFF			(0x1)

#define GREEN_LED_REG_ADDR				(28+5)
#define GREEN_LED_REG_VAL_ON			(0x3)
#define GREEN_LED_REG_VAL_OFF			(0x1)

#define YELLOW_LED_REG_ADDR				(28+6)
#define YELLOW_LED_REG_VAL_ON			(0x3)
#define YELLOW_LED_REG_VAL_OFF			(0x1)

#define RED_LED_REG_ADDR				(28+7)
#define RED_LED_REG_VAL_ON				(0x3)
#define RED_LED_REG_VAL_OFF				(0x1)



int main(void)
{
	uint8_t button_status;

    console_init();

    printf("\x1b[2J\x1b[;H");

    printf("******************\n\r");
    printf("* ECE453 Dev Platform\n\r");

    printf("* -- Initializing user push button\n\r");
	push_button_init();

    printf("* -- Initializing user LED\n\r");
    leds_init();

#if ENABLE_I2C
    printf("* -- Initializing I2C Bus\n\r");
    i2c_init();

    printf("* -- Initializing AT42QT2120\n\r");
    AT42QT2120_init();

    button_status = AT42QT2120_read_buttons();
#endif


    printf("* -- Enabling Interrupts\n\r");
	/* Enable global interrupts */
	__enable_irq();

    printf("****************** \r\n\n");

    while(1)
    {
#if ENABLE_I2C
    	if(ALERT_AT42QT2120_CHANGE)
		{
			ALERT_AT42QT2120_CHANGE = false;

			button_status = AT42QT2120_read_buttons();
			if(button_status & 0x01)
			{
				printf("BLUE ON\n\r");
				AT42QT2120_write_reg(BLUE_LED_REG_ADDR, BLUE_LED_REG_VAL_ON);
			}
			else
			{
				AT42QT2120_write_reg(BLUE_LED_REG_ADDR, BLUE_LED_REG_VAL_OFF);
			}

			if(button_status & 0x04)
			{
				printf("YELLOW ON\n\r");
				AT42QT2120_write_reg(YELLOW_LED_REG_ADDR, YELLOW_LED_REG_VAL_ON);
			}
			else
			{
				AT42QT2120_write_reg(YELLOW_LED_REG_ADDR, YELLOW_LED_REG_VAL_OFF);
			}

			if(button_status & 0x02)
			{
				printf("GREEN ON\n\r");
				AT42QT2120_write_reg(GREEN_LED_REG_ADDR, GREEN_LED_REG_VAL_ON);
			}
			else
			{
				AT42QT2120_write_reg(GREEN_LED_REG_ADDR, GREEN_LED_REG_VAL_OFF);
			}

			if(button_status & 0x08)
			{
				printf("RED ON\n\r");
				AT42QT2120_write_reg(RED_LED_REG_ADDR, RED_LED_REG_VAL_ON);
			}
			else
			{
				AT42QT2120_write_reg(RED_LED_REG_ADDR, RED_LED_REG_VAL_OFF);
			}
		}
#endif
    	if(ALERT_PUSH_BUTTON)
    	{
    		ALERT_PUSH_BUTTON = false;
    		printf("ALERT_PUSH_BUTTON Pressed!\n\r");
    	}

    	if(ALERT_CONSOLE_RX)
		{
			ALERT_CONSOLE_RX = false;
			printf("\r\nRx --> %s\r\n", pcInputString);
			cInputIndex = 0;
			memset(pcInputString,0,DEBUG_MESSAGE_MAX_LEN);
		}
    }
}



/* [] END OF FILE */
