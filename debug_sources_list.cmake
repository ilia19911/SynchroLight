
## ------- TARGET STARTS HERE -------
set(TARGET_NAME debug)
set(EXEC_NAME debug_STM32F103CB)
add_executable(${EXEC_NAME})

set(MCU STM32F103CB)
target_compile_definitions(${EXEC_NAME} PRIVATE USE_STDPERIPH_DRIVER  HSE_VALUE=16000000  STM32F10X_MD)
target_compile_definitions(${EXEC_NAME} PRIVATE )
target_include_directories(${EXEC_NAME} PRIVATE .//)
target_include_directories(${EXEC_NAME} PRIVATE .//CMSIS)
target_include_directories(${EXEC_NAME} PRIVATE .//Hdr)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//StdPeriph//inc)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//unilib)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//unilib//external_devices)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//unilib//file_io)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//unilib//mcs)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//unilib//mcs//arm_cm3)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//unilib//mcs//stm32)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//unilib//crc)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//fatfs)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//unilib//ic_drivers)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs//leds)
target_include_directories(${EXEC_NAME} PRIVATE .//commonLibs)
target_include_directories(${EXEC_NAME} PRIVATE ./)

target_include_directories(${EXEC_NAME} PRIVATE /n            ')
target_include_directories(${EXEC_NAME} PRIVATE ./)

set(COMPILE_FLAGS -gdwarf-2 -Og -mcpu=cortex-m3 -mthumb -mfloat-abi=soft  -mno-thumb-interwork -v)
set(STARTUP_CODE_SOURCE ${STARTUP_CODE_DIR}/startup_stm32f103xb.s)
set(LDSCRIPT ${STARTUP_CODE_DIR}/STM32F103C8Tx_FLASH.ld)
set	(GROUP_SRC_CMSIS
		CMSIS/core_cm3.c
		CMSIS/system_stm32f10x.c
#		CMSIS/startup_stm32f10x_md.s
	)

set	(GROUP_SRC_STDPERIPH
		../commonLibs/StdPeriph/src/misc.c
		../commonLibs/StdPeriph/src/stm32f10x_crc.c
		../commonLibs/StdPeriph/src/stm32f10x_dma.c
		../commonLibs/StdPeriph/src/stm32f10x_flash.c
		../commonLibs/StdPeriph/src/stm32f10x_gpio.c
		../commonLibs/StdPeriph/src/stm32f10x_rcc.c
		../commonLibs/StdPeriph/src/stm32f10x_spi.c
		../commonLibs/StdPeriph/src/stm32f10x_usart.c
		../commonLibs/StdPeriph/src/stm32f10x_iwdg.c
		../commonLibs/StdPeriph/src/stm32f10x_tim.c
	)

set	(GROUP_SRC_UNILIB
		../commonLibs/unilib/endianes.c
		../commonLibs/unilib/fifo.c
		../commonLibs/unilib/external_devices/sx1276_transc.c
		../commonLibs/unilib/mcs/arm_cm3/delay.c
		../commonLibs/unilib/mcs/stm32/crc32.c
		../commonLibs/unilib/mcs/stm32/rcc.c
		../commonLibs/unilib/mcs/stm32/randomizer.c
		../commonLibs/unilib/mcs/stm32/uart.c
		../commonLibs/unilib/mcs/stm32/adclib.c
		../commonLibs/StdPeriph/src/stm32f10x_adc.c
		../commonLibs/unilib/mcs/stm32/systick_timer.c
		../commonLibs/unilib/dprintf.c
		../commonLibs/unilib/spi_bitbang.c
		../commonLibs/unilib/ic_drivers/at25db321_fileStrorage.c
		../commonLibs/unilib/ic_drivers/at25df.c
		../commonLibs/unilib/mcs/stm32/uni_eeprom.c
		../commonLibs/unilib/mcs/stm32/uni_flash.c
	)

set	(GROUP_SRC_SRC
		Src/main.c
		Src/low_level0.c
		Src/it.c
		../commonLibs/rf_parser.c
		../commonLibs/fileTransfer.c
		../commonLibs/display_kingbright.c
		Src/buttons.c
		../commonLibs/display_menu.c
	)

set	(GROUP_SRC_FATFS
		../commonLibs/fatfs/ff.c
		../commonLibs/fatfs/fatfs_diskio_sdcard_spi.c
	)

set	(GROUP_SRC_LEDS
		../commonLibs/leds/player.c
		../commonLibs/leds/scen_memory.c
	)

set	(${EXEC_NAME}_LIST_OF_SOURCES
		${GROUP_SRC_CMSIS}
		${GROUP_SRC_STDPERIPH}
		${GROUP_SRC_UNILIB}
		${GROUP_SRC_SRC}
		${GROUP_SRC_FATFS}
		${GROUP_SRC_LEDS}
	)

target_sources(${EXEC_NAME} PRIVATE ${STARTUP_CODE_SOURCE} ${${EXEC_NAME}_LIST_OF_SOURCES} )

target_compile_options(${EXEC_NAME} PRIVATE ${COMPILE_FLAGS} )

target_link_options(
				${EXEC_NAME} BEFORE PRIVATE 
				"-Wl,-Map=${EXEC_NAME}.map"
				"-Wl,-T${LDSCRIPT}"
				"-Wl,--gc-sections"
				"-Wl,--verbose"
				"-Wl,-V"
				"-Wl,--cref"
				${COMPILE_FLAGS})


target_link_libraries(
				${EXEC_NAME}
				c		# c runtime
				m		# math
				nosys	# for non-os
				)

STM32_ADD_HEX_BIN_TARGETS(${EXEC_NAME})
STM32_PRINT_SIZE_OF_TARGETS(${EXEC_NAME})
