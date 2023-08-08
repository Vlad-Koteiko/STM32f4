/*! \file generalpurposetimer.hpp
    \brief class ___

    Header file with a description of the _ class
*/

#ifndef __GENERALPURPOSETIMER_H__
#define __GENERALPURPOSETIMER_H__

#include "MWR.hpp"
#include "clockControl.hpp"

/// @brief Namespace for work with TIM9-TIM14
namespace drivers::timers::generalpurposetimers
{
    /// @brief Base address timers
    enum GENERAL_PURPOSE_TIMERS : std::uintptr_t
    {

        TIM10 = 0x4001'4400,
        TIM11 = 0x4001'4800,
        TIM13 = 0x4000'1C00,
        TIM14 = 0x4000'2000
    };

    /// @brief Control register 1
    enum CR1_poz : std::uint8_t
    {
        CEN  = 0,    ///< Counter enable
        UDIS = 1,    ///< Update disable
        URS  = 2,    ///< Update request source
        OPM  = 3,    ///< One-pulse mode
        ARPE = 7,    ///< Auto-reload preload enable
        CKD  = 8     ///< Clock division [8:9]
    };

    /// @brief Interrupt enable register
    enum DIER_poz : std::uint8_t
    {
        UIE   = 0,    ///< Capture/Compare interrupt enable
        CC1IE = 1     ///< Update interrupt enable
    };

    /// @brief Status register
    enum SR_poz : std::uint8_t
    {
        UIF   = 0,    ///< Update interrupt flag
        CC1IF = 1,    ///< Capture/compare 1 interrupt flag
        CC1OF = 9     ///< Capture/Compare 1 overcapture flag
    };

    /// @brief Event generation register
    enum EGR_poz : std::uint8_t
    {
        UG   = 0,    ///< Update generation
        CC1G = 1     ///< Capture/compare 1 generation
    };

    /// @brief Capture/compare mode register 1
    enum CCMR1 : std::uint8_t
    {
        CC1S  = 0,     ///< Capture/compare 1 selection [0:1]
        OC1FE = 2,     ///< Output compare 1 fast enable
        OC1PE = 3,     ///< Output compare 1 preload enable
        OC1M  = 4,     ///< Output compare 1 mode [4:6]

        OC1PSC = 2,    ///< Input capture 1 prescaler [2:3]
        IC1F   = 4     ///< Input capture 1 filter [4:7]
    };

    /// @brief Capture/compare enable register
    enum CCER : std::uint8_t
    {
        CC1E  = 0,    ///< Capture/compare 1 output enable
        CC1P  = 1,    ///< Capture/compare 1 output Polarity
        CC1NP = 3     ///< Capture/compare 1 complemntary output Polarity
    };

    class GeneralPurposeTimer
    {
    private:
        const drivers::clock::ClockControl& clockControl;    ///< Link to class ClockControl
        std::uintptr_t                      baseAddress;     ///< Base addres GPIO

        enum Registers : std::uintptr_t
        {
            CR1     = 0x00,     ///< Control register 1
            SMCR    = 0x08,     ///< Slave mode control register
            DIER    = 0x0C,     ///< Interrupt enable register
            SR      = 0x10,     ///< Status register
            EGR     = 0x14,     ///< Event generation register
            CCMR1   = 0x18,     ///< Capture/compare mode register 1
            CCER    = 0x20,     ///< Capture/compare enable register
            CNT     = 0x24,     ///< Counter
            PSC     = 0x28,     ///< Prescaler
            ARR     = 0x2C,     ///< Auto-reload register
            CCR1    = 0x34,     ///< Capture/compare register 1
            CCR2    = 0x38,     ///< Capture/compare register 2
            OR      = 0x50      ///< Option register
        };

    public:
        /// @brief Constructor timer
        /// @param curClock Clock controll
        /// @param timer Base address timer
        GeneralPurposeTimer(drivers::clock::ClockControl& curClock, GENERAL_PURPOSE_TIMERS timer);

        /// @brief Counter enable
        void enable();

        /// @brief Counter disable
        void disable();
    };
}    // namespace drivers::timers::generalpurposetimers
#endif    // __GENERALPURPOSETIMER_H__