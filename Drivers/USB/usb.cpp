//
// Created by koteiko_vv on 21.02.2023.
//
#include "usb.h"

namespace drivers::usb {

    Usb::Usb(drivers::ClockControl &clockControl1) : clockControl(clockControl1) {}

    void Usb::Init() noexcept {
        drivers::port::GPIO<drivers::port::PORT_A> gpio(clockControl);

        gpio.SetPinMode(gpio.PIN_12,gpio.ALTERNATE_FUNCT);
        gpio.SetPinMode(gpio.PIN_11,gpio.ALTERNATE_FUNCT);
        gpio.SetPinSpeed(gpio.PIN_12,gpio.VERY_HIGH_SPEED);
        gpio.SetPinSpeed(gpio.PIN_11,gpio.VERY_HIGH_SPEED);
        gpio.SetAFPin(gpio.PIN_12,gpio.AF10);
        gpio.SetAFPin(gpio.PIN_11,gpio.AF10);

        clockControl.module_enable(ClockControl::USB_FS_MODULE);

    }

}