#ifndef RNG_H
#define RNG_H

#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::rng
{
    enum CR_poz : std::uint8_t
    {
        RNGREN = 2, // Random number generator enable
        IE    = 3  // Interrupt enable
    };

    enum SR_poz : std::uint8_t
    {
        DRDY = 0, // Data ready
        CECS = 1, // Clock error current status
        SECS = 2, // Seed error current status
        CEIS = 5, // Clock error interrupt status
        SEIS = 6, // Seed error interrupt status
    };

    class RNG
    {
        drivers::clock::ClockControl &clockControl;
        static constexpr std::uintptr_t baseAddress = 0x50060800;

        enum RegisterRNG : std::uintptr_t
        {
            CR = baseAddress,      // RNG control register
            SR = baseAddress + 4,  // RNG status register
            DR = baseAddress + 8,  // RNG data register
        };

        RNG(drivers::clock::ClockControl &clockControl);
        void Enable() noexcept;
        void Disable() noexcept;
        bool IsEnabled() noexcept;
        bool IsActiveFlag_DRDY() noexcept;
        bool IsActiveFlag_CECS() noexcept;
        bool IsActiveFlag_SECS() noexcept;
        bool IsActiveFlag_CEIS() noexcept;
        bool IsActiveFlag_SEIS() noexcept;
        void ClearFlag_CEIS() noexcept;
        void ClearFlag_SEIS() noexcept;
        void EnableIT() noexcept;
        void DisableIT() noexcept;
        bool IsEnabledIT() noexcept;
        std::uint32_t ReadRandData32() noexcept;
    };
}

#endif //RGN_H