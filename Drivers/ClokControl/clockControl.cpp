//
// Created by koteiko_vv on 02.12.2022.
//
#include "clockControl.hpp"
namespace drivers::clock
{

    ClockControl::ClockControl() {
        SetExternalClockGenerator_168MHz();
        //SetInternalClockGenerator_16MHz();
    }

    ClockControl::ClockControl(Frequency f)
    {
        drivers::flash::Flash flash;

        flash.SetLatency(5);
        while (flash.GetLatency() != 5)
        {}

        EnablePeripherals(SYSCF_MODULE);
        EnablePeripherals(PWR_MODULE);

        HSE_Enable();
        while (!HSE_IsReady())
        {}

        HSI_Disable();

        switch (f) {
            case FREQ_48000000:
                freqHCLK = FREQ_48000000;
                PLL_Config_Sys(96,4,0,4);

                while (PLL_IsReady())
                {}

                SetAHBPrescaler(AHB_DIVISOR_BY_2);
                SetAPB1Prescaler(APB_DIVISOR_BY_2);
                SetAPB2Prescaler(APB_OFF);
                SetSysClkSource(2);

                systemCoreClock = freqHCLK;
                freqAPB1 = freqHCLK / 2;
                freqAPB2 = freqHCLK / 2;

                break;

            case FREQ_50000000:
                freqHCLK = FREQ_50000000;
                PLL_Config_Sys(50,4,0,4);

                while (PLL_IsReady())
                {}

                SetAHBPrescaler(AHB_OFF);
                SetAPB1Prescaler(APB_DIVISOR_BY_2);
                SetAPB2Prescaler(APB_DIVISOR_BY_2);
                SetSysClkSource(2);

                systemCoreClock = freqHCLK;
                freqAPB1 = freqHCLK / 2;
                freqAPB2 = freqHCLK / 2;
                break;

            case FREQ_100000000:
                freqHCLK = FREQ_100000000;
                PLL_Config_Sys(100,4,0,4);

                while (PLL_IsReady())
                {}

                SetAHBPrescaler(AHB_OFF);
                SetAPB1Prescaler(APB_DIVISOR_BY_4);
                SetAPB2Prescaler(APB_DIVISOR_BY_2);
                SetSysClkSource(2);

                systemCoreClock = freqHCLK;
                freqAPB1 = freqHCLK / 4;
                freqAPB2 = freqHCLK / 2;
                break;

            case FREQ_168000000:
                freqHCLK = FREQ_168000000;
                SetExternalClockGenerator_168MHz();
                break;
        }
        InitTickSysTick(freqHCLK,1000);
    }

    std::uint32_t ClockControl::GetFreqSystemCoreClock() {
        return systemCoreClock;
    }

    std::uint32_t ClockControl::GetFreqHCLK() {
        return freqHCLK;
    }

    std::uint32_t ClockControl::GetFreqAPB1() {
        return freqAPB1;
    }

    std::uint32_t ClockControl::GetFreqAPB2() {
        return freqAPB2;
    }

    void ClockControl::SetCalibTrimming(std::uint32_t value) noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CR,0xF8);              // clear internal high-speed clock trimming
        libs::MWR::modifySetRegister(RegisterRCC::CR,(value << 3));
    }

    void ClockControl::SetAHBPrescaler(PrescalerAHB prescaler) noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CFGR,0xF0);            // AHB prescaler clear
        libs::MWR::modifySetRegister(RegisterRCC::CFGR,(prescaler << 4));
    }

    void ClockControl::SetAPB1Prescaler(PrescalerAPB prescaler) noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CFGR,(0x7 << 10));      // APB prescaler clear
        libs::MWR::modifySetRegister(RegisterRCC::CFGR,(prescaler << 10));
    }

    void ClockControl::SetAPB2Prescaler(PrescalerAPB prescaler) noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CFGR,(0x7 << 13));      // APB prescaler clear
        libs::MWR::modifySetRegister(RegisterRCC::CFGR,(prescaler << 13));
    }

    void ClockControl::SetSysClkSource(std::uint32_t value) noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CFGR,3);                // clear SWS SW
        libs::MWR::modifySetRegister(RegisterRCC::CFGR,value);
    }

    void ClockControl::InitTickSysTick(std::uint32_t HCLKFrequency, std::uint32_t ticks) noexcept
    {
        libs::MWR::write_register(RegisterSysTick::LOAD,static_cast<std::uint32_t>((HCLKFrequency / ticks) - 1)); // set reload register
        libs::MWR::write_register(RegisterSysTick::VAL,0);                                                        // load the SysTick Counter Value
        libs::MWR::write_register(RegisterSysTick::CTRL, static_cast<std::uint32_t>(5));                          // Enable the Systick Timer
    }

    void ClockControl::AHB1EnableClock(TYPE_ENABLE_CLOCK_AHB_1 typeEnableClock) noexcept
    {
        libs::MWR::modifySetRegister(RegisterRCC::AHB1ENR,(1 << typeEnableClock));
    }

    void ClockControl::AHB2EnableClock(TYPE_ENABLE_CLOCK_AHB_2 typeEnableClock) noexcept
    {
        libs::MWR::setBit(AHB2ENR, typeEnableClock);
    }

    void ClockControl::APB1EnableClock(TYPE_ENABLE_CLOCK_APB_1 typeEnableClock) noexcept
    {
        libs::MWR::modifySetRegister(RegisterRCC::APB1ENR,(1 << typeEnableClock));
    }

    void ClockControl::APB2EnableClock(TYPE_ENABLE_CLOCK_APB_2 typeEnableClock) noexcept
    {
        libs::MWR::modifySetRegister(RegisterRCC::APB2ENR,(1 << typeEnableClock));
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

    void ClockControl::EnablePeripherals(PERIPHERALS name) noexcept
    {
        switch (name)
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
            case PORT_D_MODULE:
            {
                AHB1EnableClock(PORT_D_AHB_1);   // Enable PORT  D
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

    void ClockControl::HSE_Enable() noexcept {
        libs::MWR::setBit(CR,HSEON);
    }

    void ClockControl::HSE_Disable() noexcept {
        libs::MWR::resetBit(CR, HSEON);
    }

    bool ClockControl::HSE_Status() noexcept {
        return libs::MWR::readBit<std::uint32_t>(CR, HSEON);
    }

    bool ClockControl::HSE_IsReady() noexcept {
        return (libs::MWR::read_register<std::uint32_t>(CR) & (1 << HSERDY));
    }

    bool ClockControl::HSI_Status() noexcept {
        return libs::MWR::readBit<std::uint32_t>(CR, HSION);
    }

    void ClockControl::PLL_Config_Sys(std::uint16_t  setPLLN, std::uint16_t setPLLM, std::uint16_t setPLLP, std::uint16_t setPLLQ) {

        libs::MWR::modifyResetRegister(PLLCFGR,0x0000FFFF);

        //libs::MWR::setBit(PLLCFGR, (1 << 22) | PLLM | (PLLN << 6) | (PLLQ << 24)); котейко
        libs::MWR::modifySetRegister(PLLCFGR, static_cast<std::uint32_t>((1 << PLLSRC_poz) | setPLLM | (setPLLN << PLLN_poz) | (setPLLQ << PLLQ_poz) | (setPLLP << PLLP_poz)));

        PLL_Enable();
    }

    void ClockControl::PLL_SetSource(std::uint8_t bit) {
        if(bit == 0)
            libs::MWR::resetBit(PLLCFGR, PLLSRC_poz);
        else
            libs::MWR::setBit(PLLCFGR, PLLSRC_poz);
    }

    std::uint32_t ClockControl::GetSysClkSourse() noexcept {
        return (libs::MWR::read_register<std::uint32_t>(CFGR) & 12);
    }

    void ClockControl::SetInternalClockGenerator_16MHz() noexcept {

            SetCalibTrimming(16);
            HSI_Enable();
            while (HSI_IsReady())
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

        EnablePeripherals(SYSCF_MODULE);
        EnablePeripherals(PWR_MODULE);

        HSE_Enable();
        while (!HSE_IsReady())
        {}

        PLL_Config_Sys(168,4,0,7);

        while (PLL_IsReady())
        {}

        SetAHBPrescaler(AHB_OFF);
        SetAPB1Prescaler(APB_DIVISOR_BY_4);
        SetAPB2Prescaler(APB_DIVISOR_BY_2);
        SetSysClkSource(2);

        freqHCLK = FREQ_168000000;
        systemCoreClock = freqHCLK;
        freqAPB1 = freqHCLK / 4;
        freqAPB2 = freqHCLK / 2;

        while ((GetSysClkSourse() != 8))
        {}

        HSI_Disable();

        InitTickSysTick(168000000,1000);            // 1ms
    }

    void ClockControl::HSI_Enable() noexcept {
        libs::MWR::setBit(CR, HSION);
    }

    void ClockControl::HSI_Disable() noexcept
    {
        libs::MWR::resetBit(CR, HSION);
    }

    bool ClockControl::HSI_IsReady() noexcept {
        return libs::MWR::readBit<std::uint32_t>(CR, HSIRDY);
    }

    void ClockControl::PLL_Enable() noexcept {
        libs::MWR::setBit(CR, PLLON);
    }

    void ClockControl::PLL_Disable() noexcept {
        libs::MWR::resetBit(CR, PLLON);
    }

    bool ClockControl::PLL_IsReady() noexcept {
        return (libs::MWR::readBit<std::uint32_t>(CR, PLLRDY));
    }
}