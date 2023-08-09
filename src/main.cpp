//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.hpp"

<<<<<<< HEAD
std::uint32_t counter      = 0;
std::uint8_t  bufferReceve = 0;

drivers::timers::BasicTimer *tim6Pointer;
drivers::usart::Usart       *uart_p_2;
libs::Cout                  *globalCout;

std::uint8_t recvDma[5];

=======
>>>>>>> b524d145bc0f081d8571a2f03410105f847e369d
void Bass::main() noexcept
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
