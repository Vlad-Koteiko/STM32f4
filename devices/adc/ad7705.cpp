
#include "ad7705.h"

#include "printf.h"

namespace devices::ad7705
{
    Ad7705::Ad7705(drivers::spi::SPI &spiADC, drivers::port::GPIO &portRST, drivers::port::PIN_NUMBER pinRST,
                   drivers::port::GPIO &portCS, drivers::port::PIN_NUMBER pinCS, drivers::port::GPIO &portDRDY,
                   drivers::port::PIN_NUMBER pinDRDY) :
                   spi(spiADC), portRST(portRST), portCS(portCS), portDRDY(portDRDY)
                   {
                       this->pinCS = pinCS;
                       this->pinRST = pinRST;
                       this->pinDRDY = pinDRDY;

                       portRST.SetPin(pinRST, drivers::port::PIN_SET);
                       HighCS();

    }

    void Ad7705::LowCS() noexcept {
        portCS.SetPin(pinCS, drivers::port::PIN_RESET);
    }

    void Ad7705::HighCS() noexcept {
        portCS.SetPin(pinCS, drivers::port::PIN_SET);
    }

    void Ad7705::Init(Modes m, Gain g, OutputUpadateRate uor) noexcept {

        std::uint8_t buf = 0x00;
        WriteSetup(m, g, 1, 0, 0);
        WriteClock(0, 1, uor);
    }

    void Ad7705::WriteCommunicationReg(Registers reg, std::uint8_t readOrWrite, std::uint8_t channel)  noexcept{
        std::uint8_t buf = 0;
        buf = reg | (readOrWrite << 3) | channel;
        LowCS();
        spi.TransmitReceiveArray(&buf, &buf, 1);
        HighCS();
    }

    std::uint8_t Ad7705::ReadCommunicationReg() noexcept {

        WriteCommunicationReg(COMMUNICATION, 1, 0);
        LowCS();
        std::uint8_t reg = spi.ReceiveData8();
        HighCS();
        return reg;
    }

    void Ad7705::WriteSetup(Modes m, Gain g, std::uint8_t bu, std::uint8_t buf, std::uint8_t fsync) noexcept {
        std::uint8_t reg = 0;

        WriteCommunicationReg(SETUP, 0, 0);
        LowCS();
        reg = m | g | (bu << 2) | (buf << 1) | fsync;
        spi.TransmitReceiveArray(&reg, &reg, 1);
        HighCS();
    }

    std::uint8_t Ad7705::ReadSetup() noexcept {
        std::uint8_t reg = 0;
        WriteCommunicationReg(SETUP, 1, 0);
        LowCS();
        spi.TransmitReceiveArray(&reg, &reg, 1);
        HighCS();
        return reg;
    }

    void Ad7705::WriteClock(std::uint8_t clkdis, std::uint8_t clkdiv, OutputUpadateRate our) noexcept {
        std::uint8_t reg;
        reg = (clkdis << 4) | (clkdiv << 3) | our;
        WriteCommunicationReg(CLOCK, 0, 0);
        LowCS();
        spi.TransmitReceiveArray(&reg, &reg, 1);
        HighCS();
    }

    std::uint8_t Ad7705::ReadClock() noexcept {
        std::uint8_t reg;
        WriteCommunicationReg(CLOCK, 1, 0);
        LowCS();
        spi.TransmitReceiveArray(&reg, &reg, 1);
        HighCS();
        return reg;
    }

    bool Ad7705::ReadyData() noexcept {
        std::uint8_t reg;
        WriteCommunicationReg(COMMUNICATION, 1, 0);
        LowCS();
        spi.TransmitReceiveArray(&reg, &reg, 1);
        HighCS();

        return (reg & 0x80) == 0x00;
    }

    std::uint16_t Ad7705::ReadData(Channel ch) noexcept {
        while (!ReadyData()) {}
        WriteCommunicationReg(DATA, 1, ch);
        std::uint8_t val[2];
        LowCS();
        spi.TransmitReceiveArray(val, val, 2);
        HighCS();

        return ((std::uint16_t)val[0] << 8) | ((std::uint16_t)val[1]);
    }

    std::uint8_t Ad7705::ReadTestReg() noexcept {
        WriteCommunicationReg(TEST, 1, 0);
        std::uint8_t reg = 0;
        LowCS();
        spi.TransmitReceiveArray(&reg, &reg, 1);
        HighCS();
        return reg;
    }

    std::uint32_t Ad7705::ReadZeroScaleCalibrReg(Channel ch) noexcept {
        WriteCommunicationReg(ZERO_SCALE_CALIBRATION, 1, ch);
        std::uint8_t val[3];
        LowCS();
        spi.TransmitReceiveArray(val,val, 3);
        HighCS();
        return ((std::uint32_t)val[0] << 16) | ((std::uint32_t)val[1] << 8) | (std::uint32_t)val[2];
    }

    void Ad7705::WriteZeroScaleCalibrReg(std::uint32_t reg, Channel ch) noexcept {
        WriteCommunicationReg(ZERO_SCALE_CALIBRATION, 0, ch);
        std::uint8_t val[3];
        val[0] = (reg & 0x00FF0000) >> 16;
        val[1] = (reg & 0x0000FF00) >> 8;
        val[2] =  reg & 0x000000FF;
        LowCS();
        spi.TransmitReceiveArray(val, val, 3);
        HighCS();
    }

    std::uint32_t Ad7705::ReadFullScaleCalibrReg(Channel ch) noexcept {
        WriteCommunicationReg(FULL_SCALE_CALIBRATION, 1,ch);
        std::uint8_t  val[3];
        LowCS();
        spi.TransmitReceiveArray(val, val, 3);
        HighCS();
        return ((std::uint32_t)val[0] << 16) | ((std::uint32_t)val[1] << 8) | (std::uint32_t)val[2];
    }

    void Ad7705::WriteFullScaleCalibrReg(std::uint32_t reg, Channel ch) noexcept {
        WriteCommunicationReg(FULL_SCALE_CALIBRATION, 0, ch);
        std::uint8_t val[3];
        val[0] = (reg & 0x00FF0000) >> 16;
        val[1] = (reg & 0x0000FF00) >> 8;
        val[2] =  reg & 0x000000FF;
        LowCS();
        spi.TransmitReceiveArray(val, val, 3);
        HighCS();
    }
}