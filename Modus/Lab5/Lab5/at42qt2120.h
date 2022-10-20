/*
 * opt3001.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Joe Krachey
 */

#ifndef AT42QT2120_H_
#define AT42QT2120_H_

#include <stdint.h>
#include <stdbool.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "i2c.h"

#define PIN_CT_IRQ			P10_2
#define INT_PRIORITY_CT		3

#define AT42QT2120_SUBORDINATE_ADDR                 0x1C

#define AT42QT2120_CHIP_ID_REG                      0
#define AT42QT2120_DET_STATUS_REG                   2
#define AT42QT2120_KEY_STATUS_LO_REG                3
#define AT42QT2120_KEY_STATUS_HI_REG                4
#define AT42QT2120_KEY_SLIDER_POS_REG               5

#define AT42QT2120_WRITE_PACKET_SIZE				2

extern volatile bool ALERT_AT42QT2120_CHANGE;

/** Write a register on the AT42QT2120
 *
 * @param reg The reg address to read
 * @param value The value to be written
 *
 */
void AT42QT2120_write_reg(uint8_t reg, uint8_t value);

/** Read Chip ID Register
 *
 *
 *
 */
uint8_t AT42QT2120_read_chip_id(void);

/** Read Chip Status Low Register
 *
 *
 *
 */
uint8_t AT42QT2120_read_key_status_lo(void);

/** Read Chip Status Hi Register
 *
 *
 *
 */
uint8_t AT42QT2120_read_key_status_hi(void);

/**
 *
 *
 *
 */
uint8_t AT42QT2120_read_detection_status(void);

/**
 *
 *
 *
 */
uint8_t AT42QT2120_read_slider_status(void);

/**
 *
 *
 *
 */
uint8_t AT42QT2120_read_buttons(void);

/**
 *
 *
 *
 */
void AT42QT2120_init(void);

void cap_sense_handler(void* handler_arg, cyhal_gpio_event_t event);

#endif /* AT42QT2120_H_ */
