
#include "can.hpp"

namespace drivers::can
{
    Can::Can(ADDRESSES_CAN address, drivers::clock::ClockControl& clockCl) :
        baseAddress(address), clockControl(clockCl)
    {
        switch(address)
        {
            case CAN1:
                clockControl.EnablePeripherals(drivers::clock::constants::CAN1_MODULE);
                break;

            case CAN2:
                clockControl.EnablePeripherals(drivers::clock::constants::CAN2_MODULE);
                break;
        }
    }

    void Can::configGpioForCan(drivers::port::ADDRESSES_PORT     portH,
                               drivers::port::ADDRESSES_PORT     portL,
                               drivers::port::PIN_NUMBER         pinH,
                               drivers::port::PIN_NUMBER         pinL,
                               drivers::port::ALTERNATE_FUNCTION af)
    {
        // init GPIO CANH
        drivers::port::Gpio port_tx(clockControl, portH);
        port_tx.SetPinMode(pinH, drivers::port::ALTERNATE_FUNCT);
        port_tx.SetPinSpeed(pinH, drivers::port::VERY_HIGH_SPEED);
        port_tx.SetAFPin(pinH, af);

        // init GPIO CANL
        drivers::port::Gpio port_rx(clockControl, portL);
        port_rx.SetPinMode(pinL, drivers::port::ALTERNATE_FUNCT);
        port_rx.SetPinSpeed(pinL, drivers::port::VERY_HIGH_SPEED);
        port_rx.SetAFPin(pinL, af);
    }

    bool Can::start()
    {
        libs::MWR::resetBit(baseAddress + RegisterCAN::CAN_MCR, CAN_MCR::INRQ);

        while(libs::MWR::readBit<std::uint32_t>(baseAddress + RegisterCAN::CAN_MCR,
                                                CAN_MSR::INAK) != 0)
        {
        }

        return false;
    }

    // bool Can::stop()
    // {}
}    // namespace drivers::can