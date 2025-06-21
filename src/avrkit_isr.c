#include "avrkit/isr.h"

#include <string.h>

#include AVRKIT_PLATFORM_HEADER

static isr_gpio_callback_t s_extern_isr[ISR_EXTERNAL_COUNT] = { 0 };
static isr_gpio_callback_t s_pc_isr[GPIO_PORT_COUNT * 8] = { 0 };
static const isr_gpio_callback_t s_pc_isr_empty[8] = { 0 };

void isr_set_external_trigger_mode(isrno_t idx, isr_trigger_t trigger_mode) {
	const uint8_t mask = 0b11 << (idx << 1);
	reg_t* eicra = ISR_CONTROL_REG_0;
	*eicra = (*eicra & ~mask) | (trigger_mode << (idx << 1)) & mask;
}

void isr_set_external_callback(const isrno_t idx, const isr_gpio_callback_t callback) {
	if (s_extern_isr[idx] == NULL && callback != NULL) {
		*ISR_EXTERNAL_MASK_REG_0 |= 1 << idx;
	} else if (s_extern_isr[idx] != NULL && callback == NULL) {
		*ISR_EXTERNAL_MASK_REG_0 &= ~(1 << idx);
	}
	s_extern_isr[idx] = callback;
}
isr_gpio_callback_t isr_get_external_callback(const isrno_t idx) {
	return s_extern_isr[idx];
}

void isr_set_gpio_interrupt(const pin_t pin, const isr_gpio_callback_t callback) {
	const uint8_t port = (pin >> 4) & 0xF;
	const uint8_t pin_idx = (pin & 0xF);

	s_pc_isr[pin_idx + port * 8] = callback;
	if (memcmp(s_pc_isr + port * 8, s_pc_isr_empty, sizeof(isr_gpio_callback_t) * 8) == 0) {
		*ISR_PC_CONTROL_REG_0 &= ~(1 << port);
	} else if (!isr_gpio_is_enabled(pin)) {
		*ISR_PC_CONTROL_REG_0 |= 1 << port;
	}
}
isr_gpio_callback_t isr_get_gpio_interrupt(const pin_t pin) {
	return s_pc_isr[(pin & 0xF) + ((pin >> 4) * 8)];
}
bool isr_gpio_is_enabled(const pin_t pin) {
	const uint8_t int_idx = (pin >> 4) & 0xF;
	return (*ISR_PC_CONTROL_REG_0 >> int_idx) & 1;
}

#if ISR_EXTERNAL_COUNT > 0
ISR(INT0_vect) {
	if (s_extern_isr[0] != NULL) {
		s_extern_isr[0](ISR_EXTERN_0_PIN);
	}
}
#endif

#if ISR_EXTERNAL_COUNT > 1
ISR(INT1_vect) {
	if (s_extern_isr[1] != NULL) {
		s_extern_isr[1](ISR_EXTERN_1_PIN);
	}
}
#endif
#if ISR_EXTERNAL_COUNT > 2
ISR(INT2_vect) {
	if (s_extern_isr[2] != NULL) {
		s_extern_isr[2](ISR_EXTERN_2_PIN);
	}
}
#endif
#if ISR_EXTERNAL_COUNT > 3
ISR(INT3_vect) {
	if (s_extern_isr[3] != NULL) {
		s_extern_isr[3](ISR_EXTERN_3_PIN);
	}
}
#endif
#if ISR_EXTERNAL_COUNT > 4
ISR(INT4_vect) {
	if (s_extern_isr[4] != NULL) {
		s_extern_isr[4](ISR_EXTERN_4_PIN);
	}
}
#endif
#if ISR_EXTERNAL_COUNT > 5
ISR(INT5_vect) {
	if (s_extern_isr[5] != NULL) {
		s_extern_isr[5](ISR_EXTERN_5_PIN);
	}
}
#endif
#if ISR_EXTERNAL_COUNT > 6
ISR(INT6_vect) {
	if (s_extern_isr[6] != NULL) {
		s_extern_isr[6](ISR_EXTERN_6_PIN);
	}
}
#endif
#if ISR_EXTERNAL_COUNT > 7
ISR(INT7_vect) {
	if (s_extern_isr[7] != NULL) {
		s_extern_isr[7](ISR_EXTERN_7_PIN);
	}
}
#endif

#if GPIO_PORT_COUNT > 0
ISR(PCINT0_vect) {
	const reg_t pins = *ISR_PC_MASK_REG_0;
	if (pins & 1 && s_pc_isr[0] != NULL) {
		s_pc_isr[0](PB0);
	}
	if ((pins >> 1) & 1 && s_pc_isr[1] != NULL) {
		s_pc_isr[1](PB1);
	}
	if ((pins >> 2) & 1 && s_pc_isr[2] != NULL) {
		s_pc_isr[2](PB2);
	}
	if ((pins >> 3) & 1 && s_pc_isr[3] != NULL) {
		s_pc_isr[3](PB3);
	}
	if ((pins >> 4) & 1 && s_pc_isr[4] != NULL) {
		s_pc_isr[4](PB4);
	}
	if ((pins >> 5) & 1 && s_pc_isr[5] != NULL) {
		s_pc_isr[5](PB5);
	}
	if ((pins >> 6) & 1 && s_pc_isr[6] != NULL) {
		s_pc_isr[6](PB6);
	}
	if ((pins >> 7) & 1 && s_pc_isr[7] != NULL) {
		s_pc_isr[7](PB7);
	}
}
#endif
#if GPIO_PORT_COUNT > 1
ISR(PCINT1_vect) {
	const reg_t pins = *ISR_PC_MASK_REG_1;
	if (pins & 1 && s_pc_isr[8] != NULL) {
		s_pc_isr[8](PC0);
	}
	if ((pins >> 1) & 1 && s_pc_isr[9] != NULL) {
		s_pc_isr[9](PC1);
	}
	if ((pins >> 2) & 1 && s_pc_isr[10] != NULL) {
		s_pc_isr[10](PC2);
	}
	if ((pins >> 3) & 1 && s_pc_isr[11] != NULL) {
		s_pc_isr[11](PC3);
	}
	if ((pins >> 4) & 1 && s_pc_isr[12] != NULL) {
		s_pc_isr[12](PC4);
	}
	if ((pins >> 5) & 1 && s_pc_isr[13] != NULL) {
		s_pc_isr[13](PC5);
	}
	if ((pins >> 6) & 1 && s_pc_isr[14] != NULL) {
		s_pc_isr[14](PC6);
	}
	if ((pins >> 7) & 1 && s_pc_isr[15] != NULL) {
		s_pc_isr[15](PC7);
	}
}
#endif
#if GPIO_PORT_COUNT > 2
ISR(PCINT2_vect) {
	const reg_t pins = *ISR_PC_MASK_REG_2;
	if (pins & 1 && s_pc_isr[16] != NULL) {
		s_pc_isr[16](PD0);
	}
	if ((pins >> 1) & 1 && s_pc_isr[17] != NULL) {
		s_pc_isr[17](PD1);
	}
	if ((pins >> 2) & 1 && s_pc_isr[18] != NULL) {
		s_pc_isr[18](PD2);
	}
	if ((pins >> 3) & 1 && s_pc_isr[19] != NULL) {
		s_pc_isr[19](PD3);
	}
	if ((pins >> 4) & 1 && s_pc_isr[20] != NULL) {
		s_pc_isr[20](PD4);
	}
	if ((pins >> 5) & 1 && s_pc_isr[21] != NULL) {
		s_pc_isr[21](PD5);
	}
	if ((pins >> 6) & 1 && s_pc_isr[22] != NULL) {
		s_pc_isr[22](PD6);
	}
	if ((pins >> 7) & 1 && s_pc_isr[23] != NULL) {
		s_pc_isr[23](PD7);
	}
}
#endif