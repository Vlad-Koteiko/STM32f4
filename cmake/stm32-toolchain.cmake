set(CMAKE_SYSTEM_NAME "Generic")

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


add_definitions(-DDEBUG -DSTM32F407xx -DUSE_FULL_LL_DRIVER -DHSE_VALUE=25000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1)


set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VETX_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

