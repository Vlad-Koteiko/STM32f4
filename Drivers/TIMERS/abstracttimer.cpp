
#include "abstracttimer.hpp"

namespace drivers::timers::abstracttimer
{
    AbstractTimer::AbstractTimer(std::uintptr_t base_address) : baseAddress(base_address)
    {}

    void AbstractTimer::enable() noexcept
    {
        libs::MWR::setBit(baseAddress + CR1, CEN);
    }

    void AbstractTimer::disable() noexcept
    {
        libs::MWR::resetBit(baseAddress + CR1, CEN);
    }

    bool AbstractTimer::isEnabledCounter() noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, CEN);
    }

    void AbstractTimer::updateDisable(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CR1, UDIS);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CR1, UDIS);
    }

    bool AbstractTimer::isUpdateDisable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, UDIS);
    }

    void AbstractTimer::updateRequestSource(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CR1, URS);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CR1, URS);
    }

    bool AbstractTimer::isUpdateRequestSource()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, URS);
    }

    void AbstractTimer::onePulseMode(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CR1, OPM);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CR1, OPM);
    }

    bool AbstractTimer::isOnePulseMode()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, OPM);
    }

    void AbstractTimer::autoReloadPreloadEnable(STATUS s)
    {
        libs::MWR::resetBit(baseAddress + CR1, ARPE);
        if(s == ENABLE)
            libs::MWR::setBit(baseAddress + CR1, ARPE);
    }

    bool AbstractTimer::isAutoReloadPreloadEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, ARPE);
    }

    void AbstractTimer::masterModeSelection(MASTER_MODE_SELECTION m)
    {
        std::uint32_t mask = 0x7 << MMS;
        libs::MWR::modifyResetRegister(baseAddress + CR2, mask);
        libs::MWR::modifySetRegister(baseAddress + CR2, m << MMS);
    }

    MASTER_MODE_SELECTION AbstractTimer::getMasterModeSelection()
    {
        std::uint32_t mask = 0x7;
        return static_cast<MASTER_MODE_SELECTION>(
            libs::MWR::read_register<std::int32_t>((baseAddress + CR2) >> MMS) & mask);
    }

    void AbstractTimer::enableIT(SOURCE_INTERRUPR si)
    {
        libs::MWR::setBit(baseAddress + DIER, si);
    }

    void AbstractTimer::disabelIT(SOURCE_INTERRUPR si)
    {
        libs::MWR::resetBit(baseAddress + DIER, si);
    }

    bool AbstractTimer::isEnabledIT(SOURCE_INTERRUPR si)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + DIER, si);
    }

    void AbstractTimer::enableRequestDMA(REQUESTS_DMA rd)
    {
        libs::MWR::setBit(baseAddress + DIER, rd);
    }

    void AbstractTimer::disableRequestDMA(REQUESTS_DMA rd)
    {
        libs::MWR::resetBit(baseAddress + DIER, rd);
    }

    bool AbstractTimer::isEnabledRequestDMA(REQUESTS_DMA rd)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + DIER, rd);
    }

    bool AbstractTimer::getFlagStatusRegister(SR_poz flag)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, flag);
    }

    void AbstractTimer::clearFlagStatusRegister(SR_poz flag)
    {
        libs::MWR::resetBit(baseAddress + SR, flag);
    }

    void AbstractTimer::enableEventGeneration(EGR_poz flag)
    {
        libs::MWR::setBit(baseAddress + EGR, flag);
    }

    void AbstractTimer::diasbelEventGeneration(EGR_poz flag)
    {
        libs::MWR::resetBit(baseAddress + EGR, flag);
    }

    bool AbstractTimer::isEnabledEventGeneration(EGR_poz flag)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + EGR, flag);
    }

    void AbstractTimer::setCounter(std::uint16_t val) noexcept
    {
        libs::MWR::write_register(baseAddress + CNT, val);
    }

    std::uint16_t AbstractTimer::getCounter() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + CNT);
    }

    void AbstractTimer::setPrescaler(std::uint16_t val) noexcept
    {
        libs::MWR::write_register(baseAddress + PSC, val);
    }

    std::uint16_t AbstractTimer::getPrescaler() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + PSC);
    }

    void AbstractTimer::setAutoReload(std::uint16_t val) noexcept
    {
        libs::MWR::write_register(baseAddress + ARR, val);
    }

    std::uint16_t AbstractTimer::getAutoReload() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + ARR);
    }

}    // namespace drivers::timers::abstracttimer