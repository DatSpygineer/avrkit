#ifndef AVRKIT_WDT_H
#define AVRKIT_WDT_H

#include AVRKIT_PLATFORM_HEADER

typedef enum {
	WDT_MODE_STOPPED,
	WDT_MODE_INTERRUPT,
	WDT_MODE_SYSTEM_RESET,
	WDT_MODE_INTERRUPT_AND_SYSTEM_RESET
} __attribute__((__packed__)) wdt_mode_t;

typedef enum {
	WDT_TIMEOUT_16MS,
	WDT_TIMEOUT_32MS,
	WDT_TIMEOUT_64MS,
	WDT_TIMEOUT_125MS,
	WDT_TIMEOUT_250MS,
	WDT_TIMEOUT_500MS,
	WDT_TIMEOUT_1S,
	WDT_TIMEOUT_2S,
	WDT_TIMEOUT_4S,
	WDT_TIMEOUT_8S
} __attribute__((__packed__)) wdt_timeout_t;

typedef void (*wdt_isr_callback_t)(void);

void wdt_enable(wdt_mode_t mode, wdt_timeout_t timeout, bool allow_wdt_change);
void wdt_disable(void);
void wdt_set_isr_callback(wdt_isr_callback_t callback);

#endif