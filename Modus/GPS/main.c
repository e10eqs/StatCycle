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
#include "cy_result.h"

#include "main.h"


#define POLL_PVT            1
#define READ_MESSAGE 	    0
#define STOP_NMEA_DATA      0
#define SET_AND_SAVE_CONFIG 0


int main(void)
{

    console_init();

    printf("\x1b[2J\x1b[;H");

    printf("******************\n\r");
    printf("* ECE453 Dev Platform\n\r");


    printf("* -- Initializing I2C Bus\n\r");
    i2c_init();
    printf("* -- Success\n\r");


    printf("* -- Enabling Interrupts\n\r");
	/* Enable global interrupts */
	__enable_irq();

    printf("****************** \r\n\n");

#if SET_AND_SAVE_CONFIG
    setCommunicationToUbx();
    saveConfig();
    while(1) {
    	readMessage();
    	cyhal_system_delay_ms(5000);
    }

#endif

#if READ_MESSAGE
    while(1) {
    	printf("\r\n\r\n");
    	readMessage();
    	cyhal_system_delay_ms(1000);
    }

#endif

#if STOP_NMEA_DATA
    setCommunicationToUbx();
	cyhal_system_delay_ms(1000);
	checkForAck();

	while(1) {
    	printf("\r\n\r\n");
    	readMessage();
    	cyhal_system_delay_ms(1000);
	}
#endif

#if POLL_PVT

    while(1)
    {
    	/*
    	readMessage();
    	cyhal_system_delay_ms(1000);
    	*/

    	/*
    	uint16_t bytesAvailable;
    	bytesAvailable = getNumBytesAvailable();
    	printf("Bytes available: %x\n\r", bytesAvailable);
    	cyhal_system_delay_ms(1000);
		*/

    	printf("\r\nContinue\r\n");
    	getPVT();

    	cyhal_system_delay_ms(1000);
    	uint16_t bytesAvailable;
    	bytesAvailable = getNumBytesAvailable();
    	printf("\r\nBytes available: %x\n\r", bytesAvailable);

    	/*
    	cyhal_system_delay_ms(1000);
    	bytesAvailable = getNumBytesAvailable();
    	printf("Bytes available: %x\n\r", bytesAvailable);

    	cyhal_system_delay_ms(1000);
    	bytesAvailable = getNumBytesAvailable();
    	printf("Bytes available: %x\n\r", bytesAvailable);

    	cyhal_system_delay_ms(1000);
    	bytesAvailable = getNumBytesAvailable();
    	printf("Bytes available: %x\n\r", bytesAvailable);

    	cyhal_system_delay_ms(1000);
    	bytesAvailable = getNumBytesAvailable();
    	printf("Bytes available: %x\n\r", bytesAvailable);
    	*/

    }
#endif

}



/* [] END OF FILE */
