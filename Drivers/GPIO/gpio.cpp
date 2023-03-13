//
// Created by koteiko_vv on 16.01.2023.
//
#include "gpio.h"

namespace drivers::port
{

    GPIO::GPIO(const drivers::clock::ClockControl& clockControl, ADDRESSES_PORT port) : clockControl(clockControl)
    {
        baseAddress = port;
        switch (port) {
            case PORT_A:
            {
                clockControl.EnablePeripherals(drivers::clock::PORT_A_MODULE);
                break;
            }
            case PORT_B:
            {
                clockControl.EnablePeripherals(drivers::clock::PORT_B_MODULE);
            }
            case PORT_C:
            {
                clockControl.EnablePeripherals(drivers::clock::PORT_C_MODULE);
            }
            case PORT_D:
            {
                clockControl.EnablePeripherals(drivers::clock::PORT_D_MODULE);
                break;
            }
            case PORT_E:
            {
                clockControl.EnablePeripherals(drivers::clock::PORT_E_MODULE);
                break;
            }
            case PORT_H:
            {
                clockControl.EnablePeripherals(drivers::clock::PORT_H_MODULE);
                break;
            }
        }
    }

    void GPIO::SetPinSpeed(PIN_NUMBER numberPin, PIN_SEED pinSeed) const  noexcept
    {
        libs::MWR::modifySetRegister(baseAddress + OSPEEDR, pinSeed << (numberPin * 2));
    }

    void GPIO::SetPinOutputType(PIN_NUMBER numberPin, OUTPUT_TYPE outputType) const noexcept
    {
        libs::MWR::modifySetRegister(baseAddress + OTYPER,outputType << numberPin);
    }

    void GPIO::SetPinPull(PIN_NUMBER numberPin, TYPE_PUPDR typePupdr) const noexcept
    {
        libs::MWR::modifySetRegister(baseAddress + PUPDR, typePupdr << (numberPin*2));
    }

    void GPIO::SetAFPin(PIN_NUMBER numberPin,ALTERNATE_FUNCTION alternateFunction) const noexcept
    {

        if(numberPin <= 7)
        {
            libs::MWR::modifySetRegister(baseAddress + AFRLOW, alternateFunction << (numberPin * 4));

        } else
        {
            libs::MWR::modifySetRegister(baseAddress + AFRLHIGH,alternateFunction << ((numberPin - 8) * 4));
        }
    }

    void GPIO::SetPinMode(PIN_NUMBER numberPin, PORT_MODER portModer) const noexcept
    {
        libs::MWR::modifySetRegister(baseAddress + MODER, static_cast<std::uint32_t>((portModer << numberPin * 2)));
    }

    void  GPIO::SetOutputPin(PIN_NUMBER pinNumber) const noexcept
    {
        libs::MWR::modifySetRegister(baseAddress + BSRR,pin(pinNumber));
    }

    void GPIO::ResetOutputPin(PIN_NUMBER pinNumber) const noexcept
    {
        libs::MWR::modifySetRegister(baseAddress + BSRR, pin(pinNumber) << 16);
    }

    void GPIO::SetPin(PIN_NUMBER pinNumber, STATUS_PIN modePin) const noexcept
    {
        switch (modePin) {
            case PIN_SET:
            {
                SetOutputPin(pinNumber);
                break;
            }
            case PIN_RESET:
            {
                ResetOutputPin(pinNumber);
                break;
            }
        }
    }

    void GPIO::InitPin(PIN_NUMBER pinNumber,PORT_MODER portModer) const noexcept
    {
        SetPinMode(pinNumber,portModer);
        ResetOutputPin(pinNumber);
        SetPinSpeed(pinNumber,VERY_HIGH_SPEED);
        SetPinOutputType(pinNumber,PUSH_PULL);
        SetPinPull(pinNumber,NO_PULL_UP_PULL_DOWN);
    }

    void GPIO::TogglePin(PIN_NUMBER pinNumber) const
    {
        bool statusBit = libs::MWR::readBit<uint32_t>(baseAddress + ODR, pinNumber);
        if (statusBit)
            SetPin(pinNumber, PIN_RESET);
        else
            SetPin(pinNumber, PIN_SET);
    }

}




