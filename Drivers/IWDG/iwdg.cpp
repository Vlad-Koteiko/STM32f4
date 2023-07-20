//
// Created by komiak_ap on 01.03.2023.
//

#include "iwdg.h"

/**
namespace drivers::iwdg
@brief Драйвер Watchdog-таймера
*/
namespace drivers::iwdg
{

    IWDG::IWDG(PrescalerIWDG prescaller, std::int16_t reloadValue)
    {
        libs::MWR::write_register(IWDG_KR, KEY_ENABLE);
        libs::MWR::write_register(IWDG_KR, KEY_WR_ACCESS_ENABLE);
        libs::MWR::write_register(IWDG_PR, prescaller);
        libs::MWR::write_register(IWDG_RLR, reloadValue);

        libs::MWR::write_register(IWDG_KR, KEY_RELOAD);
    }

    IWDG::IWDG(std::chrono::milliseconds period)
    {
        std::uint32_t temp = period.count();

        libs::MWR::write_register(IWDG_KR, KEY_ENABLE);
        libs::MWR::write_register(IWDG_KR, KEY_WR_ACCESS_ENABLE);

        if(temp <= 512)
        {
            libs::MWR::write_register(IWDG_PR, PrescalerIWDG::IWDG_PRESCALER_4);
            libs::MWR::write_register(IWDG_RLR, temp * 0xFFF / 512);
        }
        else if(temp <= 1024)
        {
            libs::MWR::write_register(IWDG_PR, PrescalerIWDG::IWDG_PRESCALER_8);
            libs::MWR::write_register(IWDG_RLR, temp * 0xFFF / 1024);
        }
        else if(temp <= 2048)
        {
            libs::MWR::write_register(IWDG_PR, PrescalerIWDG::IWDG_PRESCALER_16);
            libs::MWR::write_register(IWDG_RLR, temp * 0xFFF / 2048);
        }
        else if(temp <= 4096)
        {
            libs::MWR::write_register(IWDG_PR, PrescalerIWDG::IWDG_PRESCALER_32);
            libs::MWR::write_register(IWDG_RLR, temp * 0xFFF / 4096);
        }
        else if(temp <= 8192)
        {
            libs::MWR::write_register(IWDG_PR, PrescalerIWDG::IWDG_PRESCALER_64);
            libs::MWR::write_register(IWDG_RLR, temp * 0xFFF / 8192);
        }
        else if(temp <= 16384)
        {
            libs::MWR::write_register(IWDG_PR, PrescalerIWDG::IWDG_PRESCALER_128);
            libs::MWR::write_register(IWDG_RLR, temp * 0xFFF / 16384);
        }
        else if(temp <= 32768)
        {
            libs::MWR::write_register(IWDG_PR, PrescalerIWDG::IWDG_PRESCALER_256);
            libs::MWR::write_register(IWDG_RLR, temp * 0xFFF / 32768);
        }

        libs::MWR::write_register(IWDG_KR, KEY_RELOAD);
    }

    void IWDG::Refresh()
    {
        libs::MWR::write_register(IWDG_KR, KEY_RELOAD);
    }

    void IWDG::EnableWriteAccess()
    {
        libs::MWR::write_register(IWDG_KR, KEY_WR_ACCESS_ENABLE);
    }

    void IWDG::DisableWriteAccess()
    {
        libs::MWR::write_register(IWDG_KR, KEY_WR_ACCESS_DISABLE);
    }

    void IWDG::SetPrescaler(PrescalerIWDG pres)
    {
        libs::MWR::write_register(IWDG_PR, pres);
    }

    std::uint32_t IWDG::GetPrescaler()
    {
        return libs::MWR::read_register<std::uint32_t>(IWDG_PR);
    }

    void IWDG::SetReloadCounter(std::uint32_t counter)
    {
        libs::MWR::write_register(IWDG_RLR, counter);
    }

    std::uint32_t IWDG::GetReloadCounter()
    {
        return libs::MWR::read_register<std::uint32_t>(IWDG_RLR);
    }

    bool IWDG::IsActiveFlagPVU()
    {
        return !libs::MWR::readBit<std::uint32_t>(IWDG_SR, PVU);
    }

    bool IWDG::IsActiveFlagRVU()
    {
        return !libs::MWR::readBit<std::uint32_t>(IWDG_SR, RVU);
    }

    bool IWDG::IsReady()
    {
        return IWDG::IsActiveFlagPVU() | IWDG::IsActiveFlagRVU();
    }
}    // namespace drivers::iwdg