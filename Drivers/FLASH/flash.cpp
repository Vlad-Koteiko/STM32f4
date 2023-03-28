//
// Created by koteiko_vv on 17.02.2023.
//

#include "flash.h"

namespace drivers::flash
{
    void Flash::SetLatency(std::uint8_t latency) noexcept {
        libs::MWR::modifySetRegister(ACR,latency);
    }

    std::uint8_t Flash::GetLatency() noexcept {
        return (libs::MWR::read_register<std::uint32_t>(ACR) & 0x7);
    }

    void Flash::EnablePrefetch() noexcept {
        libs::MWR::setBit(ACR,PRFTEN);
    }

    void Flash::DisablePrefetch() noexcept {
        libs::MWR::resetBit(ACR,PRFTEN);
    }

    bool Flash::IsPrefetchEnabled() noexcept {
        return libs::MWR::readBit<std::uint32_t>(ACR, PRFTEN);
    }

    void Flash::EnableInstCache() noexcept {
        libs::MWR::setBit(ACR,ICEN);
    }

    void Flash::DisableInstCache() noexcept {
        libs::MWR::resetBit(ACR,ICEN);
    }

    void Flash::EnableDataCache() noexcept {
        libs::MWR::setBit(ACR, DCEN);
    }

    void Flash::DisableDataCache() noexcept {
        libs::MWR::resetBit(ACR, DCEN);
    }

    void Flash::EnableInstCacheReset() noexcept {
        libs::MWR::setBit(ACR, ICRST);
    }

    void Flash::DisableInstCacheReset() noexcept {
        libs::MWR::resetBit(ACR, ICRST);
    }

    void Flash::EnableDataCacheReset() noexcept {
        libs::MWR::setBit(ACR, DCRST);
    }

    void Flash::DisableDataCacheReset() noexcept {
        libs::MWR::resetBit(ACR, DCRST);
    }

    void Flash::Unlock() {
        libs::MWR::write_register<std::uint32_t>(KEYR, FLASH_KEY1);
        libs::MWR::write_register<std::uint32_t>(KEYR, FLASH_KEY2);
    }

    void Flash::Lock() {
        libs::MWR::setBit(CR, LOCK);
    }

    void Flash::ObUnlock() {
        libs::MWR::write_register<std::uint32_t>(OPTKEYR, FLASH_OPT_KEY1);
        libs::MWR::write_register<std::uint32_t>(OPTKEYR, FLASH_OPT_KEY2);
    }

    void Flash::ObLock() {
        libs::MWR::setBit(OPTCR, OPTLOCK);
    }

    void Flash::ProgrammDoubleWord(std::uintptr_t address, std::uint64_t data) {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, DOUBLEWORD << PSIZE);
        libs::MWR::setBit(CR, PG);

        ProgrammWord(address, data);
        //__asm("ISB");
        ProgrammWord(address + 4, data >> 32);
    }

    void Flash::ProgrammWord(std::uintptr_t address, std::uint32_t data) {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, WORD << PSIZE);
        libs::MWR::setBit(CR, PG);

        libs::MWR::write_register(address, data);
    }

    void Flash::ProgrammHalfWord(std::uintptr_t address, std::uint16_t data) {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, HALF_WORD << PSIZE);
        libs::MWR::setBit(CR, PG);

//        libs::MWR::write_register(address, data);
        *reinterpret_cast<volatile std::uint16_t *>(address) = static_cast<std::uint16_t>(data);
    }

    void Flash::ProgrammByte(std::uintptr_t address, std::uint8_t data) {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, BYTE << PSIZE);
        libs::MWR::setBit(CR, PG);

        *reinterpret_cast<volatile std::uint8_t *>(address) = static_cast<std::uint8_t>(data);
    }

    std::uint64_t Flash::ReadDoubleWord(std::uintptr_t address) {
        return libs::MWR::read_register<std::uint64_t>(address);
    }

    std::uint32_t Flash::ReadWord(std::uintptr_t address) {
        return libs::MWR::read_register<std::uint32_t>(address);
    }

    std::uint16_t Flash::ReadHalfWord(std::uintptr_t address) {
        return libs::MWR::read_register<std::uint16_t>(address);
    }

    std::uint8_t Flash::ReadByte(std::uintptr_t address) {
        return libs::MWR::read_register<std::uint8_t>(address);
    }

    void Flash::SetERRIE(STATUS x) {
        if(x == DISABLE)
            libs::MWR::resetBit(CR, ERRIE);
        else
            libs::MWR::setBit(CR, ERRIE);
    }

    bool Flash::GetERRIE() {
        return libs::MWR::readBit<std::uint32_t>(CR, ERRIE);
    }

    void Flash::SetEOPIE(STATUS x) {
        if(x == DISABLE)
            libs::MWR::resetBit(CR, EOPIE);
        else
            libs::MWR::setBit(CR, EOPIE);
    }

    bool Flash::GetEOPIE() {
        return libs::MWR::readBit<std::uint32_t>(CR, EOPIE);
    }

    void Flash::EraseSector(std::uint16_t sector, VOLTAGE voltage) {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, voltage << PSIZE);
        libs::MWR::modifyResetRegister(CR, 0xFF << SNB);
        libs::MWR::modifySetRegister(CR, sector << SNB);
        libs::MWR::setBit(CR, SER);
        libs::MWR::setBit(CR, STRT);
    }
}