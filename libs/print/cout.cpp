//
// Created by koteiko_vv on 14.12.2022.
//
#include "cout.hpp"

namespace libs
{

    Cout::Cout(drivers::usart::Usart& msgUart) : debugUart(msgUart)
    {}

    Cout& Cout::operator<<(std::string string) noexcept
    {
        pritnString(string.data());
        return *this;
    }

    Cout& Cout::operator<<(std::size_t value) noexcept
    {
        std::fill_n(buff.begin(), 11, 0);
        std::size_t index = 10;
        do
        {
            index--;
            buff[index] = (value % 10 + '0');
            value /= 10;
        } while(value != 0);
        pritnValue();
        return *this;
    }

    Cout& Cout::operator<<(std::uint32_t value) noexcept
    {
        std::fill_n(buff.begin(), 11, 0);
        std::size_t index = 10;
        do
        {
            index--;
            buff[index] = (value % 10 + '0');
            value /= 10;
        } while(value != 0);
        pritnValue();
        return *this;
    }

    Cout& Cout::operator<<(std::uint16_t value) noexcept
    {
        std::fill_n(buff.begin(), 11, 0);
        std::size_t index = 10;
        do
        {
            index--;
            buff[index] = (value % 10 + '0');
            value /= 10;
        } while(value != 0);
        pritnValue();
        return *this;
    }

    Cout& Cout::operator<<(std::uint8_t value) noexcept
    {
        std::fill_n(buff.begin(), 11, 0);
        std::size_t index = 10;
        do
        {
            index--;
            buff[index] = (value % 10 + '0');
            value /= 10;
        } while(value != 0);
        pritnValue();
        return *this;
    }

    Cout& Cout::operator<<(char string) noexcept
    {
        printChar(string);
        return *this;
    }

    Cout& Cout::operator<<(Commands command) noexcept
    {
        if(command == ENDL)
        {
            endlString();
        }
        return *this;
    }

}    // namespace libs
