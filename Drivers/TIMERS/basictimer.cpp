#include "basictimer.hpp"

namespace drivers::timers::basictimers
{
    BasicTimer::BasicTimer(clock::ClockControl &curClock, BASIC_TIMERS timer) :
        clockControl(curClock), baseAddress(timer)
    {
        switch(timer)
        {
            case TIM6:
                curClock.EnablePeripherals(drivers::clock::constants::TIM6_MODULE);
                break;
            case TIM7:
                curClock.EnablePeripherals(drivers::clock::constants::TIM7_MODULE);
                break;
        }
    }

    BasicTimer::BasicTimer(clock::ClockControl      &curClock,
                           BASIC_TIMERS              timer,
                           std::chrono::milliseconds milliseconds,
                           bool                      _enableInterrupt) :
        clockControl(curClock), baseAddress(timer)
    {
        switch(timer)
        {
            case TIM6:
                curClock.EnablePeripherals(drivers::clock::constants::TIM6_MODULE);
                break;
            case TIM7:
                curClock.EnablePeripherals(drivers::clock::constants::TIM7_MODULE);
                break;
        }

        std::uint32_t freqTimer_Hz = 1000 / milliseconds.count();
        // std::uint32_t freqTimer_Hz = 1;
        std::uint32_t freqBusTimer = clockControl.GetFreqAPB1() * 2;
        // std::uint32_t freqBusTimer = 84000000;

        std::uint16_t prescaller = 0, preload = 0;

        for(std::uint32_t i = 1; i < 65535; i++)
        {
            for(std::uint32_t j = 1; j < 65535; j++)
            {
                std::uint32_t rez  = freqBusTimer / i / j;
                std::uint32_t ost1 = freqBusTimer % i;
                std::uint32_t ost2 = freqBusTimer % j;
                if((ost1 == 0) && (ost2 == 0) && (rez == freqTimer_Hz))
                {
                    prescaller = i - 1;
                    preload    = j - 1;
                    break;
                }
            }
            if((prescaller != 0) && (preload != 0))
            {
                break;
            }
        }

        enableUpdateEvent();
        enableARRPreload();
        setPrescaler(prescaller);
        setAutoReload(preload);
        if(_enableInterrupt == true)
            enableInterrupt();
        enable();
    }

    void BasicTimer::enable() noexcept
    {
        libs::MWR::setBit(baseAddress + CR1, CEN);
    }

    void BasicTimer::disable() noexcept
    {
        libs::MWR::resetBit(baseAddress + CR1, CEN);
    }

    bool BasicTimer::isEnabledCounter() noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, CEN);
    }

    void BasicTimer::enableUpdateEvent() noexcept
    {
        libs::MWR::resetBit(baseAddress + CR1, UDIS);
    }

    void BasicTimer::disableUpdateEvent() noexcept
    {
        libs::MWR::setBit(baseAddress + CR1, UDIS);
    }

    bool BasicTimer::isEnabledUpdateEvent() noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, UDIS);
    }

    void BasicTimer::setUpdateSource(UPDATE_SOURCE us) noexcept
    {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 1 << URS);
        libs::MWR::modifySetRegister(baseAddress + CR1, us << URS);
    }

    UPDATE_SOURCE BasicTimer::getUpdateSource() noexcept
    {
        return (UPDATE_SOURCE)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, URS);
    }

    void BasicTimer::setOnePulseMode(ONE_PULSE_MODE opm) noexcept
    {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 1 << OPM);
        libs::MWR::modifySetRegister(baseAddress + CR1, opm << OPM);
    }

    ONE_PULSE_MODE BasicTimer::getOnePulseMode() noexcept
    {
        return (ONE_PULSE_MODE)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, OPM);
    }

    void BasicTimer::enableARRPreload() noexcept
    {
        libs::MWR::setBit(baseAddress + CR1, ARPE);
    }

    void BasicTimer::disableARRPreload() noexcept
    {
        libs::MWR::resetBit(baseAddress + CR1, ARPE);
    }

    bool BasicTimer::isEnabledARRPreload() noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, ARPE);
    }

    void BasicTimer::enableInterrupt() noexcept
    {
        libs::MWR::setBit(baseAddress + DIER, UIE);
    }

    void BasicTimer::disableInterrupt() noexcept
    {
        libs::MWR::resetBit(baseAddress + DIER, UIE);
    }

    bool BasicTimer::isEnabledInterrupt() noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + DIER, UIE);
    }

    void BasicTimer::enableDmaRequest() noexcept
    {
        libs::MWR::setBit(baseAddress + DIER, UDE);
    }

    void BasicTimer::disableDmaRequest() noexcept
    {
        libs::MWR::resetBit(baseAddress + DIER, UDE);
    }

    bool BasicTimer::isEnabledDmaRequest() noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + DIER, UDE);
    }

    bool BasicTimer::isUpdateInterruptFlag() noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, 0);
    }

    void BasicTimer::clearUpdateInterruptFlag() noexcept
    {
        libs::MWR::resetBit(baseAddress + SR, 0);
    }

    void BasicTimer::generateEventUpdate() noexcept
    {
        libs::MWR::setBit(baseAddress + EGR, 0);
    }

    void BasicTimer::setCounter(std::uint16_t val) noexcept
    {
        libs::MWR::write_register(baseAddress + CNT, val);
    }

    std::uint16_t BasicTimer::getCounter() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + CNT);
    }

    void BasicTimer::setPrescaler(std::uint16_t val) noexcept
    {
        libs::MWR::write_register(baseAddress + PSC, val);
    }

    std::uint16_t BasicTimer::getPrescaler() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + PSC);
    }

    void BasicTimer::setAutoReload(std::uint16_t val) noexcept
    {
        libs::MWR::write_register(baseAddress + ARR, val);
    }

    std::uint16_t BasicTimer::getAutoReload() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + ARR);
    }
}    // namespace drivers::timers::basictimers