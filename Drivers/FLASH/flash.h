//
// Created by koteiko_vv on 17.02.2023.
//

#ifndef STM32F4_FLASH_H
#define STM32F4_FLASH_H

#include "MWR.hpp"

namespace drivers::flash
{
    enum STATUS : std::uint8_t
    {
        DISABLE,
        ENABLE
    };

    enum ACR_poz : std::uint8_t
    {
        LATENCY = 0,    //[2:0]
        PRFTEN  = 8,    //Prefetch enable
        ICEN    = 9,    //Instruction cache enable
        DCEN    = 10,   //Data cache enable
        ICRST   = 11,   //Instruction cache reset
        DCRST   = 12    //Data cache reset
    };

    enum SR_poz : std::uint8_t
    {
        EOP    = 0, //End of operation
        OPERR  = 1, //Operation error
        WRPERR = 4, //Write protection error
        PGAERR = 5, //Programming alignment error
        PGPERR = 6, //Programming parallelism error
        PGSERR = 7, //Programming sequence error
        BSY    = 16 //Busy
    };

    enum CR_poz : std::uint8_t
    {
        PG    = 0,  //Programming
        SER   = 1,  //Sector Erase
        MER   = 2,  //Mass Erase
        SNB   = 3,  //Sector number [6:3]
        PSIZE = 8,  //Program size [9:8]
        STRT  = 16, //Start
        EOPIE = 24, //End of operation interrupt enable
        ERRIE = 25, //Error interrupt enable
        LOCK  = 31, //Lock
    };

    enum OPTCR_poz : std::uint8_t
    {
        OPTLOCK = 0, //Option lock
        OPTSTRT = 1, //Option start
        BOR_LEV = 2, //BOR reset Level [3:2]
        USER    = 5, //User option bytes [7:5]
        RDP     = 8, //Read protect [15:8]
        nWRP    = 16,//Not write protect [16:27]
    };

    enum FLASH_KEYS : std::uint32_t
    {
        FLASH_KEY1 = 0x45670123,
        FLASH_KEY2 = 0xCDEF89AB,
        FLASH_OPT_KEY1 = 0x08192A3B,
        FLASH_OPT_KEY2 = 0x4C5D6E7F
    };

    enum FLASH_WRITE : std::uint8_t
    {
        BYTE,
        HALF_WORD,
        WORD,
        DOUBLEWORD
    };

    enum VOLTAGE : std::uint8_t
    {
        RANGE1,
        RANGE2,
        RANGE3,
        RANGE4
    };

    class Flash {

        static constexpr std::uint32_t baseAddress = 0x40023c00;

        enum RegisterFLASH : std::uintptr_t {
            ACR       = baseAddress,        // FLASH access control register,   Address offset: 0x00
            KEYR      = baseAddress + 0x04, // FLASH key register,              Address offset: 0x04
            OPTKEYR   = baseAddress + 0x08, // FLASH option key register,       Address offset: 0x08
            SR        = baseAddress + 0x0C, // FLASH status register,           Address offset: 0x0C
            CR        = baseAddress + 0x10, // FLASH control register,          Address offset: 0x10
            OPTCR     = baseAddress + 0x14, // FLASH option control register ,  Address offset: 0x14
        };

    public:

        void SetLatency(std::uint8_t latency) noexcept;
        std::uint8_t GetLatency() noexcept;

        void EnablePrefetch() noexcept;
        void DisablePrefetch() noexcept;
        bool IsPrefetchEnabled() noexcept;
        void EnableInstCache() noexcept;
        void DisableInstCache() noexcept;
        void EnableDataCache() noexcept;
        void DisableDataCache() noexcept;
        void EnableInstCacheReset() noexcept;
        void DisableInstCacheReset() noexcept;
        void EnableDataCacheReset() noexcept;
        void DisableDataCacheReset() noexcept;

        void Unlock();
        void Lock();
        void ObUnlock();
        void ObLock();

        void ProgrammDoubleWord(std::uintptr_t address, std::uint64_t data);
        void ProgrammWord(std::uintptr_t address, std::uint32_t data);
        void ProgrammHalfWord(std::uintptr_t address, std::uint16_t data);
        void ProgrammByte(std::uintptr_t address, std::uint8_t data);

        std::uint64_t ReadDoubleWord(std::uintptr_t address);
        std::uint32_t ReadWord(std::uintptr_t address);
        std::uint16_t ReadHalfWord(std::uintptr_t address);
        std::uint8_t ReadByte(std::uintptr_t address);

        void EraseSector(std::uint16_t sector, VOLTAGE voltage);

        void SetERRIE(STATUS x);
        bool GetERRIE();
        void SetEOPIE(STATUS x);
        bool GetEOPIE();
    };
}

#endif //STM32F4_FLASH_H
