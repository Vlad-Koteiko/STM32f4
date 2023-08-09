
#include "generalpurposetimer.hpp"
namespace drivers::timers::generalpurposetimers
{
    GeneralPurposeTimer::GeneralPurposeTimer(drivers::clock::ClockControl &curClock,
                                             GENERAL_PURPOSE_TIMERS        timer) :
        clockControl(curClock), baseAddress(timer)
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

    void GeneralPurposeTimer::enable()
    {
        libs::MWR::setBit(baseAddress + CR1, CEN);
    }

    void GeneralPurposeTimer::disable()
    {
        libs::MWR::resetBit(baseAddress + CR1, CEN);
    }

    void GeneralPurposeTimer::update(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CR1, UDIS);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CR1, UDIS);
    }

    bool GeneralPurposeTimer::isUpdate()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, UDIS);
    }

    void GeneralPurposeTimer::updateRequestSource(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CR1, URS);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CR1, URS);
    }

    bool GeneralPurposeTimer::isUpdateRequestSource()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, URS);
    }

    void GeneralPurposeTimer::onePulseMode(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CR1, OPM);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CR1, OPM);
    }

    bool GeneralPurposeTimer::isOnePulseMode()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, OPM);
    }

    void GeneralPurposeTimer::autoReloadPreloadEnable(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CR1, ARPE);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CR1, ARPE);
    }

    bool GeneralPurposeTimer::isAutoReloadPreloadEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, ARPE);
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

    void GeneralPurposeTimer::enableIT(DIER_poz f)
    {
        libs::MWR::setBit(baseAddress + DIER, f);
    }

    void GeneralPurposeTimer::disabelIT(DIER_poz f)
    {
        libs::MWR::resetBit(baseAddress + DIER, f);
    }

    bool GeneralPurposeTimer::isEnabledIT(DIER_poz f)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + DIER, f);
    }

    bool GeneralPurposeTimer::getStatus(SR_poz f)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, f);
    }

    void GeneralPurposeTimer::clearStatus(SR_poz f)
    {
        libs::MWR::resetBit(baseAddress + SR, f);
    }

    void GeneralPurposeTimer::eventGeneration(EGR_poz f)
    {
        libs::MWR::setBit(baseAddress + EGR, f);
    }

    void GeneralPurposeTimer::captureCompare1Selection(CAP_COM cc)
    {
        libs::MWR::modifyResetRegister(baseAddress + CCMR1, 0x3);
        libs::MWR::modifySetRegister(baseAddress + CCMR1, cc);
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

}    // namespace drivers::timers::generalpurposetimers