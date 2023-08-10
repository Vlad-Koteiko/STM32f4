
#include "generalpurposetimer.hpp"
namespace drivers::timers::generalpurposetimers
{
    GeneralPurposeTimer::GeneralPurposeTimer(drivers::clock::ClockControl &curClock,
                                             GENERAL_PURPOSE_TIMERS        timer) :
        abstracttimer::AbstractTimer(timer), clockControl(curClock), baseAddress(timer)
    {
        switch(timer)
        {
            case TIM10:
                curClock.EnablePeripherals(drivers::clock::constants::TIM10_MODULE);
                break;
            case TIM11:
                curClock.EnablePeripherals(drivers::clock::constants::TIM11_MODULE);
                break;
            case TIM13:
                curClock.EnablePeripherals(drivers::clock::constants::TIM13_MODULE);
                break;
            case TIM14:
                curClock.EnablePeripherals(drivers::clock::constants::TIM14_MODULE);
                break;
        }
    }

    void GeneralPurposeTimer::clockDivision(CLOCK_DIVISION div)
    {
        libs::MWR::modifyResetRegister(baseAddress + CR1, (std::uint32_t)0x3 << CKD);
        libs::MWR::modifySetRegister(baseAddress + CR1, (std::uint32_t)div << CKD);
    }

    CLOCK_DIVISION GeneralPurposeTimer::getClockDivision()
    {
        return (CLOCK_DIVISION)((libs::MWR::read_register<std::uint32_t>(baseAddress + CR1) &
                                 0x00000300) >>
                                CKD);
    }

    void GeneralPurposeTimer::captureCompare1Selection(V_CC1S cc)
    {
        libs::MWR::modifyResetRegister(baseAddress + CCMR1, 0x3);
        libs::MWR::modifySetRegister(baseAddress + CCMR1, cc);
    }

    V_CC1S GeneralPurposeTimer::getCaptureCompare1Selection()
    {
        std::uint32_t mask = 0x3;
        return static_cast<V_CC1S>(libs::MWR::read_register<std::uint32_t>(baseAddress + CCMR1) &
                                   mask);
    }

    void GeneralPurposeTimer::outputCompare1FastEnable(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CCMR1, OC1FE);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CCMR1, OC1FE);
    }

    bool GeneralPurposeTimer::isOutputCompare1FastEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CCMR1, OC1FE);
    }

    void GeneralPurposeTimer::outputCompare1PreloadEnable(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CCMR1, OC1PE);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CCMR1, OC1PE);
    }

    bool GeneralPurposeTimer::isOutputCompare1PreloadEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CCMR1, OC1PE);
    }

    void GeneralPurposeTimer::outputCompare1Mode(OUTPUT_COMPARE_MODE m)
    {
        std::uint32_t mask = 0x7 << OC1M;
        libs::MWR::modifyResetRegister(baseAddress + CCMR1, mask);
        libs::MWR::modifySetRegister(baseAddress + CCMR1, m << OC1M);
    }

    OUTPUT_COMPARE_MODE GeneralPurposeTimer::getOutputCompare1Mode()
    {
        std::uint32_t mask = 0x7 << OC1M;
        return static_cast<OUTPUT_COMPARE_MODE>(
            (libs::MWR::read_register<std::uint32_t>(baseAddress + CCMR1) & mask) >> OC1M);
    }

    void GeneralPurposeTimer::inputCapture1Prescaler(INPUT_CAPTURE_PRESCALER icp)
    {
        std::uint32_t mask = 0x3 << IC1PSC;
        libs::MWR::modifyResetRegister(baseAddress + CCMR1, mask);
        libs::MWR::modifySetRegister(baseAddress + CCMR1, icp << IC1PSC);
    }

    INPUT_CAPTURE_PRESCALER GeneralPurposeTimer::getInputCapture1Prescaler()
    {
        std::uint32_t mask = 0x3 << IC1PSC;
        return static_cast<INPUT_CAPTURE_PRESCALER>(
            libs::MWR::read_register<std::uint32_t>((baseAddress + CCMR1) & mask) >> IC1PSC);
    }

    void GeneralPurposeTimer::captureCompare1OutputEnable(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CCER, CC1E);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CCER, CC1E);
    }

    bool GeneralPurposeTimer::getCaptureComapre1OutputEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CCER, CC1E);
    }

    void GeneralPurposeTimer::captureCompare1OutputPolarity(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CCER, CC1P);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CCER, CC1P);
    }

    bool GeneralPurposeTimer::getCaptureComapre1OutputPolarity()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CCER, CC1P);
    }

    void GeneralPurposeTimer::captureCompare1ComplementaryOutputPolarity(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CCER, CC1NP);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CCER, CC1NP);
    }

}    // namespace drivers::timers::generalpurposetimers