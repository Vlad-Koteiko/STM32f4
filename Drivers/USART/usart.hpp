/*! \file usart.hpp
    \brief class USART

    Header file with a description of the USART class
*/

#ifndef USART_H
#define USART_H

#include <tuple>

#include "MWR.hpp"
#include "base_interface.hpp"
#include "clockControl.hpp"
#include "dma.hpp"
#include "gpio.hpp"
#include "nvic.h"

namespace drivers::usart
{

    /*! UARTs addresses*/
    enum ADDRESSES_USART : std::uintptr_t
    {
        USART1 = 0x40011000,    ///< address USART1
        USART2 = 0x40004400,    ///< address USART2
        USART3 = 0x40004800,    ///< address USART3
        UART4  = 0x40004C00,    ///< address UART4
        UART5  = 0x40005000,    ///< address UART5
        USART6 = 0x40011400,    ///< address USART6
        UART7  = 0x40007800,    ///< address UART7
        UART8  = 0x40007C00     ///< address UART8
    };

    /*! Word length*/
    enum WORD_LENGTH : std::uint8_t
    {
        BIT_8 = 0,    ///< 8 bit
        BIT_9 = 1     ///< 9 bit
    };

    /*! Stop bit*/
    enum STOP_BIT : std::uint8_t
    {
        STOP_BIT_1   = 0,    ///< 1 stop bit
        STOP_BIT_0_5 = 1,    ///< 0.5 stop bit
        STOP_BIT_2   = 2,    ///< 2 stop bit
        STOP_BIT_1_5 = 3,    ///< 1.5 stop bit
    };

    /*! Baudrate*/
    enum BAUD_RATE : std::uint32_t
    {
        RATE_115200 = 115200,    ///< 115200
        RATE_9600   = 9600       ///< 9600
    };

    /*! Status*/
    enum STATUS : std::uint8_t
    {
        DISABLE,    ///< DISABLE = 0
        ENABLE      ///< ENABLE = 1
    };

    /*! Remap for USART1*/
    enum USART1_Remap : std::uint8_t
    {
        U1_TX_PA9_RX_PA10,    ///< Tx - PA9, Rx - PA10
        U1_TX_PA9_RX_PB7,     ///< Tx - PA9, Rx - PB7
        U1_TX_PB6_RX_PA10,    ///< Tx - PB6, Rx - PA10
        U1_TX_PB6_RX_PB7      ///< Tx - PB6, Rx - PB7
    };

    /*! Remap for USART2*/
    enum USART2_Remap : std::uint8_t
    {
        U2_TX_PA2_RX_PA3,    ///< Tx - PA2, Rx - PA3
        U2_TX_PA2_RX_PD6,    ///< Tx - PA2, Rx - PD6
        U2_TX_PD5_RX_PA3,    ///< Tx - PD5, Rx - PA3
        U2_TX_PD5_RX_PD6     ///< Tx - PD5, Rx - PD6
    };

    /*! Remap for USART3*/
    enum USART3_Remap : std::uint8_t
    {
        U3_TX_PB10_RX_PB11,    ///< Tx - PB10, Rx - PB11
        U3_TX_PB10_RX_PC11,    ///< Tx - PB10, Rx - PC11
        U3_TX_PB10_RX_PD9,     ///< Tx - PB10, Rx - PD9
        U3_TX_PC10_RX_PB11,    ///< Tx - PC10, Rx - PB11
        U3_TX_PC10_RX_PC11,    ///< Tx - PC10, Rx - PC11
        U3_TX_PC10_RX_PD9,     ///< Tx - PC10, Rx - PD9
        U3_TX_PD8_RX_PB11,     ///< Tx - PD8, Rx - PB11
        U3_TX_PD8_RX_PC11,     ///< Tx - PD8, Rx - PC11
        U3_TX_PD8_RX_PD9       ///< Tx - PD8, Rx - PD9
    };

    /*! Ramap for UART4*/
    enum UART4_Remap : std::uint8_t
    {
        U4_TX_PA0_RX_PA1,     ///< Tx - PA0, Rx - PA1
        U4_TX_PA0_RX_PC11,    ///< Tx - PA0, Rx - PC11
        U4_TX_PC10_RX_PA1,    ///< Tx - PC10, Rx - PA1
        U4_TX_PC10_RX_PC11    ///< Tx - PC10, Rx - PC11
    };

    /*! Remap for USART6*/
    enum USART6_Remap : std::uint8_t
    {
        U6_TX_PC6_RX_PC7,     ///< Tx - PC6, Rx - PC7
        U6_TX_PC6_RX_PG9,     ///< Tx - PC6, Rx - PG9
        U6_TX_PG14_RX_PC7,    ///< Tx - PG14, Rx - PC7
        U6_TX_PG14_RX_PG_9    ///< Tx - PG14, Rx - PG9
    };

    /*! Clear flags in status register*/
    enum SR_clear_flag : std::uint8_t
    {
        RXNE = 5,    ///< Flag RXNE
        TC   = 6,    ///< Flag TC
        LBD  = 8,    ///< Flag LBD
        CTS  = 9     ///< Flag CTS
    };

    /*! Enable/disable interrupt*/
    enum INTERRUPT : std::uint8_t
    {
        IDLEIE = 4,    ///< IDLE interrupt
        RXNEIE = 5,    ///< RXNE interrupt
        TCIE   = 6,    ///< Transmission complete interrupt
        TXEIE  = 7,    ///< TXE interrupt
        PEIE   = 8,    ///< PE interrupt
    };

    constexpr auto setInteget = [](BAUD_RATE     baudRate,
                                   std::uint32_t FPCLK) constexpr noexcept -> std::uint16_t {
        float         resalt  = (static_cast<float>(FPCLK) / static_cast<float>(16 * baudRate));
        std::uint16_t integet = static_cast<std::uint16_t>(resalt);
        return integet;
    };

    constexpr auto setFraction = [](BAUD_RATE     baudRate,
                                    std::uint32_t FPCLK) constexpr noexcept -> std::uint16_t {
        float         resalt   = (static_cast<float>(FPCLK) / static_cast<float>(16 * baudRate));
        std::uint16_t integet  = static_cast<std::uint16_t>(resalt);
        std::uint16_t fraction = (static_cast<float>(resalt - static_cast<float>(integet)) * 16);
        return fraction;
    };

    /// @brief Class for working with USART/UART
    class Usart : public base::BaseInterface
    {
        /*! Baud rate register*/
        enum BRR_poz
        {
            DIV_Fraction = 0,    ///< Fraction
            DIV_Mantissa = 4     ///< Mantissa
        };

        /*! Control register 1*/
        enum CR1_poz
        {
            SBK    = 0,     ///< Send break
            RWU    = 1,     ///< Receiver wakeup
            RE     = 2,     ///< Receiver enable
            TE     = 3,     ///< Transmitter enable
            IDLEIE = 4,     ///< IDLE interrupt enable
            RXNEIE = 5,     ///< RXNE interrupt enable
            TCIE   = 6,     ///< Transmission complete interrupt enable
            TXEIE  = 7,     ///< TXE interrupt enable
            PEIE   = 8,     ///< PE interrupt enable
            PS     = 9,     ///< Parity selection
            PCE    = 10,    ///< Parity control enable
            WAKE   = 11,    ///< Wakeup method
            M      = 12,    ///< Word length
            UE     = 13,    ///< USART enable
            OVER8  = 15     ///< Oversampling mode
        };

        /*! Control register 2*/
        enum CR2_poz
        {
            ADD   = 0,     ///< Address of the USART node
            LBDL  = 5,     ///< lin break detection length
            LBDIE = 6,     ///< LIN break detection interrupt enable
            LBCL  = 8,     ///< Last bit clock pulse
            CPHA  = 9,     ///< Clock phase
            CPOL  = 10,    ///< Clock polarity
            CLKEN = 11,    ///< Clock enable
            STOP  = 12,    ///< STOP bits
            LINEN = 14     ///< LIN mode enable
        };

        /*! Control register 3*/
        enum CR3_poz
        {
            EIE    = 0,     ///< Error interrupt enable
            IREN   = 1,     ///< IrDA mode enable
            IRLP   = 2,     ///< IrDA low-power
            HDSEL  = 3,     ///< Half-duplex selection
            NACK   = 4,     ///< Smartcard NACK enable
            SCEN   = 5,     ///< Smartcard mode enable
            DMAR   = 6,     ///< DMA enable receiver
            DMAT   = 7,     ///< DMA enable transmitter
            RTSE   = 8,     ///< RTS enable
            CTSE   = 9,     ///< CTS enable
            CTSIE  = 10,    ///< CTS interrupt enable
            ONEBIT = 11,    ///< One sample bit method enable
        };

        /*!  Guard time and prescaler register*/
        enum GTPR_poz
        {
            PSC = 0,    ///< Prescaler value
            GT  = 8     ///< Guard time value
        };

        using BaundRateType = std::tuple<std::uint16_t, std::uint16_t>;

        const drivers::clock::ClockControl &clockControl;    ///< Link to class ClockControl
        std::uintptr_t                      baseAddress;     ///< Base address USART/UART
        BaundRateType                       baundRateAPB2;
        BaundRateType                       baundRateAPB1;

        /*! Registers USART/UART*/
        enum RegisterUSART : std::ptrdiff_t
        {
            SR   = 0x00,    ///< USART Status register,                   Address offset: 0x00
            DR   = 0x04,    ///< USART Data register,                     Address offset: 0x04
            BRR  = 0x08,    ///< USART Baud rate register,                Address offset: 0x08
            CR1  = 0x0C,    ///< USART Control register 1,                Address offset: 0x0C
            CR2  = 0x10,    ///< USART Control register 2,                Address offset: 0x10
            CR3  = 0x14,    ///< USART Control register 3,                Address offset: 0x14
            GTPR = 0x18     ///< USART Guard time and prescaler register, Address offset: 0x18
        };

        /// @brief Remap for USART1
        /// @param remap All variants in enum USART1_Remap
        void RemapUsart1(USART1_Remap remap);

        /// @brief Remap for USART2
        /// @param remap All variants in enum USART2_Remap
        void RemapUsart2(USART2_Remap remap);

        /// @brief Remap for USART3
        /// @param remap All variants in enum USART3_Remap
        void RemapUsart3(USART3_Remap remap);

        /// @brief Remap for UART4
        /// @param remap All variants in enum UART4_Remap
        void RemapUart4(UART4_Remap remap);

        /// @brief Remap for USART6
        /// @param remap All variants in enum USART6_Remap
        void RemapUsart6(USART6_Remap remap);

    public:
        /*! Status register*/
        enum SR_poz
        {
            PE   = 0,    ///< Parity error
            FE   = 1,    ///< Framing error
            NF   = 2,    ///< Noise detected flag
            ORE  = 3,    ///< Overrun error
            IDLE = 4,    ///< IDLE line detected
            RXNE = 5,    ///< Read data register not empty
            TC   = 6,    ///< Transmission complete
            TXE  = 7,    ///< Transmit data register empty
            LBD  = 8,    ///< LIN break detection flag
            CTS  = 9     ///< CTS flag
        };

        /// @brief Constructor for USART/UART
        /// @param _clockControl Reference ClockControl
        /// @param adr Address USART/UART
        constexpr Usart(const drivers::clock::ClockControl &clockControlInit,
                        ADDRESSES_USART                     address) :
            clockControl(clockControlInit),
            baseAddress(address),
            baundRateAPB1(setInteget(RATE_115200, clockControlInit.GetFreqAPB1()),
                          setFraction(RATE_115200, clockControlInit.GetFreqAPB1())),
            baundRateAPB2(setInteget(RATE_115200, clockControlInit.GetFreqAPB2()),
                          setFraction(RATE_115200, clockControlInit.GetFreqAPB2()))
        {}

        void init() noexcept;

        /// @brief Remap pins for USART/UART
        /// @param remapPins enum USART{X}_Remap
        void Remap(uint8_t remapPins);

        /// @brief Configuration pins GPIO for USART/UART
        /// @param portTX Port GPIO TX pin
        /// @param portRX Port GPIO RX pin
        /// @param pinTX Pin number GPIO TX
        /// @param pinRX Pin number GPIO RX
        /// @param af Alternative function number
        void ConfigGpioForUart(drivers::port::ADDRESSES_PORT     portTX,
                               drivers::port::ADDRESSES_PORT     portRX,
                               drivers::port::PIN_NUMBER         pinTX,
                               drivers::port::PIN_NUMBER         pinRX,
                               drivers::port::ALTERNATE_FUNCTION af) noexcept;

        /// @brief This bit determines the word length. It is set or cleared by software.
        /// @param wordLength enum WORD_LENGTH
        void SetWordLength(WORD_LENGTH wordLength) noexcept;

        /// @brief This bit enables the receiver. It is set and cleared by software
        /// @param receiver enum STATUS
        void ReceiverEnable(STATUS receiver) noexcept;

        /// @brief This bit enables the transmitter. It is set and cleared by software
        /// @param transmitter enum STATUS
        void TransmitterEnable(STATUS transmitter) noexcept;

        /// @brief USART prescalers and outputs are stopped and the end of the current byte transfer
        /// in order to reduce power consumption
        /// @param usartEnable enum STATUS
        void UsartEnable(STATUS usartEnable) noexcept;

        /// @brief These bits are used for programming the stop bits.
        /// @param stopBit enum STATUS
        void SetStopBitsLength(STOP_BIT stopBit) noexcept;

        /// @brief Set baud rate USART/UART
        /// @param baudRate enum BAUD_RATE
        /// @param FPCLK Frequnce FPCLK
        void SetBaudRate(const BaundRateType &baundRate) noexcept;

        /// @brief Transmit data
        /// @param value Value for transmission
        //        void TransmitData(std::uint8_t value) noexcept;
        void sendByte(std::byte byte) noexcept override;

        /// @brief Trinsmit string
        /// @param value Value for transmission
        /// @param size Size value
        void TransmitString(const void *value, std::size_t size) noexcept;

        /// @brief Receive data
        /// @return Return value
        //        std::uint8_t ReceiveData() noexcept;
        std::byte readByte() noexcept override;

        /// @brief Deinit USART/UART
        void DeInit() noexcept;

        // SR

        /// @brief Read flags status register
        /// @param flag Name flag
        /// @return Сurrent flag value
        bool ReadFlag(SR_poz flag);

        /// @brief Clear flags status register
        /// @param flag Name flag
        void ClearFlag(SR_clear_flag flag);

        // CR1
        /// @brief Oversampling mode
        /// @param mode 0 - 16, 1 - 8
        void SetOversamplingMode(std::uint8_t mode);

        /// @brief Get oversampling mode
        /// @return true - 8, false - 16
        bool GetOversamplingMode();

        /// @brief Wakeup method
        /// @param x 0 - Idle Line, 1 -  Address Mark
        void SetWakeUpMethod(std::uint8_t x);

        /// @brief Get wakeup method
        /// @return false - Idle Line, true -  Address Mark
        bool GetWakeUpMethod();

        /// @brief Parity control enable
        /// @param x DISABLE - off, ENABLE - on
        void SetParityControl(STATUS x);

        /// @brief Get parity control enable
        /// @return false - off, true - on
        bool GetParityControl();

        /// @brief  Parity selection
        /// @param x 0 -  Even parity, 1 - Odd parity
        void SetParitySelection(std::uint8_t x);

        /// @brief Get parity selection
        /// @return false -  Even parity, true - Odd parity
        bool GetParitySelection();

        /// @brief Receiver wakeup
        /// @param x 0 - active mode, 1 - mute mode
        void ReceiverWakeup(std::uint8_t x);

        /// @brief Get receiver wakeup
        /// @return false - active mode, true - mute mode
        bool GetReceiverWakeup();

        /// @brief Send break
        /// @param x DISABLE - off, ENABLE - on
        void SendBreak(STATUS x);

        /// @brief Get send break
        /// @return false - off, true - on
        bool GetSendBreak();

        /// @brief Enable interrupt
        /// @param i enum INTERRUPT
        void EnableInterrupt(INTERRUPT i);

        /// @brief Disable interrupt
        /// @param i enum INTERRUPT
        void DisableInterrupt(INTERRUPT i);

        /// @brief Get source interrupt
        /// @param i enum INTERRUPT
        /// @return current status
        bool GetSourceInterrupt(INTERRUPT i);

        // CR2

        /// @brief LIN mode enable
        /// @param x enum STATUS
        void LinModeEnable(STATUS x);

        /// @brief Get LIN MODE enable
        /// @return false - disable, true - enable
        bool GetLinModeEnable();

        /// @brief Clock enable
        /// @param x enum STATUS
        void ClockEnable(STATUS x);

        /// @brief Get clock enable
        /// @return false - disable, true - enable
        bool GetClockEnable();

        /// @brief  Clock polarity
        /// @param x 0 - low, 1 - high
        void ClockPolarity(std::uint8_t x);

        /// @brief Get clock polarity
        /// @return 0 - low, 1 - high
        bool GetClockPolarity();

        /// @brief Clock phase
        /// @param x 0 - first, 1 - second
        void ClockPhase(std::uint8_t x);

        /// @brief Get clock phase
        /// @return 0 - first, 1 - second
        bool GetClockPhase();

        /// @brief  Last bit clock pulse
        /// @param x 0 - not output, 1 - output
        void LastBitClockPulse(std::uint8_t x);

        /// @brief Get last bit clock pulse
        /// @return 0 - not output, 1 - output
        bool GetLastBitClockPulse();

        /// @brief  LIN break detection interrupt enable
        /// @param x enum STATUS
        void SetLBDIE(STATUS x);

        /// @brief Get LIN break detection interrupt enable
        /// @return 0 - diable, 1 - enable
        bool GetLBDIE();

        /// @brief  Lin break detection length
        /// @param x 0 - 10bit, 1 - 11bit
        void LinBreakDetectionLength(std::uint8_t x);

        /// @brief Get Lin break detection length
        /// @return 0 - 10bit, 1 - 11bit
        bool GetLinBreakDetectionLength();

        /// @brief Address of the USART node
        /// @param x This bit-field gives the address of the USART node
        void SetAddressUsartNode(std::uint8_t x);

        /// @brief Get address of the USART node
        /// @return address
        std::uint8_t GetAddressUsartNode();

        // CR3

        /// @brief One sample bit method enable
        /// @param x 0 - three, 1 - one
        void OneSampleBitMethod(std::uint8_t x);

        /// @brief Get  one sample bit method enable
        /// @return 0 - three, 1 - one
        bool GetOneSampleBitMethod();

        /// @brief CTS interrupt enable
        /// @param x enum STATUS
        void SetCTSIE(STATUS x);

        /// @brief Get CTS interrupt enable
        /// @return false - disable, true - enable
        bool GetCTSIE();

        /// @brief CTS enable
        /// @param x enum STATUS
        void CTSEnable(STATUS x);

        /// @brief Get CTS enable
        /// @return false - disable, true - enable
        bool GetCTSEnable();

        /// @brief  RTS enable
        /// @param x enum STATUS
        void RTSEnable(STATUS x);

        /// @brief Get RTS enable
        /// @return false - disable, true - enable
        bool GetRTSEnable();

        /// @brief DMA enable transmitter
        /// @param x enum STATUS
        void DMAEnableTransmitter(STATUS x);

        /// @brief Get DMA enable transmitter
        /// @return false - disable, true - enable
        bool GetDMAEnableTransmitter();

        /// @brief DMA enable receiver
        /// @param x enum STATUS
        void DMAEnableReceiver(STATUS x);

        /// @brief Get DMA enable receiver
        /// @return false - disable, true - enable
        bool GetDMAEnableReceiver();

        /// @brief Smartcard mode enable
        /// @param x enum STATUS
        void SmartcardModeEnable(STATUS x);

        /// @brief Get Smartcard mode enable
        /// @return false - disable, true - enable
        bool GetSmartcardModeEnable();

        /// @brief Smartcard NACK enable
        /// @param x enum STATUS
        void SmartcardNACKEnable(STATUS x);

        /// @brief Get Smartcard NACK enable
        /// @return false - disable, true - enable
        bool GetSmartcardNACKEnable();

        /// @brief Half-duplex selection
        /// @param x 0 - Half duplex mode is not selected, 1 - Half duplex mode is selected
        void HalfDuplexSelection(std::uint8_t x);

        /// @brief Get Half-duplex selection
        /// @return 0 - Half duplex mode is not selected, 1 - Half duplex mode is selected
        bool GetHalfDuplexSelection();

        /// @brief  IrDA low-power
        /// @param s 0 - Normal mode, 1 - Low-power mode
        void IrDALowPower(std::uint8_t s);

        /// @brief Get IrDA low-power
        /// @return 0 - Normal mode, 1 - Low-power mode
        bool GetIrDALowPower();

        /// @brief IrDA mode enable
        /// @param x enum STATUS
        void IrDAModeEnable(STATUS x);

        /// @brief Get IrDA mode enable
        /// @return false - disable, true - enable
        bool GetIrDAModeEnable();

        /// @brief  Error interrupt enable
        /// @param x enum STATUS
        void SetEIE(STATUS x);

        /// @brief Get Error interrupt enable
        /// @return false - disable, true - enable
        bool GetEIE();

        /// @brief Init DMA for USART/UART
        /// @param usartDma Description DMA
        /// @param dmaConfig Configuration DMA
        /// @param isTransmit Enable transmit
        /// @param isReceive Enable receive
        /// @return Rezult opration
        bool InitUsartDma(const drivers::dma::DMA &usartDma,
                          drivers::dma::DMA_Config dmaConfig,
                          bool                     isTransmit,
                          bool                     isReceive);

        /// @brief Transmit DMA
        /// @param dma Description DMA
        /// @param sendBuffer Send buffer
        /// @param size Size buffer
        /// @return Rezult operation
        bool TransmitDataDma(dma::DMA &dma, std::uint8_t *sendBuffer, std::uint16_t size);

        /// @brief Receive DMA
        /// @param dma Description DMA
        /// @param recvBuffer Received buffer
        /// @param size Size buffer
        /// @return Rezult operation
        bool ReceiveDataDma(dma::DMA &dma, std::uint8_t *recvBuffer, std::uint16_t size);
    };    // end class
}    // namespace drivers::usart
#endif    // UART_USART_H
