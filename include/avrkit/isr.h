#ifndef AVRKIT_ISR_H
#define AVRKIT_ISR_H

#include "avrkit/platform/common.h"

#define enable_isr()      __asm__ __volatile__ ("sei" ::: "memory")
#define disable_isr()     __asm__ __volatile__ ("cli" ::: "memory")
#define return_from_isr() __asm__ __volatile__ ("reti" ::)

#ifdef __cplusplus
#  define ISR(vector, ...)            \
extern "C" void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__; \
void vector (void)
#else
#  define ISR(vector, ...)            \
void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__; \
void vector (void)
#endif

#define BADISR_vect __vector_default
#define ISR_BLOCK
#define ISR_NOBLOCK    __attribute__((interrupt))
#define ISR_NAKED      __attribute__((naked))
#define ISR_ALIASOF(v) __attribute__((alias(__STRINGIFY(v))))

typedef uint8_t isrno_t;
typedef void (*isr_gpio_callback_t)(pin_t);

typedef enum {
	ISR_TRIGGER_LOW,
	ISR_TRIGGER_ANY,
	ISR_TRIGGER_FALLING,
	ISR_TRIGGER_RISING
} __attribute__((__packed__)) isr_trigger_t;

void isr_set_external_trigger_mode(isrno_t idx, isr_trigger_t trigger_mode);
void isr_set_external_callback(isrno_t idx, isr_gpio_callback_t callback);
isr_gpio_callback_t isr_get_external_callback(isrno_t idx);

void isr_set_gpio_interrupt(pin_t pin, isr_gpio_callback_t callback);
isr_gpio_callback_t isr_get_gpio_interrupt(pin_t pin);
bool isr_gpio_is_enabled(pin_t pin);

#endif