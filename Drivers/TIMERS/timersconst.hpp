#ifndef __TIMERSCONST_H__
#define __TIMERSCONST_H__

#include <cstdint>

namespace drivers::timers
{
    /// @brief Status
    enum STATUS : std::uint8_t
    {
        DISABLE = 0,    ///< Disable
        ENABLE  = 1     ///< Enable
    };

    /// @brief Enum basic timers
    enum BASIC_TIMERS : std::uintptr_t
    {
        TIM6 = 0x4000'1000,    ///< Timer 6
        TIM7 = 0x4000'1400     ///< Timer 7
    };

    /// @brief Base address timers
    enum GENERAL_PURPOSE_TIMERS : std::uintptr_t
    {

        TIM10 = 0x4001'4400,    ///< Timer 10
        TIM11 = 0x4001'4800,    ///< Timer 11
        TIM13 = 0x4000'1C00,    ///< Timer 13
        TIM14 = 0x4000'2000     ///< Timer 14
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

    enum CR2_poz : std::uint8_t
    {
        MMS = 4,    ///< Master mode selection [4:6]
    };

    /// @brief DMA/Interrupt enable register
    enum DIER_poz : std::uint8_t
    {
        UIE   = 0,    ///< Update interrupt enable
        CC1IE = 1,    ///< Update interrupt enable
        UDE   = 8,    ///< Update DMA request enable
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
    enum CCMR1_poz : std::uint8_t
    {
        CC1S  = 0,     ///< Capture/compare 1 selection [0:1]
        OC1FE = 2,     ///< Output compare 1 fast enable
        OC1PE = 3,     ///< Output compare 1 preload enable
        OC1M  = 4,     ///< Output compare 1 mode [4:6]

        IC1PSC = 2,    ///< Input capture 1 prescaler [2:3]
        IC1F   = 4     ///< Input capture 1 filter [4:7]
    };

    /// @brief Capture/compare enable register
    enum CCER : std::uint8_t
    {
        CC1E  = 0,    ///< Capture/compare 1 output enable
        CC1P  = 1,    ///< Capture/compare 1 output Polarity
        CC1NP = 3     ///< Capture/compare 1 complemntary output Polarity
    };

    /// @brief Clock division
    enum CLOCK_DIVISION : std::uint8_t
    {
        DIV_NOT = 0,    ///< Not division
        DIV_2   = 1,    ///< Division 2
        DIV_4   = 2     ///< Division 4
    };

    /// @brief Capture/Compare 1 selection
    enum V_CC1S : std::uint8_t
    {
        CC1_OUTPUT = 0,    ///< CC1 channel is configured as output.
        CC1_INPUT  = 1     ///< CC1 channel is configured as input, IC1 is mapped on TI1
    };

    enum MASTER_MODE_SELECTION : std::uint8_t
    {
        MMS_RESET  = 0,    ///< Reset
        MMS_ENABLE = 1,    ///< Enable
        MMS_UPDATE = 2     ///< Update
    };

    enum SOURCE_INTERRUPR : std::uint8_t
    {
        SI_UIE   = 0,    ///< Update interrupt enable
        SI_CC1IE = 1     ///< CC1 interrupt enable
    };

    enum REQUESTS_DMA : std::uint8_t
    {
        RD_UDE = 0,    ///< Update DMA request enable
    };

    enum OUTPUT_COMPARE_MODE : std::uint8_t
    {
        OCM_FROZEN = 0,    ///< The comparison between the output compare register TIMx_CCR1 and the
                           ///< counter TIMx_CNT has no effect on the outputs
        OCM_1 = 1,    ///< Set channel 1 to active level on match. OC1REF signal is forced high when
                      ///< the counter TIMx_CNT matches the capture/compare register 1
        OCM_2 = 2,    ///< Set channel 1 to inactive level on match. OC1REF signal is forced low
                      ///< when the counter TIMx_CNT matches the capture/compare register 1
        OCM_TOGGLE = 3,    ///< OC1REF toggles when TIMx_CNT=TIMx_CCR1
        OCM_FIL    = 4,    ///< Force inactive level - OC1REF is forced low
        OCM_FAL    = 5,    ///< Force active level - OC1REF is forced high
        OCM_PWM1 = 6,    ///< PWM mode 1 - Channel 1 is active as long as TIMx_CNT < TIMx_CCR1 else
                         ///< inactive
        OCM_PWM2 =
            7    ///< PWM mode 2 - Channel 1 is inactive as long as TIMx_CNT < TIMx_CCR1 else active
    };

    enum INPUT_CAPTURE_PRESCALER : std::uint8_t
    {
        ICP_NO_PRESCALER = 0,    ///< No prescaler, capture is done each time an edge is detected on
                                 ///< the capture input
        ICP_CAP_2_EV = 1,        ///< Capture is done once every 2 events
        ICP_CAP_4_EV = 2,        ///< Capture is done once every 4 events
        ICP_CAP_8_EV = 4         ///< Capture is done once every 8 events
    };

    enum INPUT_CAPTURE_FILTER : std::uint8_t
    {
        ICF_NO  = 0,     ///< No filter, sampling is done at f(DTS)
        ICF_F1  = 1,     ///< f(SAMPLING)=f(CK_INT), N=2
        ICF_F2  = 2,     ///< f(SAMPLING)=f(CK_INT), N=4
        ICF_F3  = 3,     ///< f(SAMPLING)=f(CK_INT), N=8
        ICF_F4  = 4,     ///< f(SAMPLING)=f(DTS)/2, N=6
        ICF_F5  = 5,     ///< f(SAMPLING)=f(DTS)/2, N=8
        ICF_F6  = 6,     ///< f(SAMPLING)=f(DTS)/4, N=6
        ICF_F7  = 7,     ///< f(SAMPLING)=f(DTS)/4, N=8
        ICF_F8  = 8,     ///< f(SAMPLING)=f(DTS)/8, N=6
        ICF_F9  = 9,     ///< f(SAMPLING)=f(DTS)/8, N=8
        ICF_F10 = 10,    ///< f(SAMPLING)=f(DTS)/16, N=5
        ICF_F11 = 11,    ///< f(SAMPLING)=f(DTS)/16, N=6
        ICF_F12 = 12,    ///< f(SAMPLING)=f(DTS)/16, N=8
        ICF_F13 = 13,    ///< f(SAMPLING)=f(DTS)/32, N=5
        ICF_F14 = 14,    ///< f(SAMPLING)=f(DTS)/32, N=6
        ICF_F15 = 15,    ///< f(SAMPLING)=f(DTS)/32, N=8
    };

}    // namespace drivers::timers

#endif    // __TIMERSCONST_H__