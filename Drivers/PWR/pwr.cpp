
#include "pwr.h"

namespace drivers::pwr
{
    PWR::PWR(drivers::clock::ClockControl &clockControl) : clockControl(clockControl){
        clockControl.EnablePeripherals(drivers::clock::PWR_MODULE);
    }

    void PWR::SetLPDS() {
        libs::MWR::setBit(CR, LPDS);
    }

    void PWR::ResetLPDS() {
        libs::MWR::resetBit(CR, LPDS);
    }

    void PWR::SetPDDS() {
        libs::MWR::setBit(CR, PDDS);
    }

    void PWR::ResetPDDS() {
        libs::MWR::resetBit(CR, PDDS);
    }

    void PWR::ClearWakeUp() {
        libs::MWR::setBit(CR, CWUF);
    }

    void PWR::ClearStandbyFlag() {
        libs::MWR::setBit(CR, CSBF);
    }

    void PWR::PvdEnabled() {
        libs::MWR::setBit(CR, PVDE);
    }

    void PWR::PvdDisable() {
        libs::MWR::resetBit(CR, PVDE);
    }

    bool PWR::GetStatusPvd() {
        return libs::MWR::readBit<std::uint32_t>(CR, PVDE);
    }

    void PWR::SetPvdLevel(VOLTAGE v) {
        libs::MWR::modifyResetRegister(CR, 0x7 << PLS);
    }

    std::uint8_t PWR::GetPdvLevel() {
        return (std::uint8_t)((libs::MWR::read_register<std::uint32_t>(CR) >> PLS) & 0x7);
    }

    void PWR::BackupDomainEnable() {
        libs::MWR::setBit(CR, DBP);
    }

    void PWR::BackupDomainDisable() {
        libs::MWR::resetBit(CR, DBP);
    }

    bool PWR::GetStatusBackupDomain() {
        return libs::MWR::readBit<std::uint32_t>(CR, DBP);
    }
}