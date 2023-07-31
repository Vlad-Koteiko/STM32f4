/*! \file flash.hpp
    \brief class FLASH

    Header file with a description of the flash memory class
*/
#ifndef STM32F4_FLASH_H
#define STM32F4_FLASH_H

#include "MWR.hpp"

/// @brief  Namespace for work with FLASH
namespace drivers::flash
{
    /// @brief Enum STATUS
    enum STATUS : std::uint8_t
    {
        DISABLE,    ///< DISABLE - 0
        ENABLE      ///< ENABLE - 1
    };

    /// @brief Flash access control register
    enum ACR_poz : std::uint8_t
    {
        LATENCY = 0,     ///< [2:0]
        PRFTEN  = 8,     ///< Prefetch enable
        ICEN    = 9,     ///< Instruction cache enable
        DCEN    = 10,    ///< Data cache enable
        ICRST   = 11,    ///< Instruction cache reset
        DCRST   = 12     ///< Data cache reset
    };

    /// @brief  Flash status register
    enum SR_poz : std::uint8_t
    {
        EOP    = 0,    ///< End of operation
        OPERR  = 1,    ///< Operation error
        WRPERR = 4,    ///< Write protection error
        PGAERR = 5,    ///< Programming alignment error
        PGPERR = 6,    ///< Programming parallelism error
        PGSERR = 7,    ///< Programming sequence error
        BSY    = 16    ///< Busy
    };

    /// @brief Flash control register
    enum CR_poz : std::uint8_t
    {
        PG    = 0,     ///< Programming
        SER   = 1,     ///< Sector Erase
        MER   = 2,     ///< Mass Erase
        SNB   = 3,     ///< Sector number [6:3]
        PSIZE = 8,     ///< Program size [9:8]
        STRT  = 16,    ///< Start
        EOPIE = 24,    ///< End of operation interrupt enable
        ERRIE = 25,    ///< Error interrupt enable
        LOCK  = 31,    ///< Lock
    };

    /// @brief Flash option control register
    enum OPTCR_poz : std::uint8_t
    {
        OPTLOCK = 0,     ///< Option lock
        OPTSTRT = 1,     ///< Option start
        BOR_LEV = 2,     ///< BOR reset Level [3:2]
        USER    = 5,     ///< User option bytes [7:5]
        RDP     = 8,     ///< Read protect [15:8]
        nWRP    = 16,    ///< Not write protect [16:27]
    };

    /// @brief Keys
    enum FLASH_KEYS : std::uint32_t
    {
        FLASH_KEY1     = 0x45670123,
        FLASH_KEY2     = 0xCDEF89AB,
        FLASH_OPT_KEY1 = 0x08192A3B,
        FLASH_OPT_KEY2 = 0x4C5D6E7F
    };

    /// @brief Write flash
    enum FLASH_WRITE : std::uint8_t
    {
        BYTE,
        HALF_WORD,
        WORD,
        DOUBLEWORD
    };

    /// @brief Voltage
    enum VOLTAGE : std::uint8_t
    {
        RANGE1,
        RANGE2,
        RANGE3,
        RANGE4
    };

    /// @brief Class for working with FLASH
    class Flash
    {
        static constexpr std::uint32_t baseAddress = 0x40023c00;    ///< base address

        // clang-format off

        /// @brief Registers FLASH
        enum RegisterFLASH : std::uintptr_t
        {
            ACR     = baseAddress,              ///< FLASH access control register,   Address offset: 0x00
            KEYR    = baseAddress + 0x04,       ///< FLASH key register,              Address offset: 0x04
            OPTKEYR = baseAddress + 0x08,       ///< FLASH option key register,       Address offset: 0x08
            SR      = baseAddress + 0x0C,       ///< FLASH status register,           Address offset: 0x0C
            CR      = baseAddress + 0x10,       ///< FLASH control register,          Address offset: 0x10
            OPTCR   = baseAddress + 0x14        ///< FLASH option control register ,  Address offset: 0x14
        };

        // clang-format on

    public:
        /// @brief Set latency
        /// @param latency
        void SetLatency(std::uint8_t latency) const noexcept;

        /// @brief Get latency
        /// @return latency
        [[nodiscard]] std::uint8_t GetLatency() const noexcept;

        /// @brief Enable prefetch
        void EnablePrefetch() const noexcept;

        /// @brief Disable prefetch
        void DisablePrefetch() const noexcept;

        /// @brief Is prefetch enabled
        /// @return true - enable
        bool IsPrefetchEnabled() const noexcept;

        /// @brief Instruction cache enable
        void EnableInstCache() const noexcept;

        /// @brief Instruction cache disable
        void DisableInstCache() const noexcept;

        /// @brief  Data cache enable
        void EnableDataCache() const noexcept;

        /// @brief  Data cache disable
        void DisableDataCache() const noexcept;

        /// @brief Instruction cache reset enable
        void EnableInstCacheReset() const noexcept;

        /// @brief Instruction cache reset disable
        void DisableInstCacheReset() const noexcept;

        /// @brief  Data cache reset enable
        void EnableDataCacheReset() const noexcept;

        /// @brief  Data cache reset disable
        void DisableDataCacheReset() const noexcept;

        /// @brief Unlock flash
        void Unlock() const noexcept;

        /// @brief Lock flash
        void Lock() const noexcept;

        /// @brief Unlock options flash
        void ObUnlock() const noexcept;

        /// @brief Lock options flash
        void ObLock() const noexcept;

        /// @brief Programm double word in flash
        /// @param address address
        /// @param data data
        void ProgrammDoubleWord(std::uintptr_t address, std::uint64_t data) const noexcept;

        /// @brief Programm word in flash
        /// @param address address
        /// @param data data
        void ProgrammWord(std::uintptr_t address, std::uint32_t data) const noexcept;

        /// @brief Programm half word in flash
        /// @param address address
        /// @param data data
        void ProgrammHalfWord(std::uintptr_t address, std::uint16_t data) const noexcept;

        /// @brief Programm byte in flash
        /// @param address address
        /// @param data data
        void ProgrammByte(std::uintptr_t address, std::uint8_t data) const noexcept;

        /// @brief Read double word
        /// @param address address
        /// @return data
        [[nodiscard]] std::uint64_t ReadDoubleWord(std::uintptr_t address) const noexcept;

        /// @brief Read word
        /// @param address address
        /// @return data
        [[nodiscard]] std::uint32_t ReadWord(std::uintptr_t address) const noexcept;

        /// @brief Read half word
        /// @param address address
        /// @return data
        [[nodiscard]] std::uint16_t ReadHalfWord(std::uintptr_t address) const noexcept;

        /// @brief Read byte
        /// @param address address
        /// @return data
        [[nodiscard]] std::uint8_t ReadByte(std::uintptr_t address) const noexcept;

        /// @brief Erase secotor
        /// @param sector Number secor
        /// @param voltage Level voltage
        void EraseSector(std::uint16_t sector, VOLTAGE voltage) const noexcept;

        /// @brief Error interrupt
        /// @param x ENABLE or DISABLE
        void SetERRIE(STATUS x) const noexcept;

        /// @brief Get status error interrupt
        /// @return true - enable
        bool GetERRIE() const noexcept;

        /// @brief  End of operation interrupt
        /// @param x ENABLE or DISABLE
        void SetEOPIE(STATUS x) const noexcept;

        /// @brief Get status  end of operation interrupt
        /// @return true - enable
        bool GetEOPIE() const noexcept;
    };
}    // namespace drivers::flash

#endif    // STM32F4_FLASH_H
