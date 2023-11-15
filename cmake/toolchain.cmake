set(CMAKE_SYSTEM_NAME "Generic")

find_path(PATH_ARM_GCC arm-none-eabi-gcc)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(EXTENSION_FILE .exe)
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(EXTENSION_FILE )
else()
    message(FATAL_ERROR "Unknown system")
endif()

find_path(PATH_ARM_GCC arm-none-eabi-gcc$(EXTENSION_FILE))

message("==============")
message("PATH_ARM_GCC ${PATH_ARM_GCC}" )

if(${PATH_ARM_GCC} EQUAL PATH_ARM_GCC-NOTFOUND)
    message(FATAL_ERROR "Toolchain not found")
else()
    message("SET TOOLCHAIN")
    set(CMAKE_C_COMPILER ${PATH_ARM_GCC}/arm-none-eabi-gcc${EXTENSION_FILE})
    set(CMAKE_CXX_COMPILER ${PATH_ARM_GCC}/arm-none-eabi-g++${EXTENSION_FILE})
    set(CMAKE_ASM_COMPILER  ${PATH_ARM_GCC}/arm-none-eabi-gcc${EXTENSION_FILE})
    set(CMAKE_AR ${PATH_ARM_GCC}/arm-none-eabi-ar${EXTENSION_FILE})
    set(CMAKE_LINKER ${PATH_ARM_GCC}/arm-none-eabi-ld${EXTENSION_FILE})
    set(CMAKE_OBJCOPY ${PATH_ARM_GCC}/arm-none-eabi-objcopy${EXTENSION_FILE})
    set(CMAKE_OBJDUMP ${PATH_ARM_GCC}/arm-none-eabi-objdump${EXTENSION_FILE})
    set(SIZE ${PATH_ARM_GCC}/arm-none-eabi-size${EXTENSION_FILE})
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
endif()

