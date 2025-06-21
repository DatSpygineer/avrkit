#include "avrkit/wdt.h"
#include "avrkit/isr.h"

static wdt_isr_callback_t s_wdt_callback = NULL;

void wdt_enable(const wdt_mode_t mode, const wdt_timeout_t timeout, bool allow_wdt_change) {
	reg_t* wdtcsr = WDT_CONTROL_REGISTER;
	*wdtcsr = (mode & 1) << 6 | ((mode >> 1) & 1) << 3 | ((timeout >> 3) & 1) << 5 | timeout & 0b111 | (allow_wdt_change & 1) << 4;
}
void wdt_disable(void) {

}
void wdt_set_isr_callback(const wdt_isr_callback_t callback) {
	s_wdt_callback = callback;
}

ISR(WDT_vect) {
	if (s_wdt_callback != NULL) {
		s_wdt_callback();
	}
}