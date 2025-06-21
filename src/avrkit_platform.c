#include AVRKIT_PLATFORM_HEADER

reset_reason_t get_reset_reason(void) {
	return *MCU_STATIS_REGISTER;
}
