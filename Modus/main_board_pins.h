#ifndef MAIN_BOARD_PINS_H_
#define MAIN_BOARD_PINS_H_

#include "cyhal.h"

// SD CARD
#define PIN_SD_CARD_MOSI P5_0
#define PIN_SD_CARD_MISO P5_1
#define PIN_SD_CARD_SCLK P5_2
#define PIN_SD_CARD_CS	 P5_3

// PUSH BUTTONS
#define PIN_START_RIDE_PB P5_6
#define PIN_BT_PAIR_PB P6_2
#define PIN_TOGGLE_DISPLAY_PB P6_3

// REED SWITCH
#define PIN_REED_SWITCH P6_5

// SEGMENTS
#define PIN_SEG_EN_DP P5_6
#define PIN_SEG_EN_A P9_0
#define PIN_SEG_EN_B P9_1
#define PIN_SEG_EN_C P9_2
#define PIN_SEG_EN_D P9_3
#define PIN_SEG_EN_E P9_4
#define PIN_SEG_EN_F P9_5
#define PIN_SEG_EN_G P9_6

// DIGITS
#define PIN_DIG0_N P10_5
#define PIN_DIG1_N P10_6
#define PIN_DIG2_N P12_6
#define PIN_DIG3_N P12_7

// RADIO TX
#define PIN_TX_RADIO_DATA_IN P10_4

// GPS
#define PIN_GPS_SCL P10_0
#define PIN_GPS_SDA P10_1

//LEDS
#define PIN_USER_LED P5_5

#endif

