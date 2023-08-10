/*! \file generalpurposetimer.hpp
    \brief class ___

    Header file with a description of the _ class
*/

#ifndef __GENERALPURPOSETIMER_H__
#define __GENERALPURPOSETIMER_H__

#include "MWR.hpp"
#include "basictimer.hpp"
#include "clockControl.hpp"

/// @brief Namespace for work with TIM9-TIM14
namespace drivers::timers::generalpurposetimers
{
    // /// @brief Base address timers
    // enum GENERAL_PURPOSE_TIMERS : std::uintptr_t
    // {

    //     TIM10 = 0x4001'4400,
    //     TIM11 = 0x4001'4800,
    //     TIM13 = 0x4000'1C00,
    //     TIM14 = 0x4000'2000
    // };

    // /// @brief Control register 1
    // enum CR1_poz : std::uint8_t
    // {
    //     CEN  = 0,    ///< Counter enable
    //     UDIS = 1,    ///< Update disable
    //     URS  = 2,    ///< Update request source
    //     OPM  = 3,    ///< One-pulse mode
    //     ARPE = 7,    ///< Auto-reload preload enable
    //     CKD  = 8     ///< Clock division [8:9]
    // };

    // /// @brief Interrupt enable register
    // enum DIER_poz : std::uint8_t
    // {
    //     UIE   = 0,    ///< Capture/Compare interrupt enable
    //     CC1IE = 1     ///< Update interrupt enable
    // };

    // /// @brief Status register
    // enum SR_poz : std::uint8_t
    // {
    //     UIF   = 0,    ///< Update interrupt flag
    //     CC1IF = 1,    ///< Capture/compare 1 interrupt flag
    //     CC1OF = 9     ///< Capture/Compare 1 overcapture flag
    // };

    // /// @brief Event generation register
    // enum EGR_poz : std::uint8_t
    // {
    //     UG   = 0,    ///< Update generation
    //     CC1G = 1     ///< Capture/compare 1 generation
    // };

    // /// @brief Capture/compare mode register 1
    // enum CCMR1 : std::uint8_t
    // {
    //     CC1S  = 0,     ///< Capture/compare 1 selection [0:1]
    //     OC1FE = 2,     ///< Output compare 1 fast enable
    //     OC1PE = 3,     ///< Output compare 1 preload enable
    //     OC1M  = 4,     ///< Output compare 1 mode [4:6]

    //     OC1PSC = 2,    ///< Input capture 1 prescaler [2:3]
    //     IC1F   = 4     ///< Input capture 1 filter [4:7]
    // };

    // /// @brief Capture/compare enable register
    // enum CCER : std::uint8_t
    // {
    //     CC1E  = 0,    ///< Capture/compare 1 output enable
    //     CC1P  = 1,    ///< Capture/compare 1 output Polarity
    //     CC1NP = 3     ///< Capture/compare 1 complemntary output Polarity
    // };

    // /// @brief Status
    // enum STATUS : std::uint8_t
    // {
    //     DISABLE = 0,    ///< Disable
    //     ENABLE  = 1     ///< Enable
    // };

    // /// @brief Clock division
    // enum CLOCK_DIVISION : std::uint8_t
    // {
    //     DIV_NOT = 0,    ///< Not division
    //     DIV_2   = 1,    ///< Division 2
    //     DIV_4   = 2     ///< Division 4
    // };

    // /// @brief Capture/Compare
    // enum CAP_COM : std::uint8_t
    // {
    //     OUTPUT = 0,    ///< As output
    //     INPUT  = 1     ///< As input
    // };

    // enum MODE : std::uint8_t
    // {
    //     MODE_0,
    //     MODE_1
    // };

    class GeneralPurposeTimer : public abstracttimer::AbstractTimer
    {
    private:
        const drivers::clock::ClockControl& clockControl;    ///< Link to class ClockControl
        std::uintptr_t                      baseAddress;     ///< Base addres TIM

        void                  masterModeSelection(MASTER_MODE_SELECTION m);
        MASTER_MODE_SELECTION getMasterModeSelection();

    public:
        /// @brief Constructor timer
        /// @param curClock Clock controll
        /// @param timer Base address timer
        GeneralPurposeTimer(drivers::clock::ClockControl& curClock, GENERAL_PURPOSE_TIMERS timer);

        /// @brief Set clock division
        /// @param div enum CLAOCK_DIVISION
        void clockDivision(CLOCK_DIVISION div);

        /// @brief Get clock division
        /// @return CLOCK_DIVISION
        CLOCK_DIVISION getClockDivision();

        /// @brief Capture/Compare 1 selection
        /// @param cc
        void captureCompare1Selection(V_CC1S cc);

        /// @brief Get capture/compare 1 selection value
        /// @return Value
        V_CC1S getCaptureCompare1Selection();

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
        /// @param m Mode
        void outputCompare1Mode(OUTPUT_COMPARE_MODE m);

        /// @brief Get output compare 1 mode
        /// @return Mode
        OUTPUT_COMPARE_MODE getOutputCompare1Mode();

        /// @brief Input compare prescaler
        /// @param icp
        void inputCapture1Prescaler(INPUT_CAPTURE_PRESCALER icp);

        /// @brief Get input compare prescaler
        /// @return Prescaler
        INPUT_CAPTURE_PRESCALER getInputCapture1Prescaler();

        /// @brief Capture/Compare 1 output enable
        /// @param s STATUS
        void captureCompare1OutputEnable(STATUS s);

        /// @brief Get capture/compare 1 output enable
        /// @return true - Enable
        bool getCaptureComapre1OutputEnable();

        /// @brief Capture/Compare 1 output polarity
        /// @param s STATUS
        void captureCompare1OutputPolarity(STATUS s);

        /// @brief Get capture/compare 1 output enable
        /// @return true - Enable
        bool getCaptureComapre1OutputPolarity();

        /// @brief Capture/Compare 1 complementary output Polarity.
        /// @param s STATUS
        void captureCompare1ComplementaryOutputPolarity(STATUS s);

        /// @brief Get Capture/Compare 1 complementary output Polarity.
        /// @return true - Enable
        bool getCaptureCompare1ComplementaryOutputPolarity();
    };
}    // namespace drivers::timers::generalpurposetimers
#endif    // __GENERALPURPOSETIMER_H__