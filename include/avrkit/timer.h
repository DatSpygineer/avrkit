#ifndef AVRKIT_TIMER_H
#define AVRKIT_TIMER_H

#include AVRKIT_PLATFORM_HEADER

typedef void (*timer_callback_t)(void);

void timer_set_us(uint8_t idx, bool one_shot, uint32_t period_us, timer_callback_t callback);
inline void timer_set_ms(uint8_t idx, bool one_shot, uint32_t period_ms, timer_callback_t callback) {
	timer_set_us(idx, one_shot, period_ms * 1000, callback);
}
inline void timer_set   (uint8_t idx, bool one_shot, double period_s, timer_callback_t callback) {
	timer_set_us(idx, one_shot, (uint32_t)(period_s * 10000000), callback);
}

#endif