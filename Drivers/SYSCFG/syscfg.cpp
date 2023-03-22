//
// Created by koteiko_vv on 09.02.2023.
//
#include "syscfg.h"

namespace drivers::syscfg {

    SYSCFG::SYSCFG(drivers::clock::ClockControl &clockControl) : clockControl(clockControl) {
        clockControl.EnablePeripherals(drivers::clock::SYSCF_MODULE);
    }

    void SYSCFG::SetMemoryMap(MEM_MODE mode) {
        libs::MWR::modifyResetRegister(MEMRMP, 0x3); //обнуляем регистр
        libs::MWR::modifySetRegister(MEMRMP, mode);
    }

    std::uint8_t SYSCFG::GetMemoryMap() {
        return libs::MWR::read_register<std::uint8_t>(MEMRMP);
    }

    void SYSCFG::MiiPhyEnable() {
        libs::MWR::resetBit(PMC, 23); //сбрасываем бит MII_RMII_SEL
    }

    void SYSCFG::RmiiPhyEnable() {
        libs::MWR::setBit(PMC, 23); //устанавливаем бит MII_RMII_SEL
    }

    void SYSCFG::SetSourceEXTI(EXTI_PORT exitPort,
                               EXTI_NUMBER exitNumber) noexcept {

        if (exitNumber <= 3)
        {
            libs::MWR::modifyResetRegister(EXTICR1, 0xF << (exitNumber * 4));
            libs::MWR::modifySetRegister(EXTICR1, exitPort << (exitNumber * 4));

        } else if((exitNumber >= 4) && (exitNumber <= 7))
        {
            libs::MWR::modifyResetRegister(EXTICR2, 0xF << (exitNumber * 4));
            libs::MWR::modifySetRegister(EXTICR2, (exitPort << ((exitNumber - 4) * 4)));

        } else if((exitNumber >= 8) && (exitNumber <= 11))
        {
            libs::MWR::modifyResetRegister(EXTICR3, 0xF << (exitNumber * 4));
            libs::MWR::modifySetRegister(EXTICR3, exitPort << ((exitNumber - 8) * 4));

        } else if((exitNumber >= 12) && (exitNumber <= 15))
        {
            libs::MWR::modifyResetRegister(EXTICR4, 0xF << (exitNumber * 4));
            libs::MWR::modifySetRegister(EXTICR4, exitPort << ((exitNumber - 12) * 4));
        }
    }

    void SYSCFG::CompensationCellEnabled() {
        libs::MWR::setBit(CMPCR, 0);
    }

    void SYSCFG::CompensationCellDisabled() {
        libs::MWR::resetBit(CMPCR, 0);
    }

    bool SYSCFG::CompensationCellReady() {
        return libs::MWR::readBit<std::uint32_t>(CMPCR, 8);
    }
}