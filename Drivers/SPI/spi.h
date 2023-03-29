
#ifndef SPI_H
#define SPI_H

#include "MWR.hpp"

namespace drivers::spi
{
    enum ADDRESES_SPI : std::uintptr_t
    {
        SPI1 = 0x40013000,
        SPI2 = 0x40003800,
        SPI3 = 0x40003C00,
        SPI4 = 0x40013400,
        SPI5 = 0x40015000,
        SPI6 = 0x40015400
    };

    class SPI
    {
        enum RegistersSPI : std::uintptr_t
        {
            CR1 = 0x00, // Control register 1
            CR2 = 0x04, // Control register 2
            SR = 0x08, // Status register
            DR = 0x0C, // Data register
            CRCPR = 0x10, // CRC polynomial register
            RXCRCR = 0x14, // RX CRC register
            TXCRCR = 0x18, // TX CRC register
            I2SCFGR = 0x1C, // I2S configuration register
            I2SPR = 0x20 // I2S prescaler register
        };

        public:
    };
}

#endif //SPI_H