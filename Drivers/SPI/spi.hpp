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
/*! Example of a simple initialization of spi:<br>
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
    <br>
    Example of a simple initialization of spi with dma:<br>
    ...<br>
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
    <br>_
    _drivers::dma::DMA dma2(clockControl, drivers::dma::ADDRESSES_DMA::DMA_2);_<br>
    _dma2Pointer = &dma2;_<br>
    _drivers::dma::DMA_Config dmaConf;_<br>
    <br>
    _dmaConf.directModeErrorInterrupt             = drivers::dma::constants::DISABLE;_<br>
    _dmaConf.transferErrorInterrupt               = drivers::dma::constants::DISABLE;_<br>
    _dmaConf.halfTransferInterrupt                = drivers::dma::constants::DISABLE;_<br>
    _dmaConf.transferCompleteInterrupt            = drivers::dma::constants::ENABLE;_<br>
    _dmaConf.peripheralFlowController             = drivers::dma::constants::DISABLE;_<br>
    _dmaConf.circularMode                         = drivers::dma::constants::DISABLE;_<br>
    _dmaConf.priorityLevel                        = drivers::dma::constants::LOW;_<br>
    _dmaConf.doubleBufferMode                     = drivers::dma::constants::DISABLE;_<br>
    _dmaConf.currentTarget                        = drivers::dma::constants::MEMORY0;_<br>
    _dmaConf.peripheralBurstTransferConfiguration = drivers::dma::constants::SINGLE_TRANSFER;_<br>
    _dmaConf.memoryBurstTransferConfiguration     = drivers::dma::constants::SINGLE_TRANSFER;_<br>
    _dmaConf.FIFOThresholdSelection               = drivers::dma::constants::FIFO_0_25;_<br>
    _dmaConf.directModeDisableInvers              = drivers::dma::constants::ENABLE;_<br>
    _dmaConf.fifoErrorInterrupt                   = drivers::dma::constants::DISABLE;_<br>
    <br>
    _spi1.InitSpiDma(dma2, dmaConf, true, false, true);_<br>
    _nvic.NVIC_EnableIRQ(drivers::nvic::DMA2_Stream3);_<br>
    <br>
    ...<br>
    <br>
    _void DMA2_Stream3_IRQHandler()_<br>
     { <br>
        _globalCout->operator<<("stream3\n\r");_<br>
        _dma2Pointer->clearFlagInterrupt(drivers::dma::constants::Stream_3,
                                        drivers::dma::constants::TRANSFER_COMPLETE_INTERRUPT);_<br>
     }
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

    /// @brief Status
    enum STATUS : std::uint8_t
    {
        DISABLE = 0,    ///< Disable
        ENABLE  = 1     ///< Enable
    };

    /// @brief Spi mode
    enum SPI_MODE : std::uint8_t
    {
        SLAVE,    ///< Slave
        MASTER    ///< Master
    };

    /// @brief Spi standart
    enum SPI_STANDART : std::uint8_t
    {
        MOTOROLA,    ///< Motorola
        TI           ///< TI
    };

    /// @brief Clock phase
    enum CLOCK_PHASE : std::uint8_t
    {
        EDGE1,    ///< Edge 1
        EDGE2     ///< Edge 2
    };

    /// @brief Clock polarity
    enum CLOCK_POLARITY : std::uint8_t
    {
        LOW,    ///< Low
        HIGH    ///< High
    };

    /// @brief Baud rate prescaler
    enum BAUD_RATE_PRESCALER : std::uint8_t
    {
        DIV2,      ///< Div 2
        DIV4,      ///< Div 4
        DIV8,      ///< Div 8
        DIV16,     ///< Div 16
        DIV32,     ///< Div 32
        DIV64,     ///< Div 64
        DIV128,    ///< Div 128
        DIV256     ///< Div 256
    };

    /// @brief Transfer bit order
    enum TRANSFER_BIT_ORDER : std::uint8_t
    {
        MSB_FIRST,    ///< MSB first bit
        LSB_FIRST     ///< LSB first bit
    };

    /// @brief Transfer direction
    enum TRANSFER_DIRECTION : std::uint8_t
    {
        FULL_DUPLEX,       ///< Full duplex
        SIMPLEX_RX,        ///< Simple RX
        HALF_DUPLEX_RX,    ///< Half duplex RX
        HALF_DUPLEX_TX     ///< Half duplex TX
    };

    /// @brief Data width
    enum DATA_WIDTH : std::uint8_t
    {
        BIT8,    ///< Bit 8
        BIT16    ///< Bit 16
    };

    /// @brief Nss
    enum NSS : std::uint8_t
    {
        SOFT,          ///< Soft
        HARD_INPUT,    ///< Hard input
        HARD_OUTPUT    ///< Hard output
    };

    /// @brief Remap pins for SPI1 : SPI1_{MOSI}_{MISO}_{SCK}_{NSS}
    enum SPI1_Remap : std::uint8_t
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

    /// @brief Remap pins for SPI2 : SPI2_{MOSI}_{MISO}_{SCK}_{NSS}
    enum SPI2_Remap : std::uint8_t
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

    /// @brief Remap pins for SPI3 : SPI3_{MOSI}_{MISO}_{SCK}_{NSS}
    enum SPI3_Remap : std::uint8_t
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

    /// @brief Clear flag
    enum CLEAR_FLAG : std::uint8_t
    {
        CLF_CRCERR = 4,    ///< CRCERR
        CLF_MODF   = 5,    ///< MODF
        CLF_OVR    = 6,    ///< OVR
        CLF_FRE    = 8     ///< FRE
    };

    /// @brief Interrupt flag
    enum INT_FLAGS : std::uint8_t
    {
        I_ERRIE = 5,    ///< ERRIE
        I_RXNIE = 6,    ///< RXNIE
        I_TXEIE = 7     ///< TXEIE
    };

    /// @brief Class for working with SPI
    class SPI : public base::BaseInterface
    {
        std::uintptr_t                baseAddress;    ///< Base address spi
        drivers::clock::ClockControl &clock;          ///< Link clock control

        /// @brief Registers SPI
        enum RegistersSPI : std::uintptr_t
        {
            CR1     = 0x00,    ///< Control register 1
            CR2     = 0x04,    ///< Control register 2
            SR      = 0x08,    ///< Status register
            DR      = 0x0C,    ///< Data register
            CRCPR   = 0x10,    ///< CRC polynomial register
            RXCRCR  = 0x14,    ///< RX CRC register
            TXCRCR  = 0x18,    ///< TX CRC register
            I2SCFGR = 0x1C,    ///< I2S configuration register
            I2SPR   = 0x20     ///< I2S prescaler register
        };

        inline void ClearFlag_CRCERR() noexcept;
        inline void ClearFlag_MODF() noexcept;
        inline void ClearFlag_OVR() noexcept;
        inline void ClearFlag_FRE() noexcept;

    public:
        /// @brief Constructor SPI
        /// @param curClock Clock control
        /// @param spi Base address spi
        SPI(drivers::clock::ClockControl &curClock, ADDRESSES_SPI spi);

        /// @brief Constructor SPI
        /// @param curClock Clock control
        /// @param spi Base address spi
        /// @param remap Remap pins
        /// @param n Mode NSS
        SPI(drivers::clock::ClockControl &curClock, ADDRESSES_SPI spi, std::uint8_t remap, NSS n);

        /// @brief Configuration GPIO for SPI
        /// @param portMOSI Port MOSI
        /// @param pinMOSI  Pin MOSI
        /// @param portMISO Port MISO
        /// @param pinMISO Pin MISE
        /// @param portSCK Port SCK
        /// @param pinSCK Pin SCK
        /// @param portNSS Port NSS
        /// @param pinNSS Pin NSS
        /// @param af Altrernative function
        void ConfigGpioForSpi(drivers::port::ADDRESSES_PORT     portMOSI,
                              drivers::port::PIN_NUMBER         pinMOSI,
                              drivers::port::ADDRESSES_PORT     portMISO,
                              drivers::port::PIN_NUMBER         pinMISO,
                              drivers::port::ADDRESSES_PORT     portSCK,
                              drivers::port::PIN_NUMBER         pinSCK,
                              drivers::port::ADDRESSES_PORT     portNSS,
                              drivers::port::PIN_NUMBER         pinNSS,
                              drivers::port::ALTERNATE_FUNCTION af);

        /// @brief Configuration GPIO for SPI
        /// @param portMOSI Port MOSI
        /// @param pinMOSI  Pin MOSI
        /// @param portMISO Port MISO
        /// @param pinMISO Pin MISE
        /// @param portSCK Port SCK
        /// @param pinSCK Pin SCK
        /// @param af Altrernative function
        void ConfigGpioForSpi(drivers::port::ADDRESSES_PORT     portMOSI,
                              drivers::port::PIN_NUMBER         pinMOS,
                              drivers::port::ADDRESSES_PORT     portMISO,
                              drivers::port::PIN_NUMBER         pinMIS,
                              drivers::port::ADDRESSES_PORT     portSCK,
                              drivers::port::PIN_NUMBER         pinSCK,
                              drivers::port::ALTERNATE_FUNCTION af);

        /// @brief Remap pins for SPI1
        /// @param remap enum SPI1_Remap
        void RemapSPI1(std::uint8_t remap);

        /// @brief Remap pins for SPI1 with NSS
        /// @param remap enum SPI1_Remap
        void RemapSPI1Nss(std::uint8_t remap);

        /// @brief Remap pins for SPI2
        /// @param remap enum SPI2_Remap
        void RemapSPI2(std::uint8_t remap);

        /// @brief Remap pins for SPI2 with NSS
        /// @param remap enum SPI2_Remap
        void RemapSPI2Nss(std::uint8_t remap);

        /// @brief Remap pins for SPI3
        /// @param remap enum SPI3_Remap
        void RemapSPI3(std::uint8_t remap);

        /// @brief Remap pins for SPI3 with NSS
        /// @param remap enum SPI3_Remap
        void RemapSPI3Nss(std::uint8_t remap);

        /// @brief Enable SPI
        void Enable() noexcept;

        /// @brief Disble SPI
        void Disable() noexcept;

        /// @brief Is enabled SPI
        /// @return true - enable
        bool IsEnabled() noexcept;

        /// @brief Set mode SPI
        /// @param m mode
        void SetMode(SPI_MODE m) noexcept;

        /// @brief Get mode SPI
        /// @return mode
        SPI_MODE GetMode() noexcept;

        /// @brief Set SPI standart
        /// @param s stndart
        void SetStandard(SPI_STANDART s) noexcept;

        /// @brief Get SPI standart
        /// @return standart
        SPI_STANDART GetStandard() noexcept;

        /// @brief Set clock phase
        /// @param cp phase
        void SetClockPhase(CLOCK_PHASE cp) noexcept;

        /// @brief Get clock phase
        /// @return pahse
        CLOCK_PHASE GetClockPhase() noexcept;

        /// @brief Set clock polarity
        /// @param cp clock polarity
        void SetClockPolarity(CLOCK_POLARITY cp) noexcept;

        /// @brief Get clock polarity
        /// @return clock polarity
        CLOCK_POLARITY GetClockPolarity() noexcept;

        /// @brief Set baud rate prescaler
        /// @param brp prescaler
        void SetBaudRatePrescaler(BAUD_RATE_PRESCALER brp) noexcept;

        /// @brief Get baud rate presacaler
        /// @return prescaler
        BAUD_RATE_PRESCALER GetBaudRatePrescaler() noexcept;

        /// @brief Set trasfer bit order
        /// @param tbo bit order
        void SetTransferBitOrder(TRANSFER_BIT_ORDER tbo) noexcept;

        /// @brief Get transfer bit order
        /// @return bit order
        TRANSFER_BIT_ORDER GetTransferBitOrder() noexcept;

        /// @brief Set transfer direction
        /// @param td transfer direction
        void SetTransferDirection(TRANSFER_DIRECTION td) noexcept;

        /// @brief Get transfer direction
        /// @return transfer direction
        TRANSFER_DIRECTION GetTransferDirection() noexcept;

        /// @brief Set data width
        /// @param dw Data width
        void SetDataWidth(DATA_WIDTH dw) noexcept;

        /// @brief Get data width
        /// @return data width
        DATA_WIDTH GetDataWidth() noexcept;

        /// @brief Enable CRC
        void EnableCRC() noexcept;

        /// @brief Diasble CRC
        void DisableCRC() noexcept;

        /// @brief Is enabled CRC
        /// @return true - enable
        bool IsEnabledCRC() noexcept;

        /// @brief Set CRC next
        void SetCRCNext() noexcept;

        /// @brief Set CRC polynomial
        /// @param crcPoly polynomial
        void SetCRCPolynomial(std::uint16_t crcPoly) noexcept;

        /// @brief Get CRC polynomial
        /// @return polynomial
        std::uint16_t GetCRCPolynomial() noexcept;

        /// @brief Get RX CRC
        /// @return CRC
        std::uint16_t GetRxCRC() noexcept;

        /// @brief Get TX CRC
        /// @return CRC
        std::uint16_t GetTxCRC() noexcept;

        /// @brief Set NSS mode
        /// @param n mode
        void SetNSSMode(NSS n) noexcept;

        /// @brief Get NSS mode
        /// @return mode
        NSS GetNSSMode() noexcept;

        /// @brief Is active flag
        /// @param flag Flag name
        /// @return true - SET
        bool IsActiveFlag(SR_poz flag) noexcept;

        /// @brief Clear flag
        /// @param flag Flag name
        void ClearFlag(CLEAR_FLAG flag) noexcept;

        /// @brief Enable interrupt
        /// @param flag Flag name
        void EnableIT(INT_FLAGS flag);

        /// @brief Disable interrupt
        /// @param flag Flag name
        void DisableIT(INT_FLAGS flag);

        /// @brief Is enabled interrupt
        /// @param flag Flag name
        /// @return true - enable
        bool IsEnabledIT(INT_FLAGS flag);

        /// @brief Enable transmit DMA
        /// @param s status
        void DMAEnableTransmitter(STATUS s);

        /// @brief Is enabled transmit DMA
        /// @return true - enable
        bool IsEnbaledDMATransmitter();

        /// @brief Enable receive DMA
        /// @param s status
        void DMAEnableReceiver(STATUS s);

        /// @brief Is enabled received DMA
        /// @return true - enable
        bool IsEnabledDMAReceiver();

        /// @brief Send byte
        /// @param byte byte
        void sendByte(std::byte byte) noexcept override;

        /// @brief Read byte
        /// @return byte
        std::byte readByte() noexcept override;

        /// @brief Receive data 8 bit
        /// @return data
        std::uint8_t ReceiveData8() noexcept;

        /// @brief Receive data 16 bit
        /// @return data
        std::uint16_t ReceiveData16() noexcept;

        /// @brief Receiving an array of 8 bytes
        /// @param rxData data
        /// @param sizeArray number of bytes
        void ReceiveArray8(void *rxData, std::uint16_t sizeArray) noexcept;

        /// @brief Transmit data 8 bit
        /// @param txData data
        void TransmitData8(std::uint8_t txData) noexcept;

        /// @brief Transmit data 16 bit
        /// @param txData
        void TransmitData16(std::uint16_t txData) noexcept;

        /// @brief Sending an array of 8 bytes
        /// @param txData data
        /// @param sizeArray number of bytes
        void TransmitArray8(std::uint8_t *txData, std::uint32_t sizeArray) noexcept;

        /// @brief Receiving and transmitting an array
        /// @param txData sending data
        /// @param rxData received data
        /// @param size number of bytes
        void TransmitReceiveArray(std::uint8_t *txData, std::uint8_t *rxData, std::size_t size);

        /// @brief Receiving and transmitting an array
        /// @param txData sending data
        /// @param rxData received data
        /// @param size number of bytes
        void TransmitReceiveArray(void *txData, void *rxData, std::size_t size);

        /// @brief Init dma for spi
        /// @param spiDma Base address DMA
        /// @param dmaConfig Configuration DMA
        /// @param isTransmit is transmit
        /// @param isReceive is receive
        /// @param isDefault is default
        /// @return true - all OK
        bool InitSpiDma(const drivers::dma::DMA &spiDma,
                        drivers::dma::DMA_Config dmaConfig,
                        bool                     isTransmit,
                        bool                     isReceive,
                        bool                     isDefault);

        /// @brief Transmission via DMA
        /// @param dma Base address DMA
        /// @param stream Number stream
        /// @param sendBuffer Send buffer
        /// @param size Size
        /// @return true - All OK
        bool TransmitDataDma(dma::DMA                     &dma,
                             dma::constants::NUMBER_STREAM stream,
                             std::uint8_t                 *sendBuffer,
                             std::uint16_t                 size);

        /// @brief Received via DMA
        /// @param dma Base address DMA
        /// @param stream Number stream
        /// @param recvBuffer Recv buffer
        /// @param size Size
        /// @return true - All OK
        bool ReceiveDataDma(dma::DMA                     &dma,
                            dma::constants::NUMBER_STREAM stream,
                            std::uint8_t                 *recvBuffer,
                            std::uint16_t                 size);
    };
}    // namespace drivers::spi

#endif    // SPI_H