#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.21)


# project settings
project(STM32F4 CXX C)
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
find_package(Boost)
include_directories(${Boost_INCLUDE_DIR})

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

file(GLOB_RECURSE SOURCES "Core/*.*" "Drivers/*.*")

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