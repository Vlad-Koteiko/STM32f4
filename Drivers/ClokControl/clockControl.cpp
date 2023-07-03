//
// Created by koteiko_vv on 02.12.2022.
//
#include "clockControl.hpp"
namespace drivers::clock
{
    void ClockControl::setBaseConfig(std::array<std::uint8_t, 4> array,
                                     const drivers::clock::constants::PrescalerAHB &prescalerAhb, const drivers::clock::constants::PrescalerAPB &prescalerApb1,
                                     const drivers::clock::constants::PrescalerAPB &prescalerApb2) noexcept {

        PLL_Config_Sys(array[0],array[1],array[2],array[3]);
        while (PLL_IsReady())
        {}
        SetAHBPrescaler(prescalerAhb);
        SetAPB1Prescaler(prescalerApb1);
        SetAPB2Prescaler(prescalerApb2);
        SetSysClkSource(2);
    }

   void ClockControl::SetCalibTrimming(std::uint32_t value) const noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CR,0xF8);              // clear internal high-speed clock trimming
        libs::MWR::modifySetRegister(RegisterRCC::CR,(value << 3));
    }

    void ClockControl::SetAHBPrescaler(const constants::PrescalerAHB &prescaler) const noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CFGR,0xF0);            // AHB prescaler clear
        libs::MWR::modifySetRegister(RegisterRCC::CFGR,(prescaler << 4));
    }

    void ClockControl::SetAPB1Prescaler(const constants::PrescalerAPB &prescaler) const noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CFGR,(0x7 << 10));      // APB prescaler clear
        libs::MWR::modifySetRegister(RegisterRCC::CFGR,(prescaler << 10));
    }

    void ClockControl::SetAPB2Prescaler(const constants::PrescalerAPB &prescaler) const noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CFGR,(0x7 << 13));      // APB prescaler clear
        libs::MWR::modifySetRegister(RegisterRCC::CFGR,(prescaler << 13));
    }

    void ClockControl::SetSysClkSource(std::uint32_t value) const noexcept
    {
        libs::MWR::modifyResetRegister(RegisterRCC::CFGR,3);                // clear SWS SW
        libs::MWR::modifySetRegister(RegisterRCC::CFGR,value);
    }

    void ClockControl::InitTickSysTick(std::uint32_t HCLKFrequency, std::uint32_t ticks) const noexcept
    {
        libs::MWR::write_register(RegisterSysTick::LOAD,static_cast<std::uint32_t>((HCLKFrequency / ticks) - 1)); // set reload register
        libs::MWR::write_register(RegisterSysTick::VAL,0);                                                        // load the SysTick Counter Value
        libs::MWR::write_register(RegisterSysTick::CTRL, static_cast<std::uint32_t>(5));                          // Enable the Systick Timer
    }

    void ClockControl::AHB1EnableClock(const std::uint8_t& typeEnableClock) const noexcept
    {
        libs::MWR::modifySetRegister(RegisterRCC::AHB1ENR,(1 << typeEnableClock));
    }

    void ClockControl::AHB1DisableClock(const std::uint8_t& disableClock) const noexcept
    {
        libs::MWR::modifySetRegister(RegisterRCC::AHB1RSTR, (1 << disableClock));
    }

    void ClockControl::AHB2EnableClock(const std::uint8_t& typeEnableClock) const noexcept
    {
        libs::MWR::setBit(AHB2ENR, typeEnableClock);
    }

    void ClockControl::AHB2DisableClock(const std::uint8_t& disableClock) const noexcept
    {
        libs::MWR::setBit(AHB2RSTR, disableClock);
    }

    void ClockControl::APB1EnableClock(const std::uint8_t& typeEnableClock) const noexcept
    {
        libs::MWR::modifySetRegister(RegisterRCC::APB1ENR,(1 << typeEnableClock));
    }

    void ClockControl::APB1DisableClock(const std::uint8_t& disableClock) const noexcept
    {
        libs::MWR::modifySetRegister(RegisterRCC::APB1RSTR, (1 << disableClock));
    }

    void ClockControl::APB2EnableClock(const std::uint8_t& typeEnableClock) const noexcept
    {
        libs::MWR::modifySetRegister(RegisterRCC::APB2ENR,(1 << typeEnableClock));
    }

    void ClockControl::APB2DisableClock(const std::uint8_t& disableClock) const noexcept
    {
//        if((disableClock >= ADC1) && (disableClock <= ADC3))
//            disableClock = ADC1;

        libs::MWR::modifySetRegister(RegisterRCC::APB2RSTR , (1 << disableClock));
    }

    void ClockControl::mDelay(std::uint32_t Delay) const noexcept
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

    constexpr std::uint8_t ClockControl::getPeripheralsNumber(const std::uint8_t &number, const std::uint8_t &offset) const noexcept {
        return (number - offset);
    }

    void ClockControl::EnablePeripherals(const constants::PERIPHERALS &name) const noexcept
    {
        if(name < AHB_2)
        {
           AHB1EnableClock(name);

        }else if(name < AHP_1)
        {
           AHB2EnableClock(getPeripheralsNumber(name, AHB_2));

        }else if (name < AHP_2)
        {
            APB1EnableClock(getPeripheralsNumber(name, AHP_1));

        } else
        {
            APB2EnableClock(getPeripheralsNumber(name, AHP_2));
        }
    }

    void ClockControl::DisablePeripherals(const constants::PERIPHERALS &name) const noexcept
    {
        if(name < AHB_2)
        {
            AHB1DisableClock(name);

        }else if(name < AHP_1)
        {
            AHB2DisableClock(getPeripheralsNumber(name, AHB_2));

        }else if (name < AHP_2)
        {
            APB1DisableClock(getPeripheralsNumber(name, AHP_1));

        } else
        {
            APB2DisableClock(getPeripheralsNumber(name, AHP_2));
        }
    }

    void ClockControl::HSE_Enable() const noexcept {
        libs::MWR::setBit(CR,constants::HSEON);
    }

    void ClockControl::HSE_Disable() const noexcept {
        libs::MWR::resetBit(CR, constants::HSEON);
    }

    bool ClockControl::HSE_Status() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CR, constants::HSEON);
    }

    bool ClockControl::HSE_IsReady() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CR,constants::HSERDY);
    }

    bool ClockControl::HSI_Status() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CR, constants::HSION);
    }

    void ClockControl::PLL_Config_Sys(std::uint16_t  setPLLN, std::uint16_t setPLLM, std::uint16_t setPLLP, std::uint16_t setPLLQ) const noexcept {

        libs::MWR::modifyResetRegister(PLLCFGR,0x0000FFFF);

        //libs::MWR::setBit(PLLCFGR, (1 << 22) | PLLM | (PLLN << 6) | (PLLQ << 24)); котейко
        libs::MWR::modifySetRegister(PLLCFGR, static_cast<std::uint32_t>((1 << constants::PLLSRC) | setPLLM | (setPLLN << constants::PLLN) | (setPLLQ << constants::PLLQ) | (setPLLP << constants::PLLP)));

        PLL_Enable();
    }

    void ClockControl::PLL_SetSource(std::uint8_t bit) const noexcept {
        if(bit == 0)
            libs::MWR::resetBit(PLLCFGR, constants::PLLSRC);
        else
            libs::MWR::setBit(PLLCFGR, constants::PLLSRC);
    }

    std::uint32_t ClockControl::GetSysClkSourse() const noexcept {
        return (libs::MWR::read_register<std::uint32_t>(CFGR) & 12);
    }

    void ClockControl::HSI_Enable() const noexcept {
        libs::MWR::setBit(CR, constants::HSION);
    }

    void ClockControl::HSI_Disable() const noexcept
    {
        libs::MWR::resetBit(CR, constants::HSION);
    }

    bool ClockControl::HSI_IsReady() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CR, constants::HSIRDY);
    }

    void ClockControl::PLL_Enable() const noexcept {
        libs::MWR::setBit(CR, constants::PLLON);
    }

    void ClockControl::PLL_Disable() const noexcept {
        libs::MWR::resetBit(CR, constants::PLLON);
    }

    bool ClockControl::PLL_IsReady() const noexcept {
        return (libs::MWR::readBit<std::uint32_t>(CR, constants::PLLRDY));
    }

    void ClockControl::LSE_Enable() const noexcept {
        libs::MWR::setBit(BDCR, constants::LSEON);
    }

    void ClockControl::LSE_Disable() const noexcept {
        libs::MWR::resetBit(BDCR, constants::LSEON);
    }

    bool ClockControl::LSE_IsReady() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(BDCR, constants::LSERDY);
    }

    void ClockControl::LSE_SetBypass() const noexcept {
        libs::MWR::setBit(BDCR, constants::LSEBYP);
    }

    void ClockControl::LSE_ResetBypass() const noexcept {
        libs::MWR::resetBit(BDCR, constants::LSEBYP);
    }

    bool ClockControl::LSE_GetBypass() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(BDCR, constants::LSEBYP);
    }

    void ClockControl::SetSourceClock(constants::RTC_Source s) const noexcept {
        libs::MWR::modifyResetRegister(BDCR, 0x3 << constants::RTCSEL);
        libs::MWR::modifySetRegister( BDCR, s << constants::RTCSEL);
    }

    void ClockControl::RTC_Enable() const noexcept {
        libs::MWR::setBit(BDCR, constants::RTCEN);
    }

    void ClockControl::RTC_Disable() const noexcept {
        libs::MWR::resetBit(BDCR, constants::RTCEN);
    }

    bool ClockControl::RTC_GetStatus() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(BDCR, constants::RTCEN);
    }

    void ClockControl::BackupDomainReset() const noexcept {
        libs::MWR::setBit(BDCR, constants::BDRST);
    }

    void ClockControl::BackupDomainNoreset() const noexcept {
        libs::MWR::resetBit(BDCR, constants::BDRST);
    }

    bool ClockControl::GetBackupDomainStatus() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(BDCR, constants::BDRST);
    }

    void ClockControl::LSI_Enable() const noexcept {
        libs::MWR::setBit(CSR, constants::LSION);
    }

    void ClockControl::LSI_Disable() const noexcept {
        libs::MWR::resetBit(CSR, constants::LSION);
    }

    bool ClockControl::LSI_Status() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CSR, constants::LSION);
    }

    bool ClockControl::LSI_IsReady() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CSR, constants::LSIRDY);
    }

    void ClockControl::RemoveResetFlag() const noexcept {
        libs::MWR::setBit(CSR, constants::RMVF);
    }

    bool ClockControl::GetBorResetFlag() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CSR, constants::BORRSTF);
    }

    bool ClockControl::GetPinResetFlag() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CSR, constants::PINRSTF);
    }

    bool ClockControl::GetPorResetFlag() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CSR, constants::PORRSTF);
    }

    bool ClockControl::GetSoftwareResetFlag() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CSR, constants::SFTRSTF);
    }

    bool ClockControl::GetIwdgResetFlag() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CSR, constants::IWDGRSTF);
    }

    bool ClockControl::GetWwdgResetFlag() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CSR, constants::WWDGRSTF);
    }

    bool ClockControl::GetLowpowerResetFlag() const noexcept {
        return libs::MWR::readBit<std::uint32_t>(CSR, constants::LPWRRSTF);
    }
}
