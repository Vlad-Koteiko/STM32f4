//
// Created by koteiko_vv on 09.02.2023.
//
#ifndef EXTI_H
#define EXTI_H

#include "MWR.hpp"

namespace drivers::exti
{
    class EXIT_RTSR
    {
        static constexpr std::uint32_t baseAddress = 0x40013C00;

        enum RegisterEXIT_RTSR : std::uintptr_t
        {
            IMR = baseAddress,    // EXTI Interrupt mask register,            Address offset: 0x00
            EMR = baseAddress +
                  0x04,    // EXTI Event mask register,                Address offset: 0x04
            RTSR = baseAddress +
                   0x08,    // EXTI Rising trigger selection register,  Address offset: 0x08
            FTSR = baseAddress +
                   0x0C,    // EXTI Falling trigger selection register, Address offset: 0x0C
            SWIER = baseAddress +
                    0x10,    // EXTI Software interrupt event register,  Address offset: 0x10
            PR = baseAddress +
                 0x14,    // EXTI Pending register,                   Address offset: 0x14
        };

    public:
        enum LINE_NUMBER : std::uint8_t
        {
            LINE_0  = 0,
            LINE_1  = 1,
            LINE_2  = 2,
            LINE_3  = 3,
            LINE_4  = 4,
            LINE_5  = 5,
            LINE_6  = 6,
            LINE_7  = 7,
            LINE_8  = 8,
            LINE_9  = 9,
            LINE_10 = 10,
            LINE_11 = 11,
            LINE_12 = 12,
            LINE_13 = 13,
            LINE_14 = 14,
            LINE_15 = 15,
            LINE_16 = 16,
            LINE_17 = 17,
            LINE_18 = 18,
            LINE_19 = 19,
            LINE_20 = 20,
            LINE_21 = 21,
            LINE_22 = 22
        };

        void SetLineTrigger(LINE_NUMBER lineNumber) noexcept;

        void EnableIT(LINE_NUMBER line) noexcept;
        void DisableIT(LINE_NUMBER line) noexcept;
        bool IsEnabledIT(LINE_NUMBER line) noexcept;
        void EnableEvent(LINE_NUMBER line) noexcept;
        void DisableEvent(LINE_NUMBER line) noexcept;
        bool IsEnabledEvent(LINE_NUMBER line) noexcept;
        void EnbleRisingTrig(LINE_NUMBER line) noexcept;
        void DisableRisingTrig(LINE_NUMBER line) noexcept;
        bool IsEnabledRisingTrig(LINE_NUMBER line) noexcept;
        void EnableFallingTrig(LINE_NUMBER line) noexcept;
        void DisableFallingTrig(LINE_NUMBER line) noexcept;
        bool IsEnbledFallingTrig(LINE_NUMBER line) noexcept;
        void GenerateSWI(LINE_NUMBER line) noexcept;
        bool IsActiveFlag(LINE_NUMBER line) noexcept;
        void ClearFlag(LINE_NUMBER line) noexcept;
    };
}    // namespace drivers::exti

#endif    // UART_EXTI_H
