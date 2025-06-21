#ifndef AVRKIT_GPIO_H
#define AVRKIT_GPIO_H

#include AVRKIT_PLATFORM_HEADER

typedef enum {
	GPIO_INPUT,
	GPIO_OUTPUT
} __attribute__((__packed__)) gpio_mode_t;

void    gpio_init(pin_t pin, gpio_mode_t mode);
uint8_t gpio_get(pin_t pin);
void    gpio_set(pin_t pin, uint8_t value);
void    gpio_toggle(pin_t pin);

uint8_t gpio_get_port(uint8_t port);
void    gpio_set_port(uint8_t port, uint8_t value, uint8_t mask);
void    gpio_toggle_port(uint8_t port, uint8_t mask);

#endif