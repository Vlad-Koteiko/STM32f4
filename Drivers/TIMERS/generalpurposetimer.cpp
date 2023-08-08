
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

    
}    // namespace drivers::timers::generalpurposetimers