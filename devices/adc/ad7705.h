#ifndef AD7705_H
#define AD7705_H

#include "gpio.hpp"
#include "spi.hpp"

/*
drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);

drivers::spi::SPI spi1(clockControl, drivers::spi::SPI1);
spi1.RemapSPI1(drivers::spi::SPI1_PB5_PB4_PB3_PA15);
//spi1.SetClockPhase(drivers::spi::EDGE1);
spi1.SetClockPhase(drivers::spi::EDGE2);
//spi1.SetClockPolarity(drivers::spi::LOW);
spi1.SetClockPolarity(drivers::spi::HIGH);
spi1.SetMode(drivers::spi::MASTER);
spi1.SetBaudRatePrescaler(drivers::spi::DIV8);
spi1.SetTransferBitOrder(drivers::spi::MSB_FIRST);
spi1.SetTransferDirection(drivers::spi::FULL_DUPLEX);
spi1.SetDataWidth(drivers::spi::BIT8);
spi1.SetNSSMode(drivers::spi::HARD_OUTPUT);
spi1.SetStandard(drivers::spi::MOTOROLA);
spi1.Enable();
*/

namespace devices::ad7705
{
    enum Registers : std::uint8_t
    {
        COMMUNICATION          = 0x00,
        SETUP                  = 0x10,
        CLOCK                  = 0x20,
        DATA                   = 0x30,
        TEST                   = 0x40,
        ZERO_SCALE_CALIBRATION = 0x60,
        FULL_SCALE_CALIBRATION = 0x70
    };

    enum Channel : std::uint8_t
    {
        AIN1_PLUS_AIN1_MINUS  = 0x00,
        AIN2_PLUS_AIN2_MINUS  = 0x01,
        AIN1_MINUS_AIN1_MINUS = 0x02,
        AIN1_MINUS_AIN2_MINUS = 0x03
    };

    enum Modes : std::uint8_t
    {
        NORMAL           = 0x00,
        SELF_CALIBRATION = 0x40,
        ZERO_SCALE       = 0x80,
        FULL_SCALE       = 0xC0
    };

    enum Gain : std::uint8_t
    {
        G1   = 0x00,
        G2   = 0x08,
        G4   = 0x10,
        G8   = 0x18,
        G16  = 0x20,
        G32  = 0x28,
        G64  = 0x30,
        G128 = 0x38
    };

    enum OutputUpadateRate : std::uint8_t
    {
        FIN_24576_20Hz  = 0x00,
        FIN_24579_25Hz  = 0x01,
        FIN_24579_100Hz = 0x02,
        FIN_24579_200Hz = 0x03,
        FIN_49152_50Hz  = 0x04,
        FIN_49152_60Hz  = 0x05,
        FIN_49152_250Hz = 0x06,
        FIN_49152_500Hz = 0x07
    };

    class Ad7705
    {
        drivers::spi::SPI        &spi;
        drivers::port::Gpio      &portRST;
        drivers::port::Gpio      &portCS;
        drivers::port::Gpio      &portDRDY;
        drivers::port::PIN_NUMBER pinRST;
        drivers::port::PIN_NUMBER pinCS;
        drivers::port::PIN_NUMBER pinDRDY;

        void LowCS() noexcept;
        void HighCS() noexcept;

    public:
        Ad7705(drivers::spi::SPI        &spiADC,
               drivers::port::Gpio      &portRST,
               drivers::port::PIN_NUMBER pinRST,
               drivers::port::Gpio      &portCS,
               drivers::port::PIN_NUMBER pinCS,
               drivers::port::Gpio      &portDRDY,
               drivers::port::PIN_NUMBER pinDRDY);
        void         Init(Modes m, Gain g, OutputUpadateRate uor) noexcept;
        void         WriteCommunicationReg(Registers    reg,
                                           std::uint8_t readOrWrite,
                                           std::uint8_t channel) noexcept;
        std::uint8_t ReadCommunicationReg() noexcept;
        void         WriteSetup(Modes        m,
                                Gain         g,
                                std::uint8_t bu,
                                std::uint8_t buf,
                                std::uint8_t fsync) noexcept;
        std::uint8_t ReadSetup() noexcept;
        void WriteClock(std::uint8_t clkdis, std::uint8_t clkdiv, OutputUpadateRate our) noexcept;
        std::uint8_t  ReadClock() noexcept;
        bool          ReadyData() noexcept;
        std::uint16_t ReadData(Channel ch) noexcept;
        std::uint8_t  ReadTestReg() noexcept;
        std::uint32_t ReadZeroScaleCalibrReg(Channel ch) noexcept;
        void          WriteZeroScaleCalibrReg(std::uint32_t reg, Channel ch) noexcept;
        std::uint32_t ReadFullScaleCalibrReg(Channel ch) noexcept;
        void          WriteFullScaleCalibrReg(std::uint32_t reg, Channel ch) noexcept;
    };
}    // namespace devices::ad7705

#endif    // AD7705_H