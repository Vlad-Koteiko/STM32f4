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

    /// @brief Status
    enum STATUS : std::uint8_t
    {
        DISABLE = 0,    ///< Disable
        ENABLE  = 1     ///< Enable
    };

    /// @brief Clock division
    enum CLOCK_DIVISION : std::uint8_t
    {
        DIV_NOT = 0,    ///< Not division
        DIV_2   = 1,    ///< Division 2
        DIV_4   = 2     ///< Division 4
    };

    /// @brief Capture/Compare
    enum CAP_COM : std::uint8_t
    {
        OUTPUT = 0,    ///< As output
        INPUT  = 1     ///< As input
    };

    enum MODE : std::uint8_t
    {
        MODE_0,
        MODE_1
    };

    class GeneralPurposeTimer
    {
    private:
        const drivers::clock::ClockControl& clockControl;    ///< Link to class ClockControl
        std::uintptr_t                      baseAddress;     ///< Base addres GPIO

        enum Registers : std::uintptr_t
        {
            CR1   = 0x00,    ///< Control register 1
            SMCR  = 0x08,    ///< Slave mode control register
            DIER  = 0x0C,    ///< Interrupt enable register
            SR    = 0x10,    ///< Status register
            EGR   = 0x14,    ///< Event generation register
            CCMR1 = 0x18,    ///< Capture/compare mode register 1
            CCER  = 0x20,    ///< Capture/compare enable register
            CNT   = 0x24,    ///< Counter
            PSC   = 0x28,    ///< Prescaler
            ARR   = 0x2C,    ///< Auto-reload register
            CCR1  = 0x34,    ///< Capture/compare register 1
            CCR2  = 0x38,    ///< Capture/compare register 2
            OR    = 0x50     ///< Option register
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

        /// @brief Update disable
        /// @param s ENABLE/DISABLE
        void update(STATUS s);

        /// @brief Is update disable
        /// @return true - Enable
        bool isUpdate();

        /// @brief Update request source
        /// @param s ENABLE/DISABLE
        void updateRequestSource(STATUS s);

        /// @brief Is update request source
        /// @return true - Enable
        bool isUpdateRequestSource();

        /// @brief One pulse mode
        /// @param s ENABLE/DISABLE
        void onePulseMode(STATUS s);

        /// @brief Is one pulse mode
        /// @return true - Enable
        bool isOnePulseMode();

        /// @brief Auto-reload preload enable
        /// @param s ENABLE/DISABLE
        void autoReloadPreloadEnable(STATUS s);

        /// @brief Is auto-reload preload enable
        /// @return true - Enable
        bool isAutoReloadPreloadEnable();

        /// @brief Set clock division
        /// @param div enum CLAOCK_DIVISION
        void clockDivision(CLOCK_DIVISION div);

        /// @brief Get clock division
        /// @return CLOCK_DIVISION
        CLOCK_DIVISION getClockDivision();

        /// @brief Enable interrupt
        /// @param f Source interrupt
        void enableIT(DIER_poz f);

        /// @brief Disable interrupt
        /// @param f Source interrupt
        void disabelIT(DIER_poz f);

        /// @brief Is enabled interrupt
        /// @param f Source interrupt
        bool isEnabledIT(DIER_poz f);

        /// @brief Get status flag
        /// @param f Flag
        /// @return value
        bool getStatus(SR_poz f);

        /// @brief Clear status flag
        /// @param f Flag
        void clearStatus(SR_poz f);

        /// @brief Event generation
        /// @param f flag
        void eventGeneration(EGR_poz f);

        /// @brief Capture/Compare 1 selection
        /// @param cc
        void captureCompare1Selection(CAP_COM cc);

        /// @brief Output/Compare 1 fast enable
        /// @param s
        void outputCompare1FastEnable(STATUS s);

        /// @brief Is output/compare 1 fast enable
        /// @return true - Enable
        bool isOutputCompare1FastEnable();

        /// @brief Output/compare 1 preload enable
        /// @param s
        void outputCompare1PreloadEnable(STATUS s);

        /// @brief Is output/compare 1 preload enable
        /// @return true - Enable
        bool isOutputCompare1PreloadEnable();

        /// @brief Output compare 1 mode
        /// @param m
        void outputCompare1Mode(MODE m);
    };
}    // namespace drivers::timers::generalpurposetimers
#endif    // __GENERALPURPOSETIMER_H__