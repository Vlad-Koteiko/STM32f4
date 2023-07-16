For build. In build directory:

conan install .. -of . 
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/stm32-toolchain.cmake
cmake --build . 

For upload

cmake --build . --target upload

To build docker image:

sudo docker build -t stm32 .

To run docker image.

docker run -it --rm -v $(pwd):/build --device /dev/bus/usb/001/007 stm32 

001 is usb bus number, 007 device ID. To get this numbers run lsusb. Example 

Bus 001 Device 007: ID 0483:3748 STMicroelectronics ST-LINK/V2
