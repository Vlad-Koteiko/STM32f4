
#include "systick.h"

namespace drivers::systick
{
    SysTick::SysTick(std::uint32_t HCLKFrequency, std::uint32_t ticks)
    {
        libs::MWR::write_register(
            RegisterSysTick::LOAD,
            static_cast<std::uint32_t>((HCLKFrequency / ticks) - 1));    // set reload register
        libs::MWR::write_register(RegisterSysTick::VAL, 0);    // load the SysTick Counter Value
        libs::MWR::write_register(RegisterSysTick::CTRL, static_cast<std::uint32_t>(5));
    }

    void SysTick::SetValue(uint32_t val)
    {
        libs::MWR::write_register(VAL, val);
    }

    void SysTick::SetLoad(uint32_t val)
    {
        libs::MWR::write_register(LOAD, val);
    }

    void SysTick::SetClockSourceAHB()
    {
        libs::MWR::setBit(CTRL, CLKSOURCE);
    }

    void SysTick::SetClockSourceAHB8()
    {
        libs::MWR::resetBit(CTRL, CLKSOURCE);
    }

    void SysTick::EnableInterrupt()
    {
        libs::MWR::setBit(CTRL, TICKINT);
    }

    void SysTick::DisableInterrupt()
    {
        libs::MWR::resetBit(CTRL, TICKINT);
    }

    void SysTick::Start()
    {
        libs::MWR::setBit(CTRL, ENABLE);
    }

    void SysTick::Stop()
    {
        libs::MWR::resetBit(CTRL, ENABLE);
    }

    void SysTick::DelayMs(std::uint32_t val)
    {
        if(val < 0xFFFFFFFF)
            val++;

        while(val)
        {
            if((libs::MWR::read_register<std::uint32_t>(CTRL) & (1 << 16)) != 0)
            {
                val--;
            }
        }
    }
}    // namespace drivers::systick