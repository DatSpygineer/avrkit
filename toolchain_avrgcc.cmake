# ---------------------------------------------------------------------------------------------------
# Based on https://github.com/mkleemann/cmake-avr/blob/master/generic-gcc-avr.cmake by mkleemann
# ---------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.30)

set(AVRKIT_MCU atmega328p CACHE STRING "MCU")

if (NOT DEFINED AVRKIT_MCU)
	message(FATAL_ERROR "CMake variable \"AVRKIT_MCU\" is not set!")
endif()

message(STATUS "Using Micro controller \"${AVRKIT_MCU}\"")

if (NOT AVRKIT_FCPU)
	message(WARNING "CMake variable \"AVRKIT_FCPU\" is not set! Defaulting to 16000000")
	set(AVRKIT_FCPU 16000000)
endif()

if (NOT AVRKIT_BAUD)
	message(WARNING "CMake variable \"AVRKIT_BAUD\" is not set! Defaulting to 115200")
	set(AVRKIT_BAUD 115200)
endif()

find_program(AVRGCC_PATH     avr-gcc     REQUIRED)
find_program(AVRGXX_PATH     avr-g++     REQUIRED)
find_program(AVROBJCPY_PATH  avr-objcopy REQUIRED)
find_program(AVRSIZE_PATH    avr-size    REQUIRED)
find_program(AVROBJDUMP_PATH avr-objdump REQUIRED)
find_program(AVRAR_PATH      avr-ar      REQUIRED)
find_program(AVRLD_PATH      avr-ld      REQUIRED)
find_program(AVRRANLIB_PATH  avr-ranlib  REQUIRED)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

set(CMAKE_ASM_COMPILER ${AVRGCC_PATH}   )
set(CMAKE_C_COMPILER   ${AVRGCC_PATH}   )
set(CMAKE_CXX_COMPILER ${AVRGXX_PATH}   )
set(CMAKE_AR           ${AVRAR_PATH}    )
set(CMAKE_OBJCOPY      ${AVROBJCPY_PATH})
set(CMAKE_SIZE         ${AVRSIZE_PATH}  )
set(CMAKE_LINKER       ${AVRLD_PATH}    )
set(CMAKE_RANLIB       ${AVRRANLIB_PATH})

set(CMAKE_EXECUTABLE_SUFFIX .elf)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS   "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_C_FLAGS_DEBUG   ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${AVRKIT_MCU})

add_compile_options(
	-mmcu=${AVRKIT_MCU} # MCU
	-std=gnu99 # C99 standard
	-Os # optimize
	-Wall # enable warnings
	-Wno-main
	-Wundef
	-pedantic
	-Wstrict-prototypes
	-Werror
	-Wfatal-errors
	-Wl,--relax,--gc-sections
	-g
	-gdwarf-2
	-funsigned-char # a few optimizations
	-funsigned-bitfields
	-fpack-struct
	-fshort-enums
	-ffunction-sections
	-fdata-sections
	-fno-split-wide-types
	-fno-tree-scev-cprop
)

add_definitions(
	-DF_CPU=${AVRKIT_FCPU}
	-DBAUD=${AVRKIT_BAUD}
)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

function (add_avrkit_executable TARGET_NAME)
	add_executable(${TARGET_NAME} ${ARGN})
	target_link_libraries(${TARGET_NAME} PRIVATE avrkit)
	set_target_properties(${TARGET_NAME}
		PROPERTIES
		LINK_FLAGS "-mmcu=${AVRKIT_MCU} -Wl,--gc-sections -mrelax -Wl,-Map,${TARGET_NAME}.map"
		SUFFIX .elf
	)

	if (APPLE)
		set(AVR_SIZE_ARGS -B)
	else()
		set(AVR_SIZE_ARGS -C;--mcu=${AVRKIT_MCU})
	endif()

	add_custom_command(
		OUTPUT  ${TARGET_NAME}.hex
		COMMAND ${AVROBJCPY_PATH} -j .text -j .data -O ihex ${TARGET_NAME}.elf ${TARGET_NAME}.hex
		COMMAND ${AVRSIZE_PATH} ${AVR_SIZE_ARGS} ${TARGET_NAME}.elf
		DEPENDS ${TARGET_NAME}
	)
	add_custom_command(
		OUTPUT  ${TARGET_NAME}.lst
		COMMAND ${AVROBJDUMP_PATH} -d ${TARGET_NAME}.elf > ${TARGET_NAME}.lst
		DEPENDS ${TARGET_NAME}
	)
	add_custom_command(
		OUTPUT ${TARGET_NAME}-eeprom.hex
		COMMAND ${AVROBJCPY_PATH} -j .eeprom --set-section-flags=.eeprom=alloc,load
		--change-section-lma .eeprom=0 --no-change-warnings
		-O ihex ${TARGET_NAME}.elf ${TARGET_NAME}-eeprom.hex
	)
	add_custom_target(
		${TARGET_NAME}.elf
		ALL
		DEPENDS ${TARGET_NAME}.hex ${TARGET_NAME}.lst ${TARGET_NAME}-eeprom.hex
	)
	get_directory_property(clean_files ADDITIONAL_MAKE_CLEAN_FILES)
	set_directory_properties(
		PROPERTIES
		ADDITIONAL_MAKE_CLEAN_FILES ${TARGET_NAME}.map ${TARGET_NAME}.hex ${TARGET_NAME}-eeprom.hex
	)
	add_custom_target(upload_${TARGET_NAME}
		py ${SCRIPT_BASE_PATH}/upload.py
		--hex    ${TARGET_NAME}.hex
		--mcu    ${AVRKIT_MCU}
		--baud   ${AVRKIT_BAUD}
		DEPENDS ${TARGET_NAME}.elf
	)
	add_custom_target(upload_eeprom_${TARGET_NAME}
		py ${SCRIPT_BASE_PATH}/upload.py
		--hex    ${TARGET_NAME}-eeprom.hex
		--mcu    ${AVRKIT_MCU}
		--baud   ${AVRKIT_BAUD}
		--eeprom
		DEPENDS ${TARGET_NAME}.elf
	)
endfunction()
