//
// Created by koteiko_vv on 02.02.2023.
//
#ifndef UART_NVIC_H
#define UART_NVIC_H

#include "MWR.hpp"

namespace drivers::nvic {

    class NVIC {

        static constexpr std::uint32_t baseAddress = 0xE000E100;

        enum RegisterNVIC : std::uintptr_t {
            ISER = baseAddress,         // Interrupt Set Enable Register,
            ICER = baseAddress + 0x080, // Interrupt Clear Enable Register,
            ISPR = baseAddress + 0x100, // Interrupt Set Pending Register,
            ICPR = baseAddress + 0x180, // Interrupt Clear Pending Register
            IABR = baseAddress + 0x200, // Interrupt Active bit Register
            IP   = baseAddress + 0x300, // Interrupt Priority Register (8Bit wide)
            STIR = baseAddress + 0x18   // Software Trigger Interrupt Register
        };

    public:

        enum DEVICE_ID : std::uint8_t
        {
          EXTI_0    = 6,
          UASRT_2   = 38,
        };

        void NVIC_Enable(DEVICE_ID deviceId) noexcept;

        void NVIC_SetPriority(DEVICE_ID deviceId, std::uint32_t priority) noexcept;

    };
}

#endif //UART_NVIC_H
