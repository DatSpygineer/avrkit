#include <avrkit.h>

#define LED_PIN ARDUINO_PIN(13)

void setup() {
	gpio_init(LED_PIN, GPIO_OUTPUT);
}
void loop() {
	gpio_toggle(LED_PIN);
	sleep(1.0);
}