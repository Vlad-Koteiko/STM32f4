//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.hpp"

drivers::usart::Usart* usartGlobal;

int main() noexcept
{
    constexpr drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
    clockControl.init();
    drivers::usart::Usart usart2(clockControl, drivers::usart::USART2);
    usart2.init();
    usartGlobal = &usart2;

    std::uint8_t string[] = "Start STM32F407\n\r";

    drivers::can::Can can1(drivers::can::ADDRESSES_CAN::CAN1, clockControl);

    drivers::can::CanInitStruct ci {
        .prescaler            = 12,
        .mode                 = drivers::can::M_Normal,
        .syncJumpWidth        = 1,
        .timeSeg1             = 11,
        .timeSeg2             = 2,
        .timeTriggeredMode    = false,
        .autoBusOff           = false,
        .autoWakeUp           = false,
        .autoRetransmission   = false,
        .receiveFifoLocked    = false,
        .transmitFifoPriority = false,
    };

    can1.configGpioForCan(drivers::port::PORTD,
                          drivers::port::PORTD,
                          drivers::port::PIN_0,
                          drivers::port::PIN_1,
                          drivers::port::AF9);

    can1.init(ci);

    drivers::can::CanTxMsg ctm { .stdId = 0xcc,
                                 .extId = 0x00,
                                 .IDE   = 0x00,
                                 .RTR   = 0x00,
                                 .DLC   = 0x08,
                                 .data { 1, 2, 3, 4, 5, 6, 7, 8 } };

    // drivers::can::CanTxMsg ctm { .stdId = 0xcc,
    //                              .extId = 0x00,
    //                              .IDE   = 0x00,
    //                              .RTR   = 0x00,
    //                              .DLC   = 0x08,
    //                              .data { 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0xAA, 0xBB } };

    drivers::can::CanTxMsg ctm1 { .stdId = 0x00,
                                  .extId = 0x18FF2324,
                                  .IDE   = 0x01,
                                  .RTR   = 0x00,
                                  .DLC   = 0x08,
                                  .data { 1, 2, 3, 4, 5, 6, 7, 8 } };

    drivers::can::TxMailbox tmb;
    can1.start();

    can1.addTxMessage(ctm, &tmb);
    can1.addTxMessage(ctm1, &tmb);

    while(1)
    {
        usart2.TransmitString(string, sizeof(string));

        clockControl.mDelay(1000);
    }
}
