//
// Created by koteiko_vv on 02.12.2022.
//
#include "clockControl.hpp"
namespace drivers
{

    ClockControl::ClockControl() {
        SetExternalClockGenerator_168MHz();
//        SetInternalClockGenerator_16MHz();
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

    void ClockControl::AHB2EnableClock(TYPE_ENABLE_CLOCK_AHB_2 typeEnableClock) noexcept
    {
        libs::MWR::enableNumberBit(AHB2ENR, typeEnableClock);
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
            case USART_1_MODULE:
            {
                APB2EnableClock(USART_1_APB_2);  // Enable UARRT 1
                break;
            }
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
            case PORT_H_MODULE:
            {
                AHB1EnableClock(PORT_H_AHB_1);   // Enable PORT H
                break;
            }
            case SYSCF_MODULE:
            {
                APB2EnableClock(SYSCF);          // Enable SYSCF
                break;
            }
            case PWR_MODULE:
            {
                APB1EnableClock(PWR_APB_1);      // Enable PWR
                break;
            }
            case USB_FS_MODULE:
            {
                AHB2EnableClock(USB_OTG_FS_AHB_2);   // Enable USB
                break;
            }
        }
    }

    void ClockControl::ESE_Enable() noexcept {
        libs::MWR::setBit(CR,1 << 16);
    }

    bool ClockControl::HSE_IsReady() noexcept {
        return (libs::MWR::read_register<std::uint32_t>(CR) & (1 << 17));
    }

    void ClockControl::PLL_Config_Sys(std::uint8_t PLLN, std::uint16_t PLLM, std::uint8_t PLLQ) noexcept {
        libs::MWR::clearBit(PLLCFGR,0x0000FFFF);
        libs::MWR::setBit(PLLCFGR, (1 << 22) | PLLM | (PLLN << 6) | (PLLQ << 24));

        libs::MWR::setBit(CR, 1 << 24);
    }

    bool ClockControl::PLL_IsReady() noexcept {
        return (libs::MWR::read_register<std::uint32_t>(CR) & (1 << 25));
    }

    std::uint32_t ClockControl::GetSysClkSourse() noexcept {
        return (libs::MWR::read_register<std::uint32_t>(CFGR) & 12);
    }

    void ClockControl::SetInternalClockGenerator_16MHz() noexcept {

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

    void ClockControl::SetExternalClockGenerator_168MHz() noexcept {

        drivers::flash::Flash flash;

        flash.SetLatency(5);
        while (flash.GetLatency() != 5)
        {}

        module_enable(SYSCF_MODULE);
        module_enable(PWR_MODULE);

        ESE_Enable();
        while (!HSE_IsReady())
        {}

        PLL_Config_Sys(168,4,7);

        while (PLL_IsReady())
        {}

        SetAHBPrescaler(AHB_OFF);
        SetAPB1Prescaler(APB_DIVISOR_BY_4);
        SetAPB2Prescaler(APB_DIVISOR_BY_2);
        SetSysClkSource(2);

        while ((GetSysClkSourse() != 8))
        {}

        InitTickSysTick(168000000,1000);            // 1ms
    }


}