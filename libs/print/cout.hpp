//
// Created by koteiko_vv on 14.12.2022.
//
#ifndef COUT_H
#define COUT_H

#include <array>
#include <cstdint>
#include <string>

#include "usart.hpp"

namespace libs
{
    class Cout
    {
        std::uint8_t           endString = '\0';
        std::array<char, 11>   buff { 0 };
        drivers::usart::Usart& debugUart;

        inline void pritnString(char* ptr) noexcept
        {
            while(*ptr != endString)
            {
                printChar(*ptr);
                ptr++;
            }
        }

        inline void pritnValue() noexcept
        {
            for(std::size_t i = 0; i < buff.size();)
            {
                printChar(buff[i]);
                i++;
            }
        }

        void endlString() noexcept
        {
            printChar('\n');
            printChar('\r');
        }

    public:
        Cout(drivers::usart::Usart& msgUart);

        enum Commands
        {
            ENDL
        };

        Cout& operator<<(std::string string) noexcept;

        Cout& operator<<(std::uint32_t value) noexcept;

        Cout& operator<<(std::uint16_t value) noexcept;

        Cout& operator<<(std::uint8_t value) noexcept;

        Cout& operator<<(char string) noexcept;

        Cout& operator<<(Commands command) noexcept;

        inline void printChar(char ch) noexcept
        {
            while(!debugUart.ReadFlag(drivers::usart::Usart::SR_poz::TXE))
            {}
            debugUart.sendByte(static_cast<std::byte>(ch));
        }
    };
}    // namespace libs
#endif    // UART_COUT_H
