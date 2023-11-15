//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.hpp"

#include "etl/string.h"
#include "etl/to_string.h"
#include "etl/vector.h"
#include "etl_profile.h"
#include "string.h"

int main() noexcept
{
    constexpr drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
    clockControl.init();
    drivers::usart::Usart usart2(clockControl, drivers::usart::USART2);
    usart2.init();

    std::uint8_t string[] = "Start STM32F407\n\r";

    etl::vector data { 0, 1, 2, 70, 70, 5, 6, 7, 8, 9 };

    etl::string<100> str;
    // etl::to_string<etl::string>(data, str);
    // usart2.TransmitString(&str[0], 10);

    usart2.sendByte(static_cast<std::byte>(data[4]));

    while(1)
    {
        usart2.TransmitString(string, sizeof(string));

        clockControl.mDelay(1000);
    }
}
