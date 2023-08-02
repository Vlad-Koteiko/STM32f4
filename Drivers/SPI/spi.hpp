/*! \file spi.hpp
    \brief class SPI

    Header file with a description of the spi class
*/

#ifndef SPI_H
#define SPI_H

#include "MWR.hpp"
#include "base_interface.hpp"
#include "clockControl.hpp"
#include "dma.hpp"
#include "gpio.hpp"

/// @brief Namespace for work with SPI
/*! Example simple initalization spi:<br>
    _drivers::spi::SPI spi1(clockControl, drivers::spi::SPI1);_<br>
    _spi1.RemapSPI1(drivers::spi::SPI1_PA7_PA6_PA5_PA15);_<br>
    _spi1.SetClockPhase(drivers::spi::EDGE2);_<br>
    _spi1.SetClockPolarity(drivers::spi::HIGH);_<br>
    _spi1.SetMode(drivers::spi::MASTER);_<br>
    _spi1.SetBaudRatePrescaler(drivers::spi::DIV256);_<br>
    _spi1.SetTransferBitOrder(drivers::spi::MSB_FIRST);_<br>
    _spi1.SetTransferDirection(drivers::spi::FULL_DUPLEX);_<br>
    _spi1.SetDataWidth(drivers::spi::BIT8);_<br>
    _spi1.SetNSSMode(drivers::spi::HARD_OUTPUT);_<br>
    _spi1.SetStandard(drivers::spi::MOTOROLA);_<br>
    _spi1.Enable();_<br>
    _spi1.TransmitData8(0xAA);_<br>
*/

namespace drivers::spi
{


    /// @brief Base addresses SPI
    enum ADDRESSES_SPI : std::uintptr_t
    {
        SPI1 = 0x40013000,    ///< Base address SPI1
        SPI2 = 0x40003800,    ///< Base address SPI2
        SPI3 = 0x40003C00,    ///< Base address SPI3
        SPI4 = 0x40013400,    ///< Base address SPI4
        SPI5 = 0x40015000,    ///< Base address SPI5
        SPI6 = 0x40015400     ///< Base address SPI6
    };

    /// @brief Control register 1
    enum CR1_poz : std::uint8_t
    {
        CPHA     = 0,     ///< Clock phase
        CPOL     = 1,     ///< Clock polarity
        MSTR     = 2,     ///< Master selection
        BR       = 3,     ///< Baud rate control [5:3]
        SPE      = 6,     ///< SPI enable
        LSBFIRST = 7,     ///< Frame format
        SSI      = 8,     ///< Internal slave select
        SSM      = 9,     ///< Software slave management
        RXONLY   = 10,    ///< Receive only
        DFF      = 11,    ///< Data frame format
        CRCNEXT  = 12,    ///< CRC transfer next
        CRCEN    = 13,    ///< Hardware CRC calculation enable
        BIDIOE   = 14,    ///< Output enable in bidirectional mode
        BIDIMODE = 15,    ///< Bidirectional data mode enable
    };

    /// @brief Control register 2
    enum CR2_poz : std::uint8_t
    {
        RXDMAEN = 0,    ///< Rx buffer DMA enable
        TXDMAEN = 1,    ///< Tx buffer DMA enable
        SSOE    = 2,    ///< SS output enable
        FRF     = 4,    ///< Frame format
        ERRIE   = 5,    ///< Error interrupt enable
        RXNEIE  = 6,    ///< RX buffer not empty interrupt enable
        TXEIE   = 7     ///< Tx buffer empty interrupt enable
    };

    /// @brief Status register
    enum SR_poz : std::uint8_t
    {
        RXNE   = 0,    ///< Receive buffer not empty
        TXE    = 1,    ///< Transmit buffer empty
        CHSIDE = 2,    ///< Channel side
        UDR    = 3,    ///< Underrun flag
        CRCERR = 4,    ///< CRC error flag
        MODF   = 5,    ///< Mode fault
        OVR    = 6,    ///< Overrun flag
        BSY    = 7,    ///< Busy flag
        FRE    = 8     ///< Frame format error
    };

    /// @brief I2S configuration register
    enum I2SCFGR_poz : std::uint8_t
    {
        CHLEN   = 0,     ///< Channel length (number of bits per audio channel)
        DATLEN  = 1,     ///< Data length to be transferred [2:1]
        CKPOL   = 3,     ///< Steady state clock polarity
        I2SSTD  = 4,     ///< I2S standard selection [5:4]
        PCMSYNC = 7,     ///< PCM frame synchronization
        I2SCFG  = 8,     ///< I2S configuration mode [9:8]
        I2SE    = 10,    ///< I2S Enable
        I2SMOD  = 11     ///< I2S mode selection
    };

    /// @brief I2S prescaler register
    enum I2SPR_poz : std::uint8_t
    {
        I2SDIV = 0,    ///< I2S Linear prescaler [7:0]
        ODD    = 8,    ///< Odd factor for the prescaler
        MCKOE  = 9     ///< Master clock output enable
    };

    enum STATUS : std::uint8_t
    {
        DISABLE = 0,
        ENABLE  = 1
    };

    enum SPI_MODE : std::uint8_t
    {
        SLAVE,
        MASTER
    };

    enum SPI_STANDART : std::uint8_t
    {
        MOTOROLA,
        TI
    };

    enum CLOCK_PHASE : std::uint8_t
    {
        EDGE1,
        EDGE2
    };

    enum CLOCK_POLARITY : std::uint8_t
    {
        LOW,
        HIGH
    };

    enum BAUD_RATE_PRESCALER : std::uint8_t
    {
        DIV2,
        DIV4,
        DIV8,
        DIV16,
        DIV32,
        DIV64,
        DIV128,
        DIV256
    };

    enum TRANSFER_BIT_ORDER : std::uint8_t
    {
        MSB_FIRST,
        LSB_FIRST
    };

    enum TRANSFER_DIRECTION : std::uint8_t
    {
        FULL_DUPLEX,
        SIMPLEX_RX,
        HALF_DUPLEX_RX,
        HALF_DUPLEX_TX
    };

    enum DATA_WIDTH : std::uint8_t
    {
        BIT8,
        BIT16
    };

    enum NSS : std::uint8_t
    {
        SOFT,
        HARD_INPUT,
        HARD_OUTPUT
    };

    enum SPI1_Remap : std::uint8_t    // SPI1_{MOSI}_{MISO}_{SCK}_{NSS}
    {
        SPI1_PA7_PA6_PA5_PA4,
        SPI1_PA7_PA6_PA5_PA15,
        SPI1_PA7_PA6_PB3_PA4,
        SPI1_PA7_PA6_PB3_PA15,
        SPI1_PA7_PB4_PA5_PA4,
        SPI1_PA7_PB4_PA5_PA15,
        SPI1_PA7_PB4_PB3_PA4,
        SPI1_PA7_PB4_PB3_PA15,
        SPI1_PB5_PA6_PA5_PA4,
        SPI1_PB5_PA6_PA5_PA15,
        SPI1_PB5_PA6_PB3_PA4,
        SPI1_PB5_PA6_PB3_PA15,
        SPI1_PB5_PB4_PA5_PA4,
        SPI1_PB5_PB4_PA5_PA15,
        SPI1_PB5_PB4_PB3_PA4,
        SPI1_PB5_PB4_PB3_PA15
    };

    enum SPI2_Remap : std::uint8_t    // SPI2_{MOSI}_{MISO}_{SCK}_{NSS}
    {
        SPI2_PB15_PB14_PB10_PB9,
        SPI2_PB15_PB14_PB10_PB11,
        SPI2_PB15_PB14_PB12_PB9,
        SPI2_PB15_PB14_PB12_PB11,
        SPI2_PB15_PC2_PB10_PB9,
        SPI2_PB15_PC2_PB10_PB11,
        SPI2_PB15_PC2_PB12_PB9,
        SPI2_PB15_PC2_PB12_PB11,
        SPI2_PC3_PB14_PB10_PB9,
        SPI2_PC3_PB14_PB10_PB11,
        SPI2_PC3_PB14_PB12_PB9,
        SPI2_PC3_PB14_PB12_PB11,
        SPI2_PC3_PC2_PB10_PB9,
        SPI2_PC3_PC2_PB10_PB11,
        SPI2_PC3_PC2_PB12_PB9,
        SPI2_PC3_PC2_PB12_PB11
    };

    enum SPI3_Remap : std::uint8_t    // SPI_{MOSI}_{MISO}_{SCK}_{NSS}
    {
        SPI3_PB5_PB4_PB3_PA4,
        SPI3_PB5_PB4_PB3_PA15,
        SPI3_PB5_PB4_PC10_PA4,
        SPI3_PB5_PB4_PC10_PA15,
        SPI3_PB5_PC11_PB3_PA4,
        SPI3_PB5_PC11_PB3_PA15,
        SPI3_PB5_PC11_PC10_PA4,
        SPI3_PB5_PC11_PC10_PA15,
        SPI3_PC12_PB4_PB3_PA4,
        SPI3_PC12_PB4_PB3_PA15,
        SPI3_PC12_PB4_PC10_PA4,
        SPI3_PC12_PB4_PC10_PA15,
        SPI3_PC12_PC11_PB3_PA4,
        SPI3_PC12_PC11_PB3_PA15,
        SPI3_PC12_PC11_PC10_PA4,
        SPI3_PC12_PC11_PC10_PA15
    };

    enum CLEAR_FLAG : std::uint8_t
    {
        CLF_CRCERR = 4,
        CLF_MODF   = 5,
        CLF_OVR    = 6,
        CLF_FRE    = 8
    };

    enum INT_FLAGS : std::uint8_t
    {
        I_ERRIE = 5,
        I_RXNIE = 6,
        I_TXEIE = 7
    };

    class SPI : public base::BaseInterface
    {
        std::uintptr_t                baseAddress;
        drivers::clock::ClockControl &clock;

        enum RegistersSPI : std::uintptr_t
        {
            CR1     = 0x00,    // Control register 1
            CR2     = 0x04,    // Control register 2
            SR      = 0x08,    // Status register
            DR      = 0x0C,    // Data register
            CRCPR   = 0x10,    // CRC polynomial register
            RXCRCR  = 0x14,    // RX CRC register
            TXCRCR  = 0x18,    // TX CRC register
            I2SCFGR = 0x1C,    // I2S configuration register
            I2SPR   = 0x20     // I2S prescaler register
        };

        inline void ClearFlag_CRCERR() noexcept;
        inline void ClearFlag_MODF() noexcept;
        inline void ClearFlag_OVR() noexcept;
        inline void ClearFlag_FRE() noexcept;

    public:
        SPI(drivers::clock::ClockControl &curClock, ADDRESSES_SPI spi);
        SPI(drivers::clock::ClockControl &curClock, ADDRESSES_SPI spi, std::uint8_t remap, NSS n);

        void ConfigGpioForSpi(drivers::port::ADDRESSES_PORT     portMOSI,
                              drivers::port::PIN_NUMBER         pinMOSI,
                              drivers::port::ADDRESSES_PORT     portMISO,
                              drivers::port::PIN_NUMBER         pinMISO,
                              drivers::port::ADDRESSES_PORT     portSCK,
                              drivers::port::PIN_NUMBER         pinSCK,
                              drivers::port::ADDRESSES_PORT     portNSS,
                              drivers::port::PIN_NUMBER         pinNSS,
                              drivers::port::ALTERNATE_FUNCTION af);

        void ConfigGpioForSpi(drivers::port::ADDRESSES_PORT     portMOSI,
                              drivers::port::PIN_NUMBER         pinMOS,
                              drivers::port::ADDRESSES_PORT     portMISO,
                              drivers::port::PIN_NUMBER         pinMIS,
                              drivers::port::ADDRESSES_PORT     portSCK,
                              drivers::port::PIN_NUMBER         pinSCK,
                              drivers::port::ALTERNATE_FUNCTION af);

        void RemapSPI1(std::uint8_t remap);
        void RemapSPI1Nss(std::uint8_t remap);
        void RemapSPI2(std::uint8_t remap);
        void RemapSPI2Nss(std::uint8_t remap);
        void RemapSPI3(std::uint8_t remap);
        void RemapSPI3Nss(std::uint8_t remap);

        void                Enable() noexcept;
        void                Disable() noexcept;
        bool                IsEnabled() noexcept;
        void                SetMode(SPI_MODE m) noexcept;
        SPI_MODE            GetMode() noexcept;
        void                SetStandard(SPI_STANDART s) noexcept;
        SPI_STANDART        GetStandard() noexcept;
        void                SetClockPhase(CLOCK_PHASE cp) noexcept;
        CLOCK_PHASE         GetClockPhase() noexcept;
        void                SetClockPolarity(CLOCK_POLARITY cp) noexcept;
        CLOCK_POLARITY      GetClockPolarity() noexcept;
        void                SetBaudRatePrescaler(BAUD_RATE_PRESCALER brp) noexcept;
        BAUD_RATE_PRESCALER GetBaudRatePrescaler() noexcept;
        void                SetTransferBitOrder(TRANSFER_BIT_ORDER tbo) noexcept;
        TRANSFER_BIT_ORDER  GetTransferBitOrder() noexcept;
        void                SetTransferDirection(TRANSFER_DIRECTION td) noexcept;
        TRANSFER_DIRECTION  GetTransferDirection() noexcept;
        void                SetDataWidth(DATA_WIDTH dw) noexcept;
        DATA_WIDTH          GetDataWidth() noexcept;
        void                EnableCRC() noexcept;
        void                DisableCRC() noexcept;
        bool                IsEnabledCRC() noexcept;
        void                SetCRCNext() noexcept;
        void                SetCRCPolynomial(std::uint16_t crcPoly) noexcept;
        std::uint16_t       GetCRCPolynomial() noexcept;
        std::uint16_t       GetRxCRC() noexcept;
        std::uint16_t       GetTxCRC() noexcept;
        void                SetNSSMode(NSS n) noexcept;
        NSS                 GetNSSMode() noexcept;

        bool IsActiveFlag(SR_poz flag) noexcept;

        void ClearFlag(CLEAR_FLAG flag) noexcept;

        void EnableIT(INT_FLAGS flag);
        void DisableIT(INT_FLAGS flag);
        bool IsEnabledIT(INT_FLAGS flag);

        void DMAEnableTransmitter(STATUS s);
        bool IsEnbaledDMATransmitter();

        void DMAEnableReceiver(STATUS s);
        bool IsEnabledDMAReceiver();

        void          sendByte(std::byte byte) noexcept override;
        std::byte     readByte() noexcept override;
        std::uint8_t  ReceiveData8() noexcept;
        std::uint16_t ReceiveData16() noexcept;
        void          ReceiveArray8(void *rxData, std::uint16_t sizeArray) noexcept;
        void          TransmitData8(std::uint8_t txData) noexcept;
        void          TransmitData16(std::uint16_t txData) noexcept;
        void          TransmitArray8(std::uint8_t *txData, std::uint32_t sizeArray) noexcept;

        void TransmitReceiveArray(std::uint8_t *txData, std::uint8_t *rxData, std::size_t size);
        void TransmitReceiveArray(void *txData, void *rxData, std::size_t size);

        bool InitSpiDma(const drivers::dma::DMA &spiDma,
                        drivers::dma::DMA_Config dmaConfig,
                        bool                     isTransmit,
                        bool                     isReceive,
                        bool                     isDefault);

        bool TransmitDataDma(dma::DMA                     &dma,
                             dma::constants::NUMBER_STREAM stream,
                             std::uint8_t                 *sendBuffer,
                             std::uint16_t                 size);
        bool ReceiveDataDma(dma::DMA                     &dma,
                            dma::constants::NUMBER_STREAM stream,
                            std::uint8_t                 *recvBuffer,
                            std::uint16_t                 size);
    };
}    // namespace drivers::spi

#endif    // SPI_H