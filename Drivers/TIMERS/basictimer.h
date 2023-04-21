#ifndef BASICTIMERS_H
#define BASICTIMERS_H

#include <chrono>
#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::timers
{
    enum BASIC_TIMERS : std::uintptr_t
    {
        TIM6 = 0x40001000,
        TIM7 = 0x40001400
    };

    enum CR1_poz : std::uint8_t
    {
        CEN = 0, // Counter enable
        UDIS = 1, // Update disable
        URS = 2, // Update request source
        OPM = 3, // One-pulse mode
        ARPE = 7, // Auto-reload preload enable
    };

    enum DIER_poz : std::uint8_t
    {
        UIE = 0, // Update interrupt enable
        UDE = 8, // Update DMA request enable
    };

    enum UPDATE_SOURCE : std::uint8_t {
        REGULAR,
        COUNTER
    };

    enum ONE_PULSE_MODE : std::uint8_t {
        REPETITIVE,
        SINGLE
    };

    enum MODE : std::uint8_t {
        RESET,
        ENABLE,
        UPDATE
    };


    class BasicTimers
    {
        drivers::clock::ClockControl &clockControl;
        std::uintptr_t baseAddress;

        enum Registers : std::uintptr_t
        {
            CR1  = 0x00, // Control register 1
            CR2  = 0x04, // Control register 2
            DIER = 0x0C, // DMA/Interrupt enable register
            SR   = 0x10, // Status register
            EGR  = 0x14, // Event generation register
            CNT  = 0x24, // counter
            PSC  = 0x28, // Prescaller
            ARR  = 0x2C  // auto-reload register
        };

        constexpr std::uint16_t getPrescaller(std::uint32_t, std::uint32_t) const noexcept;
        constexpr std::uint16_t getPreload(std::uint32_t, std::uint32_t) const noexcept;

    public:

        BasicTimers(drivers::clock::ClockControl &curClock, BASIC_TIMERS timer);
        BasicTimers(drivers::clock::ClockControl &curClock, BASIC_TIMERS timer, std::chrono::milliseconds milliseconds, bool enableInterrupt);

        void EnableCounter() noexcept;
        void DisableCounter() noexcept;
        bool IsEnabledCounter() noexcept;
        void EnableUpdateEvent() noexcept;
        void DisableUpdateEvent() noexcept;
        bool IsEnabledUpdateEvent() noexcept;
        void SetUpdateSource(UPDATE_SOURCE us) noexcept;
        UPDATE_SOURCE GetUpdateSource() noexcept;
        void SetOnePulseMode(ONE_PULSE_MODE opm) noexcept;
        ONE_PULSE_MODE GetOnePulseMode() noexcept;
        void EnableARRPreload() noexcept;
        void DisableARRPreload() noexcept;
        bool IsEnabledARRPreload() noexcept;
        void EnableInterrupt() noexcept;
        void DisableInterrupt() noexcept;
        bool IsEnabledInterrupt() noexcept;
        void EnableDmaRequest() noexcept;
        void DisableDmaRequest() noexcept;
        bool IsEnabledDmaRequest() noexcept;
        bool IsUpdateInterruptFlag() noexcept;
        void ClearUpdateInterruptFlag() noexcept;
        void GenerateEventUpdate() noexcept;
        void SetCounter(std::uint16_t val) noexcept;
        std::uint16_t GetCounter() noexcept;
        void SetPrescaler(std::uint16_t val) noexcept;
        std::uint16_t GetPrescaler() noexcept;
        void SetAutoReload(std::uint16_t val) noexcept;
        std::uint16_t GetAutoReload() noexcept;
    };
}

#endif //BASICTIMERS_H