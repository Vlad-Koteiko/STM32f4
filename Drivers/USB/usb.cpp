//
// Created by koteiko_vv on 21.02.2023.
//
#include "usb.hpp"

namespace drivers::usb
{
    void Usb::init() noexcept
    {
        clockControl.EnablePeripherals(drivers::clock::constants::USB_OTG_FS_MODULE);
        gpioInit();
        nvicEnable();
        coreInit();
        coreReset();

        cout << clockControl.getFreqAPB2();
    }

    void Usb::gpioInit() const noexcept
    {
        drivers::port::Gpio portA(clockControl, drivers::port::ADDRESSES_PORT::PORTA);
        portA.SetPinMode(drivers::port::PIN_11, drivers::port::ALTERNATE_FUNCT);
        portA.SetPinSpeed(drivers::port::PIN_11, drivers::port::VERY_HIGH_SPEED);
        portA.SetAFPin(drivers::port::PIN_11, drivers::port::ALTERNATE_FUNCTION::AF10);

        portA.SetPinMode(drivers::port::PIN_12, drivers::port::ALTERNATE_FUNCT);
        portA.SetPinSpeed(drivers::port::PIN_12, drivers::port::VERY_HIGH_SPEED);
        portA.SetAFPin(drivers::port::PIN_12, drivers::port::ALTERNATE_FUNCTION::AF10);
    }

    void Usb::nvicEnable() const noexcept
    {
        drivers::nvic::NVIC::NVIC_EnableIRQ(drivers::nvic::USB_OTG_FS);
    }

    void Usb::coreInit() noexcept
    {
        setBit(RegisterGlobal::GUSBCFG, 6);    // Full Speed serial transceiver select
    }

    void Usb::coreReset() noexcept
    {
        do    // Indicates that the AHB master state machine is in the Idle condition.
        {
        } while(readBit(GRSTCTL, 31) == 0);

        setBit(GRSTCTL, 0);    // Core soft reset

        do    // Core soft reset
        {
        } while(readBit(GRSTCTL, 0) == 1);
    }

}    // namespace drivers::usb