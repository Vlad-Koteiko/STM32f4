//
// Created by koteiko_vv on 02.12.2022.
//
#include "clockControl.hpp"
namespace drivers
{

    ClockControl::ClockControl() {

            SetCalibTrimming(16);
            Enable();
            while (IsReady())
            {
            }
            SetAHBPrescaler(AHB_OFF);
            SetAPB1Prescaler(APB_OFF);
            SetAPB2Prescaler(APB_OFF);
            SetSysClkSource(0);
            InitTickSysTick(16000000,1000);            // 1ms
    }

    void ClockControl::SetCalibTrimming(std::uint32_t value) noexcept
    {
        libs::MWR::clearBit(RegisterRCC::CR,0xF8);              // clear internal high-speed clock trimming
        libs::MWR::setBit(RegisterRCC::CR,(value << 3));
    }

    void ClockControl::Enable() noexcept
    {
        libs::MWR::setBit(RegisterRCC::CR,1);                   // internal high-speed clock enable
    }

    [[nodiscard]] bool ClockControl::IsReady() noexcept
    {
        std::uint32_t flag = (1 << 1);                          // internal high-speed clock ready flag
        return (libs::MWR::read_register<std::uint32_t>(RegisterRCC::CR) == flag);
    }

    void ClockControl::SetAHBPrescaler(PrescalerAHB prescaler) noexcept
    {
        libs::MWR::clearBit(RegisterRCC::CFGR,0xF0);            // AHB prescaler clear
        libs::MWR::setBit(RegisterRCC::CFGR,(prescaler << 4));
    }

    void ClockControl::SetAPB1Prescaler(PrescalerAPB prescaler) noexcept
    {
        libs::MWR::clearBit(RegisterRCC::CFGR,(0x7 << 10));      // APB prescaler clear
        libs::MWR::setBit(RegisterRCC::CFGR,(prescaler << 10));
    }

    void ClockControl::SetAPB2Prescaler(PrescalerAPB prescaler) noexcept
    {
        libs::MWR::clearBit(RegisterRCC::CFGR,(0x7 << 13));      // APB prescaler clear
        libs::MWR::setBit(RegisterRCC::CFGR,(prescaler << 13));
    }

    void ClockControl::SetSysClkSource(std::uint32_t value) noexcept
    {
        libs::MWR::clearBit(RegisterRCC::CFGR,3);                // clear SWS SW
        libs::MWR::setBit(RegisterRCC::CFGR,value);
    }

    void ClockControl::InitTickSysTick(std::uint32_t HCLKFrequency, std::uint32_t ticks) noexcept
    {
        libs::MWR::write_register(RegisterSysTick::LOAD,static_cast<std::uint32_t>((HCLKFrequency / ticks) - 1)); // set reload register
        libs::MWR::write_register(RegisterSysTick::VAL,0);                                                        // load the SysTick Counter Value
        libs::MWR::write_register(RegisterSysTick::CTRL, static_cast<std::uint32_t>(5));                          // Enable the Systick Timer
    }

    void ClockControl::AHB1EnableClock(TYPE_ENABLE_CLOCK_AHB_1 typeEnableClock) noexcept
    {
        libs::MWR::setBit(RegisterRCC::AHB1ENR,(1 << typeEnableClock));
    }

    void ClockControl::APB1EnableClock(TYPE_ENABLE_CLOCK_APB_1 typeEnableClock) noexcept
    {
        libs::MWR::setBit(RegisterRCC::APB1ENR,(1 << typeEnableClock));
    }

    void ClockControl::APB2EnableClock(TYPE_ENABLE_CLOCK_APB_2 typeEnableClock) noexcept
    {
        libs::MWR::setBit(RegisterRCC::APB2ENR,(1 << typeEnableClock));
    }

    void ClockControl::mDelay(std::uint32_t Delay)
    {
        if(Delay < 0xFFFFFFFFU)
        {
            Delay++;
        }

        while (Delay)
        {
            if((libs::MWR::read_register<std::uint32_t>(CTRL) & (1 << 16)) != 0)
            {
                Delay--;
            }
        }
    }

    void ClockControl::module_enable(MODULE module) noexcept
    {
        switch (module)
        {
            case USART_2_MODULE:
            {
                APB1EnableClock(USART2_APB_1);   // Enable UARRT 2
                break;
            }
            case PORT_A_MODULE:
            {
                AHB1EnableClock(PORT_A_AHB_1);   // Enable PORT  A
                break;
            }
            case SYSCF_MODULE:
            {
                APB2EnableClock(SYSCF);          // Enable SYSCF
                break;
            }
        }
    }
}