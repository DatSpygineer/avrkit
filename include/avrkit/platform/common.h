#ifndef AVRKIT_PLATFORM_COMMON_H
#define AVRKIT_PLATFORM_COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	ARDUINO_UNO,
	ARDUINO_NANO,
	ARDUINO_MEGA,
	ARDUINO_MICRO,
	ARDUINO_LEONARDO
} __attribute__((__packed__)) arduino_board_t;

typedef enum {
	RESET_REASON_WATCHDOG = 0b1000,
	RESET_REASON_BROWNOUT = 0b0100,
	RESET_REASON_EXT_ISR  = 0b0010,
	RESET_REASON_POWER_ON = 0b0001
} __attribute__((__packed__)) reset_reason_t;

typedef uint8_t pin_t;
typedef volatile uint8_t reg_t;

reset_reason_t get_reset_reason(void);

#endif