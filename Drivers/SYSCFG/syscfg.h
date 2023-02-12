//
// Created by koteiko_vv on 09.02.2023.
//
#ifndef SYSCFG_H
#define SYSCFG_H

#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::syscfg {

    class SYSCFG {
        static constexpr std::uint32_t baseAddress = 0x40013800;
        ClockControl &clockControl;

        enum RegisterNVIC : std::uintptr_t {
            MEMRMP   = baseAddress,        // SYSCFG memory remap register,                      Address offset: 0x00
            PMC      = baseAddress + 0x04, // SYSCFG peripheral mode configuration register,     Address offset: 0x04
            EXTICR   = baseAddress + 0x08, // SYSCFG external interrupt configuration registers, Address offset: 0x08
            EXTICR_2 = baseAddress + 0x0C,
            CMPCR    = baseAddress + 0x20  // SYSCFG Compensation cell control register,         Address offset: 0x20
        };

    public:
        enum EXIT_PORT : std::uint16_t {
            PORT_A = 0,
            PORT_B = 0b0001,
            PORT_C = 0b0010,
            PORT_D = 0b0011,
            PORT_E = 0b0100,
            PORT_F = 0b0101,
            PORT_G = 0b0110,
            PORT_H = 0b0111,
            PORT_I = 0b1000,
        };

        enum EXIT_NUMBER : std::uint8_t {
            EXTI_0 = 0,
            EXTI_1 = 1,
            EXTI_2 = 2,
            EXTI_3 = 3,
            EXTI_4 = 4,
            EXTI_5 = 5,
            EXTI_6 = 6,
            EXTI_7 = 7,
            EXTI_8 = 8,
            EXTI_9 = 9,
            EXTI_10 = 10,
            EXTI_11 = 11,
            EXTI_12 = 12,
            EXTI_13 = 13,
            EXTI_14 = 14,
            EXTI_15 = 15,
            EXTI_16 = 16
        };

        SYSCFG(ClockControl &clockControl1);

        void SetSourceEXTI(EXIT_PORT exitPort, EXIT_NUMBER exitNumber) noexcept;
    };
}

#endif //UART_SYSCFG_H
