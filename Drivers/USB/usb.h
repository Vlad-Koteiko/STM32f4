//
// Created by koteiko_vv on 21.02.2023.
//

#ifndef STM32F4_USB_H
#define STM32F4_USB_H

#include "MWR.hpp"
#include "gpio.h"
#include "clockControl.hpp"
#include "nvic.h"

namespace drivers::usb {

    class Usb
    {
        static constexpr std::uintptr_t baseAddress = 0x50000000;
        ClockControl &clockControl;

        enum Register : std::uintptr_t
        {
            SR    = baseAddress,        // USART Status register,                   Address offset: 0x00
            DR    = baseAddress + 0x04, // USART Data register,                     Address offset: 0x04
            BRR   = baseAddress + 0x08, // USART Baud rate register,                Address offset: 0x08
            CR1   = baseAddress + 0x0C, // USART Control register 1,                Address offset: 0x0C
            CR2   = baseAddress + 0x10, // USART Control register 2,                Address offset: 0x10
            CR3   = baseAddress + 0x14, // USART Control register 3,                Address offset: 0x14
            GTPR  = baseAddress + 0x18  // USART Guard time and prescaler register, Address offset: 0x18
        };

    public:
        Usb(ClockControl &clockControl1);

        void Init() noexcept;
    };
}

#endif //STM32F4_USB_H
