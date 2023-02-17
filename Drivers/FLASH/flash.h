//
// Created by koteiko_vv on 17.02.2023.
//

#ifndef STM32F4_FLASH_H
#define STM32F4_FLASH_H

#include "MWR.hpp"

namespace drivers::flash
{
    class FLASH {

        static constexpr std::uint32_t baseAddress = 0x40023c00 ;

        enum RegisterFLASH : std::uintptr_t {
            ACR       = baseAddress,        // FLASH access control register,   Address offset: 0x00
            KEYR      = baseAddress + 0x04, // FLASH key register,              Address offset: 0x04
            OPTKEYR   = baseAddress + 0x08, // FLASH option key register,       Address offset: 0x08
            SR        = baseAddress + 0x0C, // FLASH status register,           Address offset: 0x0C
            CR        = baseAddress + 0x10, // FLASH control register,          Address offset: 0x10
            OPTCR     = baseAddress + 0x14, // FLASH option control register ,  Address offset: 0x14
            OPTCR1    = baseAddress + 0x18, // FLASH option control register 1, Address offset: 0x18
        };

    public:

        void SetLatency(std::uint8_t latency) noexcept;

        std::uint8_t GetLatency() noexcept;

    };
}

#endif //STM32F4_FLASH_H
