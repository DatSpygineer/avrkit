#ifndef AVRKIT_H
#define AVRKIT_H

#include "avrkit/gpio.h"
#include "avrkit/isr.h"

#include <util/delay.h>

#define endless_loop for(;;)

static __inline__ void sleep(const double s)     { _delay_ms(s * 1000.0); }
static __inline__ void sleep_ms(const double ms) { _delay_ms(ms); }
static __inline__ void sleep_us(const double us) { _delay_us(us); }

#ifndef AVRKIT_NO_MAIN

void setup(void);
void loop(void);

int main(void) { // NOLINT: "main" cannot be inline
	setup();
	endless_loop {
		loop(); // NOLINT: Main program loop
	}
}

#endif

#endif // AVRKIT_H