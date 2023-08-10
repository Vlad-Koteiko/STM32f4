//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.hpp"

#include "basictimer.hpp"
#include "cout.hpp"

void Bass::main() noexcept
{
    constexpr drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
    clockControl.init();
    drivers::usart::Usart usart2(clockControl, drivers::usart::USART2);
    usart2.init();
    libs::Cout cout(usart2);

    std::uint8_t string[] = "Start STM32F407\n\r";

    drivers::timers::basictimers::BasicTimer tim6(clockControl, drivers::timers::TIM6);

    tim6.setPrescaler(16800);
    tim6.setAutoReload(10000);

    tim6.enable();

    while(1)
    {
        usart2.TransmitString(string, sizeof(string));
        cout << tim6.getCounter();
        cout.ENDL;
        clockControl.mDelay(1000);
    }
}
