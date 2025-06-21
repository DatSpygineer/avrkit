cmake_minimum_required(VERSION 3.30)

project(avrkit_uart C)
project(avrkit_spi C)
project(avrkit_i2c C)
project(avrkit_wdt C)
project(avrkit C)

set(CMAKE_C_STANDARD 11)

set(SCRIPT_BASE_PATH ${CMAKE_CURRENT_LIST_DIR})

add_library(avrkit STATIC
	${CMAKE_CURRENT_LIST_DIR}/include/avrkit.h
	${CMAKE_CURRENT_LIST_DIR}/include/avrkit/gpio.h
	${CMAKE_CURRENT_LIST_DIR}/include/avrkit/isr.h
	${CMAKE_CURRENT_LIST_DIR}/include/avrkit/platform/common.h
	${CMAKE_CURRENT_LIST_DIR}/include/avrkit/platform/${AVRKIT_MCU}.h

	${CMAKE_CURRENT_LIST_DIR}/src/avrkit_platform.c
	${CMAKE_CURRENT_LIST_DIR}/src/avrkit_gpio.c
	${CMAKE_CURRENT_LIST_DIR}/src/avrkit_isr.c
	${CMAKE_CURRENT_LIST_DIR}/src/avrkit_timer.c
)
target_compile_definitions(avrkit PUBLIC
	-DF_CPU=${AVRKIT_FCPU}
	-DBAUD=${AVRKIT_BAUD}
	-DAVRKIT_PLATFORM_HEADER="avrkit/platform/${AVRKIT_MCU}.h"
)
target_include_directories(avrkit PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)

if (AVRKIT_ARDUINO)
	target_compile_definitions(avrkit PUBLIC -DARDUINO=${AVRKIT_ARDUINO})
endif()

add_library(avrkit_wdt STATIC
	${CMAKE_CURRENT_LIST_DIR}/include/avrkit/wdt.h
	${CMAKE_CURRENT_LIST_DIR}/src/avrkit_wdt.c
)
target_link_libraries(avrkit_wdt PUBLIC avrkit)

add_library(avrkit_i2c STATIC
	${CMAKE_CURRENT_LIST_DIR}/include/avrkit/i2c.h
	${CMAKE_CURRENT_LIST_DIR}/src/avrkit_i2c.c
)
target_link_libraries(avrkit_i2c PUBLIC avrkit)

add_library(avrkit_spi STATIC
	${CMAKE_CURRENT_LIST_DIR}/include/avrkit/spi.h
	${CMAKE_CURRENT_LIST_DIR}/src/avrkit_spi.c
)
target_link_libraries(avrkit_spi PUBLIC avrkit)

add_library(avrkit_uart STATIC
	${CMAKE_CURRENT_LIST_DIR}/include/avrkit/uart.h
	${CMAKE_CURRENT_LIST_DIR}/src/avrkit_uart.c
)
target_link_libraries(avrkit_uart PUBLIC avrkit)

add_library(avrkit::avrkit ALIAS avrkit)
add_library(avrkit::wdt    ALIAS avrkit_wdt)
add_library(avrkit::i2c    ALIAS avrkit_i2c)
add_library(avrkit::spi    ALIAS avrkit_spi)
add_library(avrkit::uart   ALIAS avrkit_uart)

add_subdirectory(example)
