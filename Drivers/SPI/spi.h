
#ifndef SPI_H
#define SPI_H

#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::spi
{
    enum ADDRESSES_SPI : std::uintptr_t
    {
        SPI1 = 0x40013000,
        SPI2 = 0x40003800,
        SPI3 = 0x40003C00,
        SPI4 = 0x40013400,
        SPI5 = 0x40015000,
        SPI6 = 0x40015400
    };

    enum CR1_poz: std::uint8_t
    {
        CPHA     = 0, // Clock phase
        CPOL     = 1, // Clock polarity
        MSTR     = 2, // Master selection
        BR       = 3, // Baud rate control [5:3]
        SPE      = 6, // SPI enable
        LSBFIRST = 7, // Frame format
        SSI      = 8, // Internal slave select
        SSM      = 9, // Software slave management
        RXONLY   = 10, // Receive only
        DFF      = 11, // Data frame format
        CRCNEXT  = 12, // CRC transfer next
        CRCEN    = 13, // Hardware CRC calculation enable
        BIDIOE   = 14, // Output enable in bidirectional mode
        BIDIMODE = 15, // Bidirectional data mode enable
    };

    enum CR2_poz : std::uint8_t
    {
        RXDMAEN = 0, // Rx buffer DMA enable
        TXDMAEN = 1, // Tx buffer DMA enable
        SSOE    = 2, // SS output enable
        FRF     = 4, // Frame format
        ERRIE   = 5, // Error interrupt enable
        RXNEIE  = 6, // RX buffer not empty interrupt enable
        TXEIE   = 7  // Tx buffer empty interrupt enable
    };

    enum SR_poz : std::uint8_t
    {
        RXNE   = 0, // Receive buffer not empty
        TXE    = 1, // Transmit buffer empty
        CHSIDE = 2, // Channel side
        UDR    = 3, // Underrun flag
        CRCERR = 4, // CRC error flag
        MODF   = 5, // Mode fault
        OVR    = 6, // Overrun flag
        BSY    = 7, // Busy flag
        FRE    = 8  // Frame format error
    };

    enum I2SCFGR_poz : std::uint8_t
    {
        CHLEN   = 0,  // Channel length (number of bits per audio channel)
        DATLEN  = 1,  // Data length to be transferred [2:1]
        CKPOL   = 3,  // Steady state clock polarity
        I2SSTD  = 4,  // I2S standard selection [5:4]
        PCMSYNC = 7,  // PCM frame synchronization
        I2SCFG  = 8,  // I2S configuration mode [9:8]
        I2SE    = 10, // I2S Enable
        I2SMOD  = 11  // I2S mode selection
    };

    enum I2SPR_poz : std::uint8_t
    {
        I2SDIV = 0, // I2S Linear prescaler [7:0]
        ODD    = 8, // Odd factor for the prescaler
        MCKOE  = 9  // Master clock output enable
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

    class SPI
    {
        std::uintptr_t baseAddress;
        drivers::clock::ClockControl &clock;

        enum RegistersSPI : std::uintptr_t
        {
            CR1     = 0x00, // Control register 1
            CR2     = 0x04, // Control register 2
            SR      = 0x08, // Status register
            DR      = 0x0C, // Data register
            CRCPR   = 0x10, // CRC polynomial register
            RXCRCR  = 0x14, // RX CRC register
            TXCRCR  = 0x18, // TX CRC register
            I2SCFGR = 0x1C, // I2S configuration register
            I2SPR   = 0x20  // I2S prescaler register
        };

    public:
        SPI(drivers::clock::ClockControl curClock, ADDRESSES_SPI spi);
        void Enable() noexcept;
        void Disable() noexcept;
        bool IsEnabled() noexcept;
        void SetMode(SPI_MODE m) noexcept;
        SPI_MODE GetMode() noexcept;
        void SetStandard(SPI_STANDART s) noexcept;
        SPI_STANDART GetStandard() noexcept;
        void SetClockPhase(CLOCK_PHASE cp) noexcept;
        CLOCK_PHASE GetClockPhase() noexcept;
        void SetClockPolarity(CLOCK_POLARITY cp) noexcept;
        CLOCK_POLARITY GetClockPolarity() noexcept;
        void SetBaudRatePrescaler(BAUD_RATE_PRESCALER brp) noexcept;
        BAUD_RATE_PRESCALER GetBaudRatePrescaler() noexcept;
        void SetTransferBitOrder(TRANSFER_BIT_ORDER tbo) noexcept;
        TRANSFER_BIT_ORDER GetTransferBitOrder() noexcept;
        void SetTransferDirection(TRANSFER_DIRECTION td) noexcept;
        TRANSFER_DIRECTION GetTransferDirection() noexcept;
        void SetDataWidth(DATA_WIDTH dw) noexcept;
        DATA_WIDTH GetDataWidth() noexcept;
        void EnableCRC() noexcept;
        void DisableCRC() noexcept;
        bool IsEnabledCRC() noexcept;
        void SetCRCNext() noexcept;
        void SetCRCPolynomial(std::uint16_t crcPoly) noexcept;
        std::uint16_t GetCRCPolynomial() noexcept;
        std::uint16_t GetRxCRC() noexcept;
        std::uint16_t GetTxCRC() noexcept;
        void SetNSSMode(NSS n) noexcept;
        NSS GetNSSMode() noexcept;
        bool IsActiveFlag_RXNE() noexcept;
        bool IsActiveFlag_TXE() noexcept;
        bool IsActiveFlag_CRCERR() noexcept;
        bool IsActiveFlag_MODF() noexcept;
        bool IsActiveFlag_OVR() noexcept;
        bool IsActiveFlag_BSY() noexcept;
        bool IsActiveFlag_FRE() noexcept;
        void ClearFlag_CRCERR() noexcept;
        void ClearFlag_MODF() noexcept;
        void ClearFlag_OVR() noexcept;
        void ClearFlag_FRE() noexcept;
        /**
         *  \brief Включить прерывание с ошибкой
         *  
         *  \return void
         */
        void EnableIT_ERR() noexcept;
        /**
         *  \brief Включить прерывание Rx-буфера, не являющееся пустым
         *  
         *  \return void
         */
        void EnableIT_RXNE() noexcept;
        /**
         *  \brief Включить прерывание с пустым буфером Tx
         *  
         *  \return void
         */
        void EnableIT_TXE() noexcept;
        /**
         *  \brief Отключить прерывание при ошибке
         *  
         *  \return void
         */
        void DisableIT_ERR() noexcept;
        /**
         *  \brief Отключить прерывание Rx-буфера, не являющееся пустым
         *  
         *  \return void
         */
        void DisableIT_RXNE() noexcept;
        /**
         *  \brief Отключить прерывание с пустым буфером Tx
         *  
         *  \return void
         */
        void DisableIT_TXE() noexcept;
        bool IsEnabledIT_ERR() noexcept;
        bool IsEnabledIT_RXNE() noexcept;
        bool IsEnabledIT_TXE() noexcept;
        
        void EnableDMAReq_RX() noexcept;
        void DisableDMAReq_RX() noexcept;
        bool IsEnabledDMAReq_RX() noexcept;
        void EnableDMAReq_TX() noexcept;
        void DisableDMAReq_TX() noexcept;
        bool IsEnabledDMAReq_TX() noexcept;
        
        std::uint8_t ReceiveData8() noexcept;
        std::uint16_t ReceiveData16() noexcept;
        void TransmitData8(std::uint8_t txData) noexcept;
        void TransmitData16(std::uint16_t txData) noexcept;
    };
}

#endif //SPI_H