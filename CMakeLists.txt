#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.21)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_C_COMPILER E:/tools/bin/arm-none-eabi-gcc.exe)
    set(CMAKE_CXX_COMPILER E:/tools/bin/arm-none-eabi-g++.exe)
    set(CMAKE_ASM_COMPILER  E:/tools/bin/arm-none-eabi-gcc.exe)
    set(CMAKE_AR E:/tools/bin/arm-none-eabi-ar.exe)
    set(CMAKE_LINKER E:/tools/bin/arm-none-eabi-ld)
    set(CMAKE_OBJCOPY E:/tools/bin/arm-none-eabi-objcopy.exe)
    set(CMAKE_OBJDUMP E:/tools/bin/arm-none-eabi-objdump.exe)
    set(SIZE E:/tools/bin/arm-none-eabi-size.exe)
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
    set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
    set(CMAKE_AR arm-none-eabi-ar)
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CMAKE_OBJDUMP arm-none-eabi-objdump)
    set(SIZE arm-none-eabi-size)
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
else()
    message(FATAL_ERROR "Unknown host system")
endif()

# project settings
project(STM32F4 CXX C)
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
find_package(Boost)
include_directories(${Boost_INCLUDE_DIR})


#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_compile_options(-mfloat-abi=soft -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nosys.specs -specs=nano.specs)
#add_link_options(-mfloat-abi=soft -mfpu=fpv4-sp-d16 -specs=nosys.specs -specs=nano.specs)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "ReleaseMain")
    message(STATUS "Release O2")
    add_compile_options(-O2 )
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(Core/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc
        Drivers/CMSIS/Device/ST/STM32F4xx/Include
        Drivers/CMSIS/Include
        libs/print
        libs/logger
        libs/MWR
        libs/uartbuf
        Drivers/ClokControl
        Drivers/GPIO
        Drivers/USART
        Drivers/NVIC
        Drivers/EXTI
        Drivers/SYSCFG
        Drivers/FLASH
        Drivers/IWDG
        Drivers/PWR
        Drivers/RNG
        Drivers/TIMERS
        Drivers/SPI
        devices/flash
        devices/display
        devices/adc
        Drivers/DMA
        src/)

add_definitions(-DDEBUG -DSTM32F407xx -DUSE_FULL_LL_DRIVER -DHSE_VALUE=25000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1)

file(GLOB_RECURSE SOURCES "Core/*.*" "Drivers/*.*")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VETX_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

#--------------------------------------------------------------CPP------------------------------------------------------------------------------
set(PROJECT_NAME_CPP stm32f4 )

set(SOURCE_CPP src/main.cpp
               libs/print/cout.cpp

               Drivers/GPIO/gpio.cpp
               Drivers/USART/usart.cpp
               Drivers/NVIC/nvic.cpp
               Drivers/GPIO/gpio.cpp
               Drivers/Startup/StartupSTM32F407.cpp
               Drivers/EXTI/exti.cpp
               Drivers/SYSCFG/syscfg.cpp
               Drivers/ClokControl/clockControl.cpp
               Drivers/ClokControl/systick.cpp
               Drivers/EXTI/exti.cpp
               Drivers/FLASH/flash.cpp
               Drivers/IWDG/iwdg.cpp
               Drivers/PWR/pwr.cpp
               Drivers/TIMERS/basictimer.cpp
               Drivers/DMA/dma.cpp
        Drivers/RNG/rng.cpp
        Drivers/SPI/spi.cpp
        devices/flash/w25q16.cpp
        devices/display/st7789.cpp
        devices/display/consolas_9pt_bold.cpp
        libs/print/printf.c
        devices/adc/ad7705.cpp
        libs/uartbuf/uartbuf.hpp
        libs/uartbuf/uartbuf.cpp libs/logger/format_check.hpp libs/logger/logger.hpp)

add_executable(${PROJECT_NAME_CPP}.elf ${SOURCE_CPP} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME_CPP}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME_CPP}.bin)

add_custom_command(TARGET ${PROJECT_NAME_CPP}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME_CPP}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME_CPP}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

find_program(UPLOADER st-flash )

if( UPLOADER)

    add_custom_target(upload
    WORKING_DIRECTORY
        ${CMAKE_BINARY_DIR}
    COMMAND
        ${UPLOADER} --reset write ${BIN_FILE} 0x8000000
    DEPENDS
        ${BIN_FILE}
    COMMENT
        "upload via st-flash"
    VERBATIM
  )

add_dependencies(upload ${PROJECT_NAME_CPP}.elf )

endif()