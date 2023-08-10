#include "basictimer.hpp"

namespace drivers::timers::basictimers
{
    BasicTimer::BasicTimer(const clock::ClockControl &curClock, BASIC_TIMERS timer) :
        AbstractTimer(timer), clockControl(curClock)
    {
        switch(timer)
        {
            case TIM6:
                curClock.EnablePeripherals(drivers::clock::constants::TIM6_MODULE);
                break;
            case TIM7:
                curClock.EnablePeripherals(drivers::clock::constants::TIM7_MODULE);
                break;
            default:
                break;
        }
    }

    BasicTimer::BasicTimer(const clock::ClockControl &curClock,
                           BASIC_TIMERS               timer,
                           std::chrono::milliseconds  milliseconds,
                           bool                       _enableInterrupt) :
        AbstractTimer(timer), clockControl(curClock)
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

        updateDisable(ENABLE);
        autoReloadPreloadEnable(ENABLE);
        setPrescaler(prescaller);
        setAutoReload(preload);
        if(_enableInterrupt == true)
            enableIT(SI_UIE);
        enable();
    }

}    // namespace drivers::timers::basictimers