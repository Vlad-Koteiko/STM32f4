//
// Created by koteiko_vv on 17.02.2023.
//

#include "flash.hpp"

namespace drivers::flash
{
    void Flash::SetLatency(std::uint8_t latency) const noexcept
    {
        libs::MWR::modifySetRegister(ACR, latency);
    }

    std::uint8_t Flash::GetLatency() const noexcept
    {
        return (libs::MWR::read_register<std::uint32_t>(ACR) & 0x7);
    }

    void Flash::EnablePrefetch() const noexcept
    {
        libs::MWR::setBit(ACR, PRFTEN);
    }

    void Flash::DisablePrefetch() const noexcept
    {
        libs::MWR::resetBit(ACR, PRFTEN);
    }

    bool Flash::IsPrefetchEnabled() const noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(ACR, PRFTEN);
    }

    void Flash::EnableInstCache() const noexcept
    {
        libs::MWR::setBit(ACR, ICEN);
    }

    void Flash::DisableInstCache() const noexcept
    {
        libs::MWR::resetBit(ACR, ICEN);
    }

    void Flash::EnableDataCache() const noexcept
    {
        libs::MWR::setBit(ACR, DCEN);
    }

    void Flash::DisableDataCache() const noexcept
    {
        libs::MWR::resetBit(ACR, DCEN);
    }

    void Flash::EnableInstCacheReset() const noexcept
    {
        libs::MWR::setBit(ACR, ICRST);
    }

    void Flash::DisableInstCacheReset() const noexcept
    {
        libs::MWR::resetBit(ACR, ICRST);
    }

    void Flash::EnableDataCacheReset() const noexcept
    {
        libs::MWR::setBit(ACR, DCRST);
    }

    void Flash::DisableDataCacheReset() const noexcept
    {
        libs::MWR::resetBit(ACR, DCRST);
    }

    void Flash::Unlock() const noexcept
    {
        libs::MWR::write_register<std::uint32_t>(KEYR, FLASH_KEY1);
        libs::MWR::write_register<std::uint32_t>(KEYR, FLASH_KEY2);
    }

    void Flash::Lock() const noexcept
    {
        libs::MWR::setBit(CR, LOCK);
    }

    void Flash::ObUnlock() const noexcept
    {
        libs::MWR::write_register<std::uint32_t>(OPTKEYR, FLASH_OPT_KEY1);
        libs::MWR::write_register<std::uint32_t>(OPTKEYR, FLASH_OPT_KEY2);
    }

    void Flash::ObLock() const noexcept
    {
        libs::MWR::setBit(OPTCR, OPTLOCK);
    }

    void Flash::ProgrammDoubleWord(std::uintptr_t address, std::uint64_t data) const noexcept
    {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, DOUBLEWORD << PSIZE);
        libs::MWR::setBit(CR, PG);

        ProgrammWord(address, data);
        //__asm("ISB");
        ProgrammWord(address + 4, data >> 32);
    }

    void Flash::ProgrammWord(std::uintptr_t address, std::uint32_t data) const noexcept
    {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, WORD << PSIZE);
        libs::MWR::setBit(CR, PG);
        libs::MWR::write_register(address, data);
    }

    void Flash::ProgrammHalfWord(std::uintptr_t address, std::uint16_t data) const noexcept
    {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, HALF_WORD << PSIZE);
        libs::MWR::setBit(CR, PG);

        //        libs::MWR::write_register(static_cast<std::uint16_t>(address), data);
        *reinterpret_cast<volatile std::uint16_t *>(address) = static_cast<std::uint16_t>(data);
    }

    void Flash::ProgrammByte(std::uintptr_t address, std::uint8_t data) const noexcept
    {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, BYTE << PSIZE);
        libs::MWR::setBit(CR, PG);

        *reinterpret_cast<volatile std::uint8_t *>(address) = static_cast<std::uint8_t>(data);
    }

    std::uint64_t Flash::ReadDoubleWord(std::uintptr_t address) const noexcept
    {
        return libs::MWR::read_register<std::uint64_t>(address);
    }

    std::uint32_t Flash::ReadWord(std::uintptr_t address) const noexcept
    {
        return libs::MWR::read_register<std::uint32_t>(address);
    }

    std::uint16_t Flash::ReadHalfWord(std::uintptr_t address) const noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(address);
    }

    std::uint8_t Flash::ReadByte(std::uintptr_t address) const noexcept
    {
        return libs::MWR::read_register<std::uint8_t>(address);
    }

    void Flash::SetERRIE(STATUS x) const noexcept
    {
        if(x == DISABLE)
            libs::MWR::resetBit(CR, ERRIE);
        else
            libs::MWR::setBit(CR, ERRIE);
    }

    bool Flash::GetERRIE() const noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(CR, ERRIE);
    }

    void Flash::SetEOPIE(STATUS x) const noexcept
    {
        if(x == DISABLE)
            libs::MWR::resetBit(CR, EOPIE);
        else
            libs::MWR::setBit(CR, EOPIE);
    }

    bool Flash::GetEOPIE() const noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(CR, EOPIE);
    }

    void Flash::EraseSector(std::uint16_t sector, VOLTAGE voltage) const noexcept
    {
        libs::MWR::modifyResetRegister(CR, 0x3 << PSIZE);
        libs::MWR::modifySetRegister(CR, voltage << PSIZE);
        libs::MWR::modifyResetRegister(CR, 0xFF << SNB);
        libs::MWR::modifySetRegister(CR, sector << SNB);
        libs::MWR::setBit(CR, SER);
        libs::MWR::setBit(CR, STRT);
    }
}    // namespace drivers::flash