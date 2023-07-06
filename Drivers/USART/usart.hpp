/*! \file usart.hpp
    \brief class USART
    
    Header file with a description of the USART class
*/

#ifndef USART_H
#define USART_H

#include "MWR.hpp"
#include "clockControl.hpp"
#include "nvic.h"
#include "gpio.h"
#include "dma.hpp"

namespace drivers::usart {

    
    /*! UARTs addresses*/
    enum ADDRESSES_USART : std::uintptr_t
    {
        USART1 = 0x40011000, ///< address USART1
        USART2 = 0x40004400, ///< address USART2
        USART3 = 0x40004800, ///< address USART3
        UART4  = 0x40004C00, ///< address UART4
        UART5  = 0x40005000, ///< address UART5
        USART6 = 0x40011400, ///< address USART6
        UART7  = 0x40007800, ///< address UART7
        UART8  = 0x40007C00  ///< address UART8
    };

    /*! Word length*/
    enum WORD_LENGTH : std::uint8_t
    {
        BIT_8 = 0, ///< 8 bit
        BIT_9 = 1  ///< 9 bit
    };

    /*! Stop bit*/
    enum STOP_BIT : std::uint8_t
    {
        STOP_BIT_1    = 0,  ///< 1 stop bit
        STOP_BIT_0_5  = 1,  ///< 0.5 stop bit
        STOP_BIT_2    = 2,  ///< 2 stop bit
        STOP_BIT_1_5  = 3,  ///< 1.5 stop bit
    };

    /*! Baudrate*/
    enum BAUD_RATE : std::uint32_t
    {
        RATE_115200 = 115200,   ///< 115200
        RATE_9600   = 9600      ///< 9600
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
        U1_TX_PA9_RX_PA10,      ///< Tx - PA9, Rx - PA10
        U1_TX_PA9_RX_PB7,       ///< Tx - PA9, Rx - PB7
        U1_TX_PB6_RX_PA10,      ///< Tx - PB6, Rx - PA10
        U1_TX_PB6_RX_PB7        ///< Tx - PB6, Rx - PB7
    };

    /*! Remap for USART2*/
    enum USART2_Remap : std::uint8_t
    {
        U2_TX_PA2_RX_PA3,       ///< Tx - PA2, Rx - PA3
        U2_TX_PA2_RX_PD6,       ///< Tx - PA2, Rx - PD6
        U2_TX_PD5_RX_PA3,       ///< Tx - PD5, Rx - PA3
        U2_TX_PD5_RX_PD6        ///< Tx - PD5, Rx - PD6
    };

    /*! Remap for USART3*/
    enum USART3_Remap : std::uint8_t
    {
        U3_TX_PB10_RX_PB11,     ///< Tx - PB10, Rx - PB11
        U3_TX_PB10_RX_PC11,     ///< Tx - PB10, Rx - PC11
        U3_TX_PB10_RX_PD9,      ///< Tx - PB10, Rx - PD9
        U3_TX_PC10_RX_PB11,     ///< Tx - PC10, Rx - PB11
        U3_TX_PC10_RX_PC11,     ///< Tx - PC10, Rx - PC11
        U3_TX_PC10_RX_PD9,      ///< Tx - PC10, Rx - PD9
        U3_TX_PD8_RX_PB11,      ///< Tx - PD8, Rx - PB11
        U3_TX_PD8_RX_PC11,      ///< Tx - PD8, Rx - PC11
        U3_TX_PD8_RX_PD9        ///< Tx - PD8, Rx - PD9
    };

    /*! Ramap for UART4*/
    enum UART4_Remap : std::uint8_t
    {
        U4_TX_PA0_RX_PA1,       ///< Tx - PA0, Rx - PA1
        U4_TX_PA0_RX_PC11,      ///< Tx - PA0, Rx - PC11
        U4_TX_PC10_RX_PA1,      ///< Tx - PC10, Rx - PA1
        U4_TX_PC10_RX_PC11      ///< Tx - PC10, Rx - PC11
    };

    /*! Remap for USART6*/
    enum USART6_Remap : std::uint8_t
    {
        U6_TX_PC6_RX_PC7,       ///< Tx - PC6, Rx - PC7
        U6_TX_PC6_RX_PG9,       ///< Tx - PC6, Rx - PG9
        U6_TX_PG14_RX_PC7,      ///< Tx - PG14, Rx - PC7
        U6_TX_PG14_RX_PG_9      ///< Tx - PG14, Rx - PG9
    };

    /*! Clear flags in status register*/
    enum SR_clear_flag : std::uint8_t
    {
        RXNE = 5,       ///< Flag RXNE
        TC   = 6,       ///< Flag TC
        LBD  = 8,       ///< Flag LBD
        CTS  = 9        ///< Flag CTS
    };


    /*! Enable/disable interrupt*/
    enum INTERRUPT : std::uint8_t
    {
        IDLEIE = 4,     ///< IDLE interrupt
        RXNEIE = 5,     ///< RXNE interrupt
        TCIE   = 6,     ///< Transmission complete interrupt
        TXEIE  = 7,     ///< TXE interrupt
        PEIE   = 8,     ///< PE interrupt
    };


    /// @brief Class for working with USART/UART
    class USART
    {
        /*! Status register*/
        enum SR_poz
        {
            PE   = 0,   ///< Parity error
            FE   = 1,   ///< Framing error
            NF   = 2,   ///< Noise detected flag
            ORE  = 3,   ///< Overrun error
            IDLE = 4,   ///< IDLE line detected
            RXNE = 5,   ///< Read data register not empty
            TC   = 6,   ///< Transmission complete
            TXE  = 7,   ///< Transmit data register empty
            LBD  = 8,   ///< LIN break detection flag
            CTS  = 9    ///< CTS flag
        };

        /*! Baud rate register*/
        enum BRR_poz
        {
            DIV_Fraction = 0,   ///< Fraction
            DIV_Mantissa = 4    ///< Mantissa
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
            ADD   = 0,      ///< Address of the USART node
            LBDL  = 5,      ///< lin break detection length
            LBDIE = 6,      ///< LIN break detection interrupt enable
            LBCL  = 8,      ///< Last bit clock pulse
            CPHA  = 9,      ///< Clock phase
            CPOL  = 10,     ///< Clock polarity
            CLKEN = 11,     ///< Clock enable
            STOP  = 12,     ///< STOP bits
            LINEN = 14      ///< LIN mode enable
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

        drivers::clock::ClockControl &clockControl;     ///< Link to class ClockControl
        std::uintptr_t baseAddress;                     ///< Base address USART/UART

        /*! Registers USART/UART*/
        enum RegisterUSART : std::ptrdiff_t
        {
            SR    = 0x00,       ///< USART Status register,                   Address offset: 0x00
            DR    = 0x04,       ///< USART Data register,                     Address offset: 0x04
            BRR   = 0x08,       ///< USART Baud rate register,                Address offset: 0x08
            CR1   = 0x0C,       ///< USART Control register 1,                Address offset: 0x0C
            CR2   = 0x10,       ///< USART Control register 2,                Address offset: 0x10
            CR3   = 0x14,       ///< USART Control register 3,                Address offset: 0x14
            GTPR  = 0x18        ///< USART Guard time and prescaler register, Address offset: 0x18
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

        /// @brief Constructor for USART/UART !
        /// @param _clockControl Reference ClockControl !
        /// @param adr Address USART/UART !
        constexpr USART(drivers::clock::ClockControl &_clockControl, ADDRESSES_USART adr) : clockControl(_clockControl), baseAddress(adr)
        {
            switch (adr)
            {
                case USART1:
                {
                    clockControl.EnablePeripherals(drivers::clock::constants::USART1_MODULE);
                    RemapUsart1(U1_TX_PA9_RX_PA10);
                    SetBaudRate(RATE_115200, clockControl.GetFreqAPB2());
                    break;
                }
                case USART2:
                {
                    clockControl.EnablePeripherals(drivers::clock::constants::USART2_MODULE);
                    RemapUsart2(U2_TX_PA2_RX_PA3);
                    SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
                    break;
                }
                case USART3:
                {
                    clockControl.EnablePeripherals(drivers::clock::constants::USART3_MODULE);
                    RemapUsart3(U3_TX_PB10_RX_PB11);
                    SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
                    break;
                }
                case UART4:
                {
                    clockControl.EnablePeripherals(drivers::clock::constants::UART4_MODULE);
                    RemapUart4(U4_TX_PA0_RX_PA1);
                    SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
                    break;
                }

                case UART5:
                {
                    clockControl.EnablePeripherals(drivers::clock::constants::UART5_MODULE);
                    ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTD, drivers::port::PIN_12, drivers::port::PIN_2, drivers::port::AF8);
                    SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
                    break;
                }

                case USART6:
                {
                    clockControl.EnablePeripherals(drivers::clock::constants::USART6_MODULE);
                    RemapUsart6(U6_TX_PC6_RX_PC7);
                    SetBaudRate(RATE_115200, clockControl.GetFreqAPB2());
                    break;
                }
            }
            SetWordLength(BIT_8);
            ReceiverEnable(ENABLE);
            TransmitterEnable(ENABLE);
            UsartEnable(ENABLE);
        }

        void Remap(uint8_t remapPins);

        void ConfigGpioForUart(drivers::port::ADDRESSES_PORT portTX, drivers::port::ADDRESSES_PORT portRX, drivers::port::PIN_NUMBER pinTX, drivers::port::PIN_NUMBER pinRX, drivers::port::ALTERNATE_FUNCTION af) noexcept;
        void SetWordLength(WORD_LENGTH wordLength) noexcept;      // This bit determines the word length. It is set or cleared by software.
        void ReceiverEnable(STATUS receiver) noexcept;          // This bit enables the receiver. It is set and cleared by software
        void TransmitterEnable(STATUS transmitter) noexcept;  // This bit enables the transmitter. It is set and cleared by software
        void UsartEnable(STATUS usartEnable) noexcept;   // USART prescalers and outputs are stopped and the end of the current byte transfer in order to reduce power consumption
        void SetStopBitsLength(STOP_BIT stopBit) noexcept;      // These bits are used for programming the stop bits.
        void SetBaudRate(BAUD_RATE baudRate, std::uint32_t FPCLK) noexcept;
        void TransmitData(std::uint8_t value) noexcept;
        void TransmitString(void* value, std::size_t size) noexcept;
        std::uint8_t ReceiveData() noexcept;
        bool IsBusyFlag() noexcept;
        void DeInit() noexcept;

        //SR
        bool ReadFlag(SR_poz flag);
        void ClearFlag(SR_clear_flag flag);

        //CR1
        /**
         * Режим передискретизации
         * @param mode 0 - 16, 1 - 8
         */
        void SetOversamplingMode(std::uint8_t mode);
        bool GetOversamplingMode();
        /**
         * Способ пробуждения
         * @param x 0 - Линия холостого хода, 1 - Адресная метка
         */
        void SetWakeUpMethod(std::uint8_t x);
        bool GetWakeUpMethod();
        /**
         * Установка контроля четности
         * @param x DISABLE - выкл, ENABLE - вкл
         */
        void SetParityControl(STATUS x);
        bool GetParityControl();
        /**
         * Выбор четности
         * @param x 0 - четный, 1 - нечетный
         */
        void SetParitySelection(std::uint8_t x);
        bool GetParitySelection();
        /**
         * Пробуждение приемника
         * @param x 0 - активный, 1 - беззвучный
         */
        void ReceiverWakeup(std::uint8_t x);
        bool GetReceiverWakeup();
        /**
         * Отправить перерыв
         * @param x DISABLE - выкл, ENABLE - вкл
         */
        void SendBreak(STATUS x);
        bool GetSendBreak();

        void EnableInterrupt(INTERRUPT i);
        void DisableInterrupt(INTERRUPT i);
        bool GetSourceInterrupt(INTERRUPT i);

        //CR2
        void LinModeEnable(STATUS x);
        bool GetLinModeEnable();
        void ClockEnable(STATUS x);
        bool GetClockEnable();
        void ClockPolarity(std::uint8_t x);
        bool GetClockPolarity();
        void ClockPhase(std::uint8_t x);
        bool GetClockPhase();
        void LastBitClockPulse(std::uint8_t x);
        bool GetLastBitClockPulse();
        void SetLBDIE(STATUS x);
        bool GetLBDIE();
        void LinBreakDetectionLength(std::uint8_t x);
        bool GetLinBreakDetectionLength();
        void SetAddressUsartNode(std::uint8_t x);
        std::uint8_t GetAddressUsartNode();

        //CR3
        void OneSampleBitMethod(std::uint8_t x);
        bool GetOneSampleBitMethod();
        void SetCTSIE(STATUS x);
        bool GetCTSIE();
        void CTSEnable(STATUS x);
        bool GetCTSEnable();
        void RTSEnable(STATUS x);
        bool GetRTSEnable();
        void DMAEnableTransmitter(STATUS x);
        bool GetDMAEnableTransmitter();
        void DMAEnableReceiver(STATUS x);
        bool GetDMAEnableReceiver();
        void SmartcardModeEnable(STATUS x);
        bool GetSmartcardModeEnable();
        void SmartcardNACKEnable(STATUS x);
        bool GetSmartcardNACKEnable();
        void HalfDuplexSelection(std::uint8_t x);
        bool GetHalfDuplexSelection();
        void IrDALowPower(std::uint8_t s);
        bool GetIrDALowPower();
        void IrDAModeEnable(STATUS x);
        bool GetIrDAModeEnable();
        void SetEIE(STATUS x);
        bool GetEIE();

        bool InitUsartDma(const drivers::dma::DMA &usartDma, drivers::dma::DMA_Config dmaConfig, bool isTransmit, bool isReceive);
        bool TransmitDataDma(dma::DMA &dma, std::uint8_t *sendBuffer, std::uint16_t size);
        bool ReceiveDataDma(dma::DMA &dma, std::uint8_t *recvBuffer, std::uint16_t size);
    };
}
#endif //UART_USART_H
