#ifndef AVRKIT_PLATFORM_ATMEGA328P_H
#define AVRKIT_PLATFORM_ATMEGA328P_H

#include "avrkit/platform/common.h"

// == Misc registers ==

#define MCU_STATIS_REGISTER ((reg_t*)0x55)

// == GPIO registers ==

#define GPIO_PIN_START_ADDRESS  0x23
#define GPIO_DDR_START_ADDRESS  0x24
#define GPIO_PORT_START_ADDRESS 0x25
#define GPIO_PORT_COUNT 3

enum {
	PB0 = 0x00,
	PB1 = 0x01,
	PB2 = 0x02,
	PB3 = 0x03,
	PB4 = 0x04,
	PB5 = 0x05,
	PB6 = 0x06,
	PB7 = 0x07,

	PC0 = 0x10,
	PC1 = 0x11,
	PC2 = 0x12,
	PC3 = 0x13,
	PC4 = 0x14,
	PC5 = 0x15,
	PC6 = 0x16,
	PC7 = 0x17,

	PD0 = 0x20,
	PD1 = 0x21,
	PD2 = 0x22,
	PD3 = 0x23,
	PD4 = 0x24,
	PD5 = 0x25,
	PD6 = 0x26,
	PD7 = 0x27
};

enum {
	GPIO_PORT_B,
	GPIO_PORT_C,
	GPIO_PORT_D
};

#define ARDUINO_PIN(__pin) \
	(__pin) < 8 ? (PD0 + __pin) : \
	(\
		(__pin) < 14 ? (PB0 + (__pin - 8)) : \
		(__pin) - 14 + PC0\
	)

// == External interrupt, pin change interrupt registers ==

#define ISR_EXTERNAL_COUNT 2
#define ISR_CONTROL_REG_0 ((reg_t*)0x69)
#define ISR_EXTERNAL_MASK_REG_0 ((reg_t*)0x3D)
#define ISR_PC_CONTROL_REG_0 ((reg_t*)0x68)

#define ISR_PC_MASK_REG_0 ((reg_t*)0x6B)
#define ISR_PC_MASK_REG_1 ((reg_t*)0x6C)
#define ISR_PC_MASK_REG_2 ((reg_t*)0x6D)

enum {
	ISR_EXTERN_0,
	ISR_EXTERN_1
};

enum {
	ISR_EXTERN_0_PIN = PD2,
	ISR_EXTERN_1_PIN = PD3
};

#ifdef ARDUINO
	enum {
		A0 = 14,
		A1 = 15,
		A2 = 16,
		A3 = 17,
		A4 = 18,
		A5 = 19
	};
#endif

// == Watchdog registers ==

#define WDT_CONTROL_REGISTER ((reg_t*)0x60)

// == ADC registers ==

// == Timer registers ==

#define TIMER_COUNT 3

enum {
	TIMER_0,
	TIMER_1,
	TIMER_2
};

// == PWM registers ==

// == SPI registers ==

// == I2C registers ==

// == UART registers ==

#endif