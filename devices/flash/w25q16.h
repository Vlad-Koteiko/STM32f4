#ifndef W25Q16_H
#define W25Q16_H

#include "gpio.h"
#include "spi.h"
#include "systick.h"

namespace devices::flash
{
    enum DEFAULT_COMMANDS : std::uint8_t
    {
        WRITE_STATUS_REGISTER1 = 0x01,
        WRITE_DATA             = 0x02,
        READ_DATA              = 0x03,
        WRITE_DISABLE          = 0x04,
        READ_STATUS_REGISTER1  = 0x05,
        WRITE_ENABLE           = 0x06,
        WRITE_STATUS_REGISTER3 = 0x11,
        READ_STATUS_REGISTER3  = 0x15,
        SECTOR_ERASE           = 0x20,
        WRITE_STATUS_REGISTER2 = 0x31,
        READ_STATUS_REGISTER2  = 0x35,
        JEDEC_ID               = 0x9F,
        CHIP_ERASE             = 0xC7,
        BLOCK_ERASE            = 0xD8
    };

    class W25Q16
    {
        drivers::spi::SPI        &flashSpi;
        drivers::port::GPIO      &csPort;
        drivers::port::PIN_NUMBER csPin;

        void UnSelect() noexcept;
        void Select() noexcept;
        void WaitForWriteEnd() noexcept;

    public:
        W25Q16(drivers::spi::SPI        &flashSPI,
               drivers::port::GPIO      &csPorT,
               drivers::port::PIN_NUMBER csPiN);
        std::uint32_t ReadID() noexcept;
        std::uint8_t  ReadStatusRegister1() noexcept;
        void          WriteStatusRegister1(std::uint8_t reg) noexcept;
        std::uint8_t  ReadStatusRegister2() noexcept;
        void          WriteStatusRegister2(std::uint8_t reg) noexcept;
        std::uint8_t  ReadStatusRegister3() noexcept;
        void          WriteStatusRegister3(std::uint8_t reg) noexcept;
        void          WriteEnable() noexcept;
        void          WriteDisable() noexcept;

        void EraseChip() noexcept;
        void EraseSector(std::uint32_t address) noexcept;
        void EraseBlock(std::uint32_t address) noexcept;

        void ReadBytes(std::uint32_t address,
                       std::uint8_t *rxData,
                       std::uint32_t bytesToRead) noexcept;
        void WriteBytes(std::uint32_t address,
                        std::uint8_t *txData,
                        std::uint32_t bytesToWrite) noexcept;
    };
}    // namespace devices::flash

#endif    // W25Q16_H