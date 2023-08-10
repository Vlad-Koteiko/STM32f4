/*! \file basictimer.hpp
    \brief class BasicTimer

    Header file with a description of the basic timer class
*/

#ifndef BASICTIMERS_H
#define BASICTIMERS_H

#include <chrono>

#include "MWR.hpp"
#include "abstracttimer.hpp"
#include "clockControl.hpp"

/// @brief Namespace for work with BasicTimer
namespace drivers::timers::basictimers
{
    /// @brief Class for working with BasicTimer
    class BasicTimer : public abstracttimer::AbstractTimer
    {
        const drivers::clock::ClockControl &clockControl;    ///< Reference Clock Controll
        std::uintptr_t                      baseAddress;     ///< Base address

    protected:
        /// @brief Registers BasicTimer
        enum Registers : std::uintptr_t
        {
            CR1   = 0x00,    ///< Control register 1
            CR2   = 0x04,    ///< Control register 2
            DIER  = 0x0C,    ///< DMA/Interrupt enable register
            SR    = 0x10,    ///< Status register
            EGR   = 0x14,    ///< Event generation register
            CCMR1 = 0x18,    ///< Capture/compare mode register 1
            CNT   = 0x24,    ///< counter
            PSC   = 0x28,    ///< Prescaller
            ARR   = 0x2C,    ///< auto-reload register
            CCR1  = 0x34,    ///< Capture/compare register 1
            CCR2  = 0x38,    ///< Capture/compare register 2
            OR    = 0x50     ///< Option register
        };

    public:
        BasicTimer() = delete;

        /// @brief Constructor BasicTimer
        /// @param curClock Pointer ClockControl
        /// @param timer Base address timer
        BasicTimer(const drivers::clock::ClockControl &curClock, BASIC_TIMERS timer);

        /// @brief Constructor BasicTimer
        /// @param curClock Pointer ClockControl
        /// @param timer Base address timer
        /// @param milliseconds Period
        /// @param enableInterrupt Activate interrupt
        BasicTimer(const drivers::clock::ClockControl &curClock,
                   BASIC_TIMERS                        timer,
                   std::chrono::milliseconds           milliseconds,
                   bool                                enableInterrupt);
    };
}    // namespace drivers::timers::basictimers

#endif    // BASICTIMERS_H