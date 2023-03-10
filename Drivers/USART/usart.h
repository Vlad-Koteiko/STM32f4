//
// Created by koteiko_vv on 23.01.2023.
//

#ifndef USART_H
#define USART_H

#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::usart {

    enum ADDRESSES_USART : std::uintptr_t
    {
        USART_2 = 0x40004400
    };

    template<ADDRESSES_USART baseAddress>
    class USART
    {
        ClockControl &clockControl;

        enum RegisterGPIO : std::uintptr_t
        {
            SR    = baseAddress,        // USART Status register,                   Address offset: 0x00
            DR    = baseAddress + 0x04, // USART Data register,                     Address offset: 0x04
            BRR   = baseAddress + 0x08, // USART Baud rate register,                Address offset: 0x08
            CR1   = baseAddress + 0x0C, // USART Control register 1,                Address offset: 0x0C
            CR2   = baseAddress + 0x10, // USART Control register 2,                Address offset: 0x10
            CR3   = baseAddress + 0x14, // USART Control register 3,                Address offset: 0x14
            GTPR  = baseAddress + 0x18  // USART Guard time and prescaler register, Address offset: 0x18
        };
    public:
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
            USART_ENABLE_OFF = 0,
            USART_ENABLE_ON  = 1
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

        USART(ClockControl clockControl1) : clockControl(clockControl1)
        {
            switch (baseAddress) {
                case USART_2:
                {
                    clockControl.module_enable(clockControl.USART_2_MODULE);
                    break;
                }
            }

        }

        void DataWidth(WORD_LENGTH wordLength) noexcept        // This bit determines the word length. It is set or cleared by software.
        {
            libs::MWR::setBit(CR1, wordLength << 12);
        }

        void SetReceiver(RECEIVER receiver) noexcept           // This bit enables the receiver. It is set and cleared by software
        {
            libs::MWR::setBit(CR1, receiver << 2);
        }

        void SetTransmitter(TRANSMITTER transmitter) noexcept  // This bit enables the transmitter. It is set and cleared by software
        {
            libs::MWR::setBit(CR1, transmitter << 3);
        }

        void USART_ENABLE(USART_ENABLE usartEnable) noexcept   // USART prescalers and outputs are stopped and the end of the current byte transfer in order to reduce power consumption
        {
            libs::MWR::setBit(CR1, usartEnable << 13);
        }

        void SetStopBitsLength(STOP_BIT stopBit) noexcept      // These bits are used for programming the stop bits.
        {
            libs::MWR::setBit(CR2, stopBit << 12);
        }

        void SetBaudRate(BAUD_RATE baudRate, std::uint32_t FPCLK) noexcept
        {
            float resalt = (static_cast<float>(FPCLK)/static_cast<float>(16 * baudRate));
            std::uint16_t integet = static_cast<std::uint16_t>(resalt);
            std::uint16_t fraction = (static_cast<float>(resalt -  static_cast<float>(integet)) * 16);
            libs::MWR::write_register(BRR,(integet << 4) | (fraction & 0x000F));
        }

        inline void TransmitData (std::uint8_t value) noexcept
        {
            libs::MWR::write_register(DR,value);
        }

        bool IsBusyFlag() noexcept
        {
            return (libs::MWR::read_register<std::uint32_t>(SR) & (1 << 7));
        }

        void uartInit(BAUD_RATE baudRate,std::uint32_t FPCLK) noexcept
        {
            DataWidth(BIT_8);
            SetReceiver(RECEIVER_ON);
            SetTransmitter(TRANSMITTER_ON);
            SetBaudRate(baudRate,FPCLK);
            USART_ENABLE(USART_ENABLE_ON);
        }
    };
}
#endif //UART_USART_H
