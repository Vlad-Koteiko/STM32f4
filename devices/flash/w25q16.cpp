
#include "w25q16.h"

namespace libs::flash
{
    void W25Q16::UnSelect() noexcept {
        csPort.SetPin(csPin, drivers::port::PIN_SET);
    }

    void W25Q16::Select() noexcept {
        csPort.SetPin(csPin, drivers::port::PIN_RESET);
    }


    void W25Q16::WaitForWriteEnd() noexcept{
        std::uint8_t tempReg;

        do
        {
            tempReg = ReadStatusRegister1();
            drivers::systick::SysTick::DelayMs(1);
        } while ((tempReg & 0x01) == 0x01);

    }

    W25Q16::W25Q16(drivers::spi::SPI &flashSPI, drivers::port::GPIO &csPorT, drivers::port::PIN_NUMBER csPiN)
    : flashSpi(flashSPI), csPort(csPorT), csPin(csPiN){

        UnSelect();

    }

    void W25Q16::WriteEnable() noexcept {
        Select();
        std::uint8_t buf = WRITE_ENABLE;
        flashSpi.TransmitReceiveArray(&buf, &buf, 1);
        UnSelect();
    }

    void W25Q16::WriteDisable() noexcept {
        Select();
        std::uint8_t buf = WRITE_DISABLE;
        flashSpi.TransmitReceiveArray(&buf, &buf, 1);
        UnSelect();
    }


    std::uint32_t W25Q16::ReadID() noexcept{
        Select();
        std::uint8_t buf[4];
        buf[0] = JEDEC_ID;
        buf[1] = 0xFF;
        buf[2] = 0xFF;
        buf[3] = 0xFF;
        flashSpi.TransmitReceiveArray(buf, buf, 4);
        UnSelect();
        return (buf[1] << 16) | (buf[2] << 8) | ((std::uint32_t)buf[3]);
    }

    std::uint8_t W25Q16::ReadStatusRegister1() noexcept{
        Select();
        std::uint8_t buf[2];
        buf[0] = READ_STATUS_REGISTER1;
        buf[1] = 0xFF;
        flashSpi.TransmitReceiveArray(buf, buf, 2);
        UnSelect();
        return buf[1];
    }

    void W25Q16::WriteStatusRegister1(std::uint8_t reg) noexcept {
        WriteEnable();
        Select();
        std::uint8_t buf[2];
        buf[0] = WRITE_STATUS_REGISTER1;
        buf[1] = reg;
        flashSpi.TransmitArray8(buf, 2);
        UnSelect();
        WriteDisable();
    }

    std::uint8_t W25Q16::ReadStatusRegister2() noexcept {
        Select();
        std::uint8_t buf[2];
        buf[0] = READ_STATUS_REGISTER2;
        buf[1] = 0xFF;
        flashSpi.TransmitReceiveArray(buf, buf, 2);
        UnSelect();
        return buf[1];
    }

    void W25Q16::WriteStatusRegister2(std::uint8_t reg) noexcept {
        WriteEnable();
        Select();
        std::uint8_t buf[2];
        buf[0] = WRITE_STATUS_REGISTER2;
        buf[1] = reg;
        flashSpi.TransmitArray8(buf, 2);
        UnSelect();
        WriteDisable();
    }

    std::uint8_t W25Q16::ReadStatusRegister3() noexcept {
        Select();
        std::uint8_t buf[2];
        buf[0] = READ_STATUS_REGISTER3;
        buf[1] = 0xFF;
        flashSpi.TransmitReceiveArray(buf, buf, 2);
        UnSelect();
        return buf[1];
    }

    void W25Q16::WriteStatusRegister3(std::uint8_t reg) noexcept {
        WriteEnable();
        Select();
        std::uint8_t buf[2];
        buf[0] = WRITE_STATUS_REGISTER3;
        buf[1] = reg;
        flashSpi.TransmitArray8(buf, 2);
        UnSelect();
        WriteDisable();
    }

    void W25Q16::EraseChip() noexcept {
        WriteEnable();
        Select();
        std::uint8_t buf = CHIP_ERASE;
        flashSpi.TransmitReceiveArray(&buf, &buf, 1);
        UnSelect();
        WaitForWriteEnd();
        WriteDisable();
    }

    void W25Q16::EraseSector(std::uint32_t address) noexcept {
        WriteEnable();
        Select();
        std::uint8_t buf[4];
        buf[0] = SECTOR_ERASE;
        buf[1] = (address & 0xFF0000) >> 16;
        buf[2] = (address & 0x00FF00) >> 8;
        buf[3] = (address & 0x0000FF);
        flashSpi.TransmitReceiveArray(buf, buf, 4);
        UnSelect();

        WaitForWriteEnd();
        WriteDisable();
    }

    void W25Q16::EraseBlock(std::uint32_t address) noexcept {
        WriteEnable();
        Select();
        std::uint8_t buf[4];
        buf[0] = BLOCK_ERASE;
        buf[1] = (address & 0xFF0000) >> 16;
        buf[2] = (address & 0x00FF00) >> 8;
        buf[3] = (address & 0x0000FF);
        flashSpi.TransmitReceiveArray(buf, buf, 4);
        UnSelect();
        WaitForWriteEnd();
        WriteDisable();
    }

    void W25Q16::ReadBytes(std::uint32_t address, std::uint8_t *rxData, std::uint32_t bytesToRead) noexcept{
        Select();
        std::uint8_t buf[4];
        std::uint8_t dummy = 0xFF;
        buf[0] = READ_DATA;
        buf[1] = (address & 0xFF0000) >> 16;
        buf[2] = (address & 0x00FF00) >> 8;
        buf[3] = (address & 0x0000FF);

        flashSpi.TransmitReceiveArray(buf, buf, 4);
        flashSpi.TransmitReceiveArray(&dummy, rxData, bytesToRead);

        UnSelect();
        WaitForWriteEnd();
    }

    void W25Q16::WriteBytes(std::uint32_t address, std::uint8_t *txData, std::uint32_t bytesToWrite) noexcept{

        WriteEnable();
        Select();
        std::uint8_t buf[32];
        std::uint8_t dummy = 0xFF;
        buf[0] = WRITE_DATA;
        buf[1] = (address & 0xFF0000) >> 16;
        buf[2] = (address & 0x00FF00) >> 8;
        buf[3] = (address & 0x0000FF);

        flashSpi.TransmitReceiveArray(buf, buf, 4);
        flashSpi.TransmitReceiveArray(txData, &dummy, bytesToWrite);
        UnSelect();

        WaitForWriteEnd();

        WriteDisable();
    }
}