#include "basictimer.hpp"

namespace drivers::timers
{
    BasicTimers::BasicTimers(clock::ClockControl &curClock, BASIC_TIMERS timer) : clockControl(curClock), baseAddress(timer) {
        switch (timer) {
            case TIM6: curClock.EnablePeripherals(drivers::clock::constants::TIM6_MODULE); break;
            case TIM7: curClock.EnablePeripherals(drivers::clock::constants::TIM7_MODULE); break;
        }
    }

    BasicTimers::BasicTimers(clock::ClockControl &curClock, BASIC_TIMERS timer, std::chrono::milliseconds milliseconds,
                             bool enableInterrupt) : clockControl(curClock) , baseAddress(timer) {

        switch (timer) {
            case TIM6: curClock.EnablePeripherals(drivers::clock::constants::TIM6_MODULE); break;
            case TIM7: curClock.EnablePeripherals(drivers::clock::constants::TIM7_MODULE); break;
        }

        std::uint32_t freqTimer_Hz = 1000 / milliseconds.count();
        //std::uint32_t freqTimer_Hz = 1;
        std::uint32_t freqBusTimer = clockControl.GetFreqAPB1() * 2;
        //std::uint32_t freqBusTimer = 84000000;


        std::uint16_t  prescaller = 0, preload = 0;

        for(std::uint32_t i = 1; i < 65535; i++)
        {
            for(std::uint32_t j = 1; j < 65535; j++)
            {

                std::uint32_t rez = freqBusTimer / i / j;
                std::uint32_t ost1 = freqBusTimer % i;
                std::uint32_t ost2 = freqBusTimer % j;
                if((ost1 == 0) && (ost2 == 0) && (rez == freqTimer_Hz))
                {
                    prescaller = i - 1;
                    preload = j - 1;
                    break;
                }
            }
            if((prescaller != 0) && (preload != 0))
            {
                break;
            }
        }

        EnableUpdateEvent();
        EnableARRPreload();
        SetPrescaler(prescaller);
        SetAutoReload(preload);
        if(enableInterrupt == true)
            EnableInterrupt();
        EnableCounter();
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