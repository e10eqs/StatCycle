/*----------------------------------------------------------------------*/
/* Foolproof FatFs sample project for AVR              (C)ChaN, 2014    */
/*----------------------------------------------------------------------*/

#include "ff.h"		/* Declarations of FatFs API */
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "stdlib.h"
#define PIN_USER_LED P5_5	/* ADD CODE */

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */


int main (void)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

	__enable_irq();

	cyhal_gpio_init(
			PIN_USER_LED,                // Pin
			CYHAL_GPIO_DIR_OUTPUT,      // Direction
			CYHAL_GPIO_DRIVE_STRONG,    // Drive Mode
			false);				        // InitialValue

	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
	char* path = "/";

    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */                                     /* It is a file. */
             printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);

//	fr = f_open(&Fil, "mock_test.txt", FA_CREATE_ALWAYS | FA_WRITE);	/* Create a file */
//	//fr = f_unlink("brandonthisareallylongfilename.txt"); /* delete a file */
//	if (fr == FR_OK) {
//		char thingToWrite[] = "It works! Testing!!! This is for ECE453.\r\n";
//		f_write(&Fil, thingToWrite, strlen(thingToWrite), &bw);	/* Write data to the file */
//		fr = f_close(&Fil);							/* Close the file */
//		//free(thingToRead);
//		if (fr == FR_OK && bw == strlen(thingToWrite)) {		/* Lights green LED if data written well */
//			cyhal_gpio_write(PIN_USER_LED, true);
//		}
//	}
	for (;;) ;
}


