//
// Created by koteiko_vv on 23.01.2023.
//

#ifndef USART_H
#define USART_H

#include "MWR.hpp"
#include "clockControl.hpp"
#include "nvic.h"
#include "gpio.h"

namespace drivers::usart {

    enum ADDRESSES_USART : std::uintptr_t
    {
        USART1 = 0x40011000,
        USART2 = 0x40004400,
        USART3 = 0x40004800,
        UART4  = 0x40004C00,
        UART5  = 0x40005000,
        USART6 = 0x40011400,
        UART7  = 0x40007800,
        UART8  = 0x40007C00
    };

    enum WORD_LENGTH : std::uint8_t
    {
        BIT_8 = 0,
        BIT_9 = 1
    };

    enum RECEIVER : std::uint8_t
    {
        RECEIVER_OFF = 0,
        RECEIVER_ON  = 1
    };

    enum TRANSMITTER : std::uint8_t
    {
        TRANSMITTER_OFF = 0,
        TRANSMITTER_ON  = 1
    };

    enum USART_ENABLE : std::uint8_t
    {
        OFF = 0,
        ON  = 1
    };

    enum STOP_BIT : std::uint8_t
    {
        STOP_BIT_1    = 0,
        STOP_BIT_0_5  = 1,
        STOP_BIT_2    = 2,
        STOP_BIT_1_5  = 3,
    };

    enum BAUD_RATE : std::uint32_t
    {
        RATE_115200 = 115200,
        RATE_9600   = 9600
    };

    enum SR_poz
    {
        PE   = 0,   //Parity error
        FE   = 1,   //Framing error
        NF   = 2,   //Noise detected flag
        ORE  = 3,   //Overrun error
        IDLE = 4,   //IDLE line detected
        RXNE = 5,   //Read data register not empty
        TC   = 6,   //Transmission complete
        TXE  = 7,   //Transmit data register empty
        LBD  = 8,   //LIN break detection flag
        CTS  = 9    //CTS flag
    };

    enum BRR_poz
    {
        DIV_Fraction = 0,
        DIV_Mantissa = 4
    };

    enum CR1_poz
    {
        SBK    = 0,     //Send break
        RWU    = 1,     //Receiver wakeup
        RE     = 2,     //Receiver enable
        TE     = 3,     //Transmitter enable
        IDLEIE = 4,     //IDLE interrupt enable
        RXNEIE = 5,     //RXNE interrupt enable
        TCIE   = 6,     //Transmission complete interrupt enable
        TXEIE  = 7,     //TXE interrupt enable
        PEIE   = 8,     //PE interrupt enable
        PS     = 9,     //Parity selection
        PCE    = 10,    //Parity control enable
        WAKE   = 11,    //Wakeup method
        M      = 12,    //Word length
        UE     = 13,    //USART enable
        OVER8  = 15     //Oversampling mode
    };

    enum CR2_poz
    {
        ADD   = 0,      //Address of the USART node
        LBDL  = 5,      //lin break detection length
        LBDIE = 6,      //LIN break detection interrupt enable
        LBCL  = 8,      //Last bit clock pulse
        CPHA  = 9,      //Clock phase
        CPOL  = 10,     //Clock polarity
        CLKEN = 11,     //Clock enable
        STOP  = 12,     //STOP bits
        LINEN = 14      //LIN mode enable
    };

    enum CR3_poz
    {
        EIE    = 0,     // Error interrupt enable
        IREN   = 1,     //IrDA mode enable
        IRLP   = 2,     //IrDA low-power
        HDSEL  = 3,     //Half-duplex selection
        NACK   = 4,     //Smartcard NACK enable
        SCEN   = 5,     //Smartcard mode enable
        DMAR   = 6,     //DMA enable receiver
        DMAT   = 7,     //DMA enable transmitter
        RTSE   = 8,     //RTS enable
        CTSE   = 9,     //CTS enable
        CTSIE  = 10,    //CTS interrupt enable
        ONEBIT = 11,    //One sample bit method enable
    };

    enum GTPR_poz
    {
        PSC = 0,    //Prescaler value
        GT  = 8     //Guard time value
    };

    class USART
    {
        using readWrite = libs::MWR;
        drivers::clock::ClockControl &clockControl;
        std::uintptr_t baseAddress;

        enum RegisterUSART : std::uintptr_t
        {
            SR    = 0x00,       // USART Status register,                   Address offset: 0x00
            DR    = 0x04,       // USART Data register,                     Address offset: 0x04
            BRR   = 0x08,       // USART Baud rate register,                Address offset: 0x08
            CR1   = 0x0C,       // USART Control register 1,                Address offset: 0x0C
            CR2   = 0x10,       // USART Control register 2,                Address offset: 0x10
            CR3   = 0x14,       // USART Control register 3,                Address offset: 0x14
            GTPR  = 0x18        // USART Guard time and prescaler register, Address offset: 0x18
        };

    public:
        USART(drivers::clock::ClockControl &clockControl, ADDRESSES_USART adr);

        void DataWidth(WORD_LENGTH wordLength) noexcept  ;      // This bit determines the word length. It is set or cleared by software.

        void SetReceiver(RECEIVER receiver) noexcept ;          // This bit enables the receiver. It is set and cleared by software

        void SetTransmitter(TRANSMITTER transmitter) noexcept;  // This bit enables the transmitter. It is set and cleared by software


        void UsartEnable(USART_ENABLE usartEnable) noexcept;   // USART prescalers and outputs are stopped and the end of the current byte transfer in order to reduce power consumption

        void SetStopBitsLength(STOP_BIT stopBit) noexcept;      // These bits are used for programming the stop bits.

        void SetBaudRate(BAUD_RATE baudRate, std::uint32_t FPCLK);

        void TransmitData(std::uint8_t value) noexcept;

        void TransmitString(void* value, std::size_t size) noexcept;

        std::uint8_t ReceiveData() noexcept;

        bool IsBusyFlag() noexcept;

        void uartInit(BAUD_RATE baudRate,std::uint32_t FPCLK) noexcept;

        void EnableInterrupt();
    };
}
#endif //UART_USART_H
