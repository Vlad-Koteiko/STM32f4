//
// Created by koteiko_vv on 09.02.2023.
//
#include "exti.h"

namespace drivers::exti
{

    void EXIT_RTSR::SetLineTrigger(drivers::exti::EXIT_RTSR::LINE_NUMBER lineNumber) noexcept
    {
        libs::MWR::modifyResetRegister(EMR, 1 << lineNumber);
        libs::MWR::modifySetRegister(IMR, 1 << lineNumber);

        libs::MWR::modifyResetRegister(FTSR, 1 << lineNumber);
        libs::MWR::modifySetRegister(RTSR, 1 << lineNumber);
    }

    void EXIT_RTSR::EnableIT(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::setBit(IMR, line);
    }

    void EXIT_RTSR::DisableIT(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::resetBit(IMR, line);
    }

    bool EXIT_RTSR::IsEnabledIT(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(IMR, line);
    }

    void EXIT_RTSR::EnableEvent(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::setBit(EMR, line);
    }

    void EXIT_RTSR::DisableEvent(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::resetBit(EMR, line);
    }

    bool EXIT_RTSR::IsEnabledEvent(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(EMR, line);
    }

    void EXIT_RTSR::EnbleRisingTrig(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::setBit(RTSR, line);
    }

    void EXIT_RTSR::DisableRisingTrig(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::resetBit(RTSR, line);
    }

    bool EXIT_RTSR::IsEnabledRisingTrig(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(RTSR, line);
    }

    void EXIT_RTSR::EnableFallingTrig(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::setBit(FTSR, line);
    }

    void EXIT_RTSR::DisableFallingTrig(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::resetBit(FTSR, line);
    }

    bool EXIT_RTSR::IsEnbledFallingTrig(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(FTSR, line);
    }

    void EXIT_RTSR::GenerateSWI(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::setBit(SWIER, line);
    }

    bool EXIT_RTSR::IsActiveFlag(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(PR, line);
    }

    void EXIT_RTSR::ClearFlag(EXIT_RTSR::LINE_NUMBER line) noexcept
    {
        libs::MWR::setBit(PR, line);
    }
}    // namespace drivers::exti
