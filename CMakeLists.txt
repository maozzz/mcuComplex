
#${templateWarning}
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.7)

set(COMPILER_PATH /usr/local/gcc_arm/bin/)

# specify the cross compiler
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER ${COMPILER_PATH}arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER ${COMPILER_PATH}arm-none-eabi-g++)
set(AS ${COMPILER_PATH}arm-none-eabi-as)
set(AR ${COMPILER_PATH}arm-none-eabi-ar)
set(OBJCOPY ${COMPILER_PATH}arm-none-eabi-objcopy)
set(OBJDUMP ${COMPILER_PATH}arm-none-eabi-objdump)
set(SIZE ${COMPILER_PATH}arm-none-eabi-size)
set(mcpu cortex-m3)

set(linkerScript emdr1986x-std-per-lib/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/MDR32F9Qx.ld)


SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/${linkerScript})

#Uncomment for hardware floating point
#SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
#add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1)

#Uncomment for software floating point
#SET(FPU_FLAGS "-mfloat-abi=soft")

SET(COMMON_FLAGS
        "-mcpu=${mcpu} -mthumb -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wshadow -Wredundant-decls -Wno-missing-field-initializers -Wconversion -Wno-packed-bitfield-compat -pipe")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS "-u _printf_float -specs=nano.specs -specs=nosys.specs -Wl,-gc-sections,--print-memory-usage -T ${LINKER_SCRIPT}")

project(mcuComplex C CXX ASM)

set(CMAKE_CXX_STANDARD 14)

#add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1)
add_definitions(${defines})

file(GLOB_RECURSE SOURCES
        src/main.cpp
        src/*.*
        emdr1986x-std-per-lib/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/startup_MDR32F9Qx.S
        emdr1986x-std-per-lib/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/system_MDR32F9Qx.c
                emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_adc.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_arinc429r.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_arinc429t.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_audio.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_bkp.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_can.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_comp.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_dac.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_dma.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_dsp.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_ebc.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_eeprom.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_i2c.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_iwdg.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_keypad.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_led.c
        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_port.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_power.c
        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_rst_clk.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_sdio.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_ssp.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_timer.c
                emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_uart.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_usb.c
        #        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_wwdg.c

        rtos/*.c
        )

include_directories(
        inc
        src
        emdr1986x-std-per-lib
        emdr1986x-std-per-lib/config
        emdr1986x-std-per-lib/CMSIS/CM3/CoreSupport
        emdr1986x-std-per-lib/CMSIS/CM3/DeviceSupport/MDR32F9Qx/inc
        emdr1986x-std-per-lib/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup
        emdr1986x-std-per-lib/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc
        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/inc
        emdr1986x-std-per-lib/MDR32F9Qx_StdPeriph_Driver/inc/USB_Library

        rtos/include
)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
set(SREC_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.srec)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${SREC_FILE}
        COMMENT "Building ${HEX_FILE}eag
Building ${BIN_FILE}")


