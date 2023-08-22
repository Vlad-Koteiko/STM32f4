//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.hpp"

int main() noexcept
{
    constexpr drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
    clockControl.init();
    drivers::usart::Usart usart2(clockControl, drivers::usart::USART2);
    usart2.init();

    std::uint8_t string[] = "Start STM32F407\n\r";

    while(1)
    {
        usart2.TransmitString(string, sizeof(string));

        clockControl.mDelay(1000);
    }
}
