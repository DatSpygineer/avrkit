#include "avrkit/gpio.h"

#define GPIO_GET_PORT(__x) ((reg_t*)(GPIO_PORT_START_ADDRESS + ((((__x) >> 4) & 0xF) * 3)))
#define GPIO_GET_DDR(__x)  ((reg_t*)(GPIO_DDR_START_ADDRESS  + ((((__x) >> 4) & 0xF) * 3)))

void gpio_init(const pin_t pin, const gpio_mode_t mode) {
	reg_t* ddr = GPIO_GET_DDR(pin);
	if (mode == GPIO_OUTPUT) {
		*ddr |= 1 << (pin & 0xF);
	} else {
		*ddr &= ~(1 << (pin & 0xF));
	}
}
uint8_t gpio_get(const pin_t pin) {
	const reg_t* port = GPIO_GET_PORT(pin);
	return *port >> (pin & 0xF) & 1;
}
void gpio_set(const pin_t pin, const uint8_t value) {
	reg_t* port = GPIO_GET_PORT(pin);
	if (value != 0) {
		*port |= 1 << (pin & 0xF);
	} else {
		*port &= ~(1 << (pin & 0xF));
	}
}
void gpio_toggle(const pin_t pin) {
	reg_t* port = GPIO_GET_PORT(pin);
	*port ^= 1 << (pin & 0xF);
}

uint8_t gpio_get_port(const uint8_t port) {
	return *(reg_t*)(GPIO_PORT_START_ADDRESS + (port * 3));
}
void gpio_set_port(const uint8_t port, const uint8_t value, const uint8_t mask) {
	reg_t* port_reg = (reg_t*)(GPIO_PORT_START_ADDRESS + (port * 3));
	*port_reg = (*port_reg & ~mask) | (value & mask);
}
void gpio_toggle_port(const uint8_t port, const uint8_t mask) {
	reg_t* port_reg = (reg_t*)(GPIO_PORT_START_ADDRESS + (port * 3));
	*port_reg ^= mask;
}