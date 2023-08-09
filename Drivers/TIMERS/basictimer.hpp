/*! \file basictimer.hpp
    \brief class BasicTimer

    Header file with a description of the basic timer class
*/

#ifndef BASICTIMERS_H
#define BASICTIMERS_H

#include <chrono>

#include "MWR.hpp"
#include "clockControl.hpp"

/// @brief Namespace for work with BasicTimer
namespace drivers::timers::basictimers
{
    /// @brief Enum basic timers
    enum BASIC_TIMERS : std::uintptr_t
    {
        TIM6 = 0x40001000,    ///< Timer 6
        TIM7 = 0x40001400     ///< Timer 7
    };

    /// @brief Control register 1
    enum CR1_poz : std::uint8_t
    {
        CEN  = 0,    ///< Counter enable
        UDIS = 1,    ///< Update disable
        URS  = 2,    ///< Update request source
        OPM  = 3,    ///< One-pulse mode
        ARPE = 7,    ///< Auto-reload preload enable
    };

    /// @brief DMA/Interrupt enable register
    enum DIER_poz : std::uint8_t
    {
        UIE = 0,    ///< Update interrupt enable
        UDE = 8,    ///< Update DMA request enable
    };

    /// @brief Enum update source
    enum UPDATE_SOURCE : std::uint8_t
    {
        REGULAR,    ///< Regular
        COUNTER     ///< Counter
    };

    /// @brief One pulse mode
    enum ONE_PULSE_MODE : std::uint8_t
    {
        REPETITIVE,    ///< Repetitive
        SINGLE         ///< Single
    };

    /// @brief Mode
    enum MODE : std::uint8_t
    {
        RESET,     ///< Reset
        ENABLE,    ///< Enable
        UPDATE     ///< Update
    };

    /// @brief Class for working with BasicTimer
    class BasicTimer
    {
        drivers::clock::ClockControl &clockControl;    ///< Reference Clock Controll
        std::uintptr_t                baseAddress;     ///< Base address

        /// @brief Registers BasicTimer
        enum Registers : std::uintptr_t
        {
            CR1  = 0x00,    ///< Control register 1
            CR2  = 0x04,    ///< Control register 2
            DIER = 0x0C,    ///< DMA/Interrupt enable register
            SR   = 0x10,    ///< Status register
            EGR  = 0x14,    ///< Event generation register
            CNT  = 0x24,    ///< counter
            PSC  = 0x28,    ///< Prescaller
            ARR  = 0x2C     ///< auto-reload register
        };

    public:
        /// @brief Constructor BasicTimer
        /// @param curClock Pointer ClockControl
        /// @param timer Base address timer
        BasicTimer(drivers::clock::ClockControl &curClock, BASIC_TIMERS timer);

        /// @brief Constructor BasicTimer
        /// @param curClock Pointer ClockControl
        /// @param timer Base address timer
        /// @param milliseconds Period
        /// @param enableInterrupt Activate interrupt
        BasicTimer(drivers::clock::ClockControl &curClock,
                   BASIC_TIMERS                  timer,
                   std::chrono::milliseconds     milliseconds,
                   bool                          enableInterrupt);

        /// @brief Enable counter
        void enable() noexcept;

        /// @brief Disable counter
        void disable() noexcept;

        /// @brief Is enable counter
        /// @return true - enable
        bool isEnabledCounter() noexcept;

        /// @brief Enbable update event
        void enableUpdateEvent() noexcept;

        /// @brief Disble update event
        void disableUpdateEvent() noexcept;

        /// @brief Is enable update event
        /// @return true - enable
        bool isEnabledUpdateEvent() noexcept;

        /// @brief Set update source
        /// @param us Enum UPDATE_SOURCE
        void setUpdateSource(UPDATE_SOURCE us) noexcept;

        /// @brief Get update source
        /// @return Enum UPDATE_SOURCE
        UPDATE_SOURCE getUpdateSource() noexcept;

        /// @brief Set one pulse mode
        /// @param opm Enum ONE_PULSE_MODE
        void setOnePulseMode(ONE_PULSE_MODE opm) noexcept;

        /// @brief Get one pulse mode
        /// @return Enum ONE_PULSE_MODE
        ONE_PULSE_MODE getOnePulseMode() noexcept;

        /// @brief Enable ARR preload
        void enableARRPreload() noexcept;

        /// @brief Disable ARR preload
        void disableARRPreload() noexcept;

        /// @brief Is enabled ARR preload
        /// @return true - enable
        bool isEnabledARRPreload() noexcept;

        /// @brief Enable interrupt
        void enableInterrupt() noexcept;

        /// @brief Disable interrupt
        void disableInterrupt() noexcept;

        /// @brief Is enabled interrupt
        /// @return true - enable
        bool isEnabledInterrupt() noexcept;

        /// @brief Enble dma request
        void enableDmaRequest() noexcept;

        /// @brief Disable dma request
        void disableDmaRequest() noexcept;

        /// @brief Is enabled dma request
        /// @return true - enable
        bool isEnabledDmaRequest() noexcept;

        /// @brief Is update interrupt flag
        /// @return true - update
        bool isUpdateInterruptFlag() noexcept;

        /// @brief Clear update interrupt flag
        void clearUpdateInterruptFlag() noexcept;

        /// @brief Generate event update
        void generateEventUpdate() noexcept;

        /// @brief Set counter
        /// @param val value
        void setCounter(std::uint16_t val) noexcept;

        /// @brief Get counter
        /// @return value
        std::uint16_t getCounter() noexcept;

        /// @brief Set prescaler
        /// @param val value
        void setPrescaler(std::uint16_t val) noexcept;

        /// @brief Get prescaler
        /// @return
        std::uint16_t getPrescaler() noexcept;

        /// @brief Set autoreload
        /// @param val value
        void setAutoReload(std::uint16_t val) noexcept;

        /// @brief Get autoreload
        /// @return value
        std::uint16_t getAutoReload() noexcept;
    };
}    // namespace drivers::timers::basictimers

#endif    // BASICTIMERS_H