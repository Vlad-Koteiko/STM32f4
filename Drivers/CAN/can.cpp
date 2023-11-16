
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