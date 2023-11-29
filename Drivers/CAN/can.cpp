
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

    bool Can::init(CanInit ci)
    {
        std::uint8_t waitAck    = 0;
        bool         statusInit = false;
        // exit from sleep mode
        libs::MWR::resetBit(CAN_MCR, SLEEP);

        // request initialisation
        libs::MWR::setBit(CAN_MCR, INRQ);

        // wait the acknowledge
        while((libs::MWR::readBit<std::uint32_t>(CAN_MSR, INAK) == false) || (waitAck < delayCan))
        {
            clockControl.mDelay(1);
            waitAck++;
        }

        if(libs::MWR::readBit<std::uint32_t>(CAN_MSR, INAK))
        {
            if(ci.timeTriggeredMode)
                libs::MWR::setBit(CAN_MCR, TTCM);
            else
                libs::MWR::resetBit(CAN_MCR, TTCM);

            if(ci.autoBusOff)
                libs::MWR::setBit(CAN_MCR, ABOM);
            else
                libs::MWR::resetBit(CAN_MCR, ABOM);

            if(ci.autoWakeUp)
                libs::MWR::setBit(CAN_MCR, AWUM);
            else
                libs::MWR::resetBit(CAN_MCR, AWUM);

            if(ci.autoRetransmission)
                libs::MWR::setBit(CAN_MCR, NART);
            else
                libs::MWR::resetBit(CAN_MCR, NART);

            if(ci.receiveFifoLocked)
                libs::MWR::setBit(CAN_MCR, RFLM);
            else
                libs::MWR::resetBit(CAN_MCR, RFLM);

            if(ci.transmitFifoPriority)
                libs::MWR::setBit(CAN_MCR, TXFP);
            else
                libs::MWR::resetBit(CAN_MCR, TXFP);

            libs::MWR::write_register(
                CAN_BTR,
                ((std::uint32_t)ci.mode << 30) | ((std::uint32_t)ci.syncJumpWidth << 24) |
                    ((std::uint32_t)ci.timeSeg1 << 16) | ((std::uint32_t)ci.timeSeg2 << 20) |
                    ((std::uint32_t)ci.prescaler - 1));


        }
        else
        {
            statusInit = false;
        }

        return statusInit;
    }
    
    void Can::deinit()
    {
        
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
        std::uint8_t waitAck    = 0;
        bool         statusInit = false;
        // request leave initialization
        libs::MWR::resetBit(CAN_MCR, INRQ);

        waitAck = 0;

        // wait the acknowledge
        while((libs::MWR::readBit<std::uint32_t>(CAN_MSR, INAK) == false) ||
                (waitAck < delayCan))
        {
            clockControl.mDelay(1);
            waitAck++;
        }

        if(libs::MWR::readBit<std::uint32_t>(CAN_MSR, INAK) == true)
            statusInit = false;
        else
            statusInit = true;

        return statusInit;
    }
    
    bool Can::stop()
    {
        std::uint8_t waitAck    = 0;
        bool         statusInit = false;

        // request initialization
        libs::MWR::setBit(CAN_MCR, INRQ);

        while ((libs::MWR::readBit<std::uint32_t>(CAN_MSR, INAK) == false) ||
                (waitAck < delayCan))
        {
            clockControl.mDelay(1);
            waitAck++;
        }

        // exit from sleep mode
        libs::MWR::resetBit(CAN_MCR, SLEEP);
        
    }

    // bool Can::stop()
    // {}
}    // namespace drivers::can