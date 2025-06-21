#include "avrkit/timer.h"

typedef struct {
	uint32_t scale, bits;
} prescaler_t;

static const prescaler_t s_prescalers[][] = {
	// Timer0 prescalers
	{
		{    1, 0b001 },
		{    8, 0b010 },
		{   64, 0b011 },
		{  256, 0b100 },
		{ 1024, 0b101 }
	},
	// Timer1 prescalers
	{
		{    1, 0b001 },
		{    8, 0b010 },
		{   64, 0b011 },
		{  256, 0b100 },
		{ 1024, 0b101 }
	},
	// Timer2 prescalers
	{
		{    1, 0b001 },
		{    8, 0b010 },
		{   32, 0b011 },
		{   64, 0b100 },
		{  128, 0b101 },
		{  256, 0b110 },
		{ 1024, 0b111 }
	}
};

static uint8_t timer_get_prescaler(uint8_t idx, uint32_t freq) {

}

void timer_set_us(uint8_t idx, bool one_shot, uint32_t period_us, timer_callback_t callback) {

}
