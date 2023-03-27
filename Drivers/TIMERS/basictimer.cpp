
#include "basictimer.h"

namespace drivers::timers
{
    BasicTimers::BasicTimers(clock::ClockControl &curClock, BASIC_TIMERS timer) : clockControl(curClock), baseAddress(timer) {
        switch (timer) {
            case TIM6: curClock.EnablePeripherals(drivers::clock::TIM6_MODULE); break;
            case TIM7: curClock.EnablePeripherals(drivers::clock::TIM7_MODULE); break;
        }
    }

    void BasicTimers::EnableCounter() noexcept {
        libs::MWR::setBit(baseAddress + CR1, CEN);
    }

    void BasicTimers::DisableCounter() noexcept {
        libs::MWR::resetBit(baseAddress + CR1, CEN);
    }

    bool BasicTimers::IsEnabledCounter() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, CEN);
    }

    void BasicTimers::EnableUpdateEvent() noexcept {
        libs::MWR::resetBit(baseAddress + CR1, UDIS);
    }

    void BasicTimers::DisableUpdateEvent() noexcept {
        libs::MWR::setBit(baseAddress + CR1, UDIS);
    }

    bool BasicTimers::IsEnabledUpdateEvent() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, UDIS);
    }

    void BasicTimers::SetUpdateSource(UPDATE_SOURCE us) noexcept {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 1 << URS);
        libs::MWR::modifySetRegister(baseAddress + CR1, us << URS);
    }

    UPDATE_SOURCE BasicTimers::GetUpdateSource() noexcept {
        return (UPDATE_SOURCE)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, URS);
    }

    void BasicTimers::SetOnePulseMode(ONE_PULSE_MODE opm) noexcept {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 1 << OPM);
        libs::MWR::modifySetRegister(baseAddress + CR1, opm << OPM);
    }

    ONE_PULSE_MODE BasicTimers::GetOnePulseMode() noexcept {
        return (ONE_PULSE_MODE)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, OPM);
    }

    void BasicTimers::EnableARRPreload() noexcept {
        libs::MWR::setBit(baseAddress + CR1, ARPE);
    }

    void BasicTimers::DisableARRPreload() noexcept {
        libs::MWR::resetBit(baseAddress + CR1, ARPE);
    }

    bool BasicTimers::IsEnabledARRPreload() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, ARPE);
    }

    void BasicTimers::EnableInterrupt() noexcept {
        libs::MWR::setBit(baseAddress + DIER, UIE);
    }

    void BasicTimers::DisableInterrupt() noexcept {
        libs::MWR::resetBit(baseAddress + DIER, UIE);
    }

    bool BasicTimers::IsEnabledInterrupt() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + DIER, UIE);
    }

    void BasicTimers::EnableDmaRequest() noexcept {
        libs::MWR::setBit(baseAddress + DIER, UDE);
    }

    void BasicTimers::DisableDmaRequest() noexcept {
        libs::MWR::resetBit(baseAddress + DIER, UDE);
    }

    bool BasicTimers::IsEnabledDmaRequest() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + DIER, UDE);
    }

    bool BasicTimers::IsUpdateInterruptFlag() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, 0);
    }

    void BasicTimers::ClearUpdateInterruptFlag() noexcept {
        libs::MWR::resetBit(baseAddress + SR, 0);
    }

    void BasicTimers::GenerateEventUpdate() noexcept {
        libs::MWR::setBit(baseAddress + EGR, 0);
    }

    void BasicTimers::SetCounter(std::uint16_t val) noexcept {
        libs::MWR::write_register(baseAddress + CNT, val);
    }

    std::uint16_t BasicTimers::GetCounter() noexcept {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + CNT);
    }

    void BasicTimers::SetPrescaler(std::uint16_t val) noexcept {
        libs::MWR::write_register(baseAddress + PSC, val);
    }

    std::uint16_t BasicTimers::GetPrescaler() noexcept {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + PSC);
    }

    void BasicTimers::SetAutoReload(std::uint16_t val) noexcept {
        libs::MWR::write_register(baseAddress + ARR, val);
    }

    std::uint16_t BasicTimers::GetAutoReload() noexcept {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + ARR);
    }
}