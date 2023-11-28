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

    libs::Cout cout(usart2);

    //        sharedData.cout = &cout;

    shared::Data::setCout(&cout);

    shared::Data::getCout()->operator<<("Hello 1\n\r");

    drivers::usb::Usb usb(clockControl, usart2);
    usb.init();
    usb.start();

    shared::Data::getCout()->operator<<("Hello 3\n\r");

    std::uint8_t string[] = "Start STM32F407\n\r";

    while(1)
    {
        usart2.TransmitString(string, sizeof(string));
        clockControl.mDelay(1000);
    }
}