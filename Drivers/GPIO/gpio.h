//
// Created by koteiko_vv on 16.01.2023.
//

#ifndef GPIO_H
#define GPIO_H

#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::port
{
    enum ADDRESSES_PORT : std::uintptr_t
    {
        PORT_A = 0x40020000,
        PORT_B = 0x40020400,
        PORT_C = 0x40020800,
        PORT_D = 0x40020C00,
        PORT_E = 0x40021000,
        PORT_F = 0x40021400,
        PORT_G = 0x40021800,
        PORT_H = 0x40021C00,
        PORT_I = 0x40022000,
    };

    template<ADDRESSES_PORT baseAddress>
    class GPIO
    {
        drivers::clock::ClockControl &clockControl;

        [[nodiscard]] std::uint16_t pin (std::uint8_t numberPin)
        {
          return (1 << numberPin);
        }

    public:

        enum RegisterGPIO : std::uintptr_t
        {
            MODER    = baseAddress,        // GPIO port mode register,               Address offset: 0x00
            OTYPER   = baseAddress + 0x04, // GPIO port output type register,        Address offset: 0x04
            OSPEEDR  = baseAddress + 0x08, // GPIO port output speed register,       Address offset: 0x08
            PUPDR    = baseAddress + 0x0C, // GPIO port pull-up/pull-down register,  Address offset: 0x0C
            IDR      = baseAddress + 0x10, // GPIO port input data register,         Address offset: 0x10
            ODR      = baseAddress + 0x14, // GPIO port output data register,        Address offset: 0x14
            BSRR     = baseAddress + 0x18, // GPIO port bit set/reset register,      Address offset: 0x18
            LCKR     = baseAddress + 0x1C, // GPIO port configuration lock register, Address offset: 0x1C
            AFRLOW   = baseAddress + 0x20, // GPIO port alternate function low reg,  Address offset: 0x20
            AFRLHIGH = baseAddress + 0x24, // GPIO port alternate function high reg, Address offset: 0x24
        };

        enum PIN_SEED : std::uint8_t
        {
            LOW_SPEED       = 0b00000000,
            MEDIOM_SPPED    = 0b00000001,
            HIGH_SPEED      = 0b00000010,
            VERY_HIGH_SPEED = 0b00000011
        };

        enum OUTPUT_TYPE : std::uint8_t
        {
            PUSH_PULL  = 0,
            OPEN_DRAIN = 1
        };

        enum TYPE_PUPDR : std::uint8_t
        {
            NO_PULL_UP_PULL_DOWN = 0,
            PULL_UP              = 1,
            PULL_DOWN            = 0b00000010
        };

        enum ALTERNATE_FUNCTION : std::uint8_t
        {
            AF0  = 0,
            AF1  = 0b00000001,
            AF2  = 0b00000010,
            AF3  = 0b00000011,
            AF4  = 0b00000100,
            AF5  = 0b00000101,
            AF6  = 0b00000110,
            AF7  = 0b00000111,
            AF8  = 0b00001000,
            AF9  = 0b00001001,
            AF10 = 0b00001010,
            AF11 = 0b00001011,
            AF12 = 0b00001100,
            AF13 = 0b00001101,
            AF14 = 0b00001110,
            AF15 = 0b00001111,
        };

        enum PIN_NUMBER : std::uint8_t
        {
            PIN_0  = 0,
            PIN_1  = 1,
            PIN_2  = 2,
            PIN_3  = 3,
            PIN_4  = 4,
            PIN_5  = 5,
            PIN_6  = 6,
            PIN_7  = 7,
            PIN_8  = 8,
            PIN_9  = 9,
            PIN_10 = 10,
            PIN_11 = 11,
            PIN_12 = 12,
            PIN_13 = 13,
            PIN_14 = 14,
            PIN_15 = 15
        };

        enum PORT_MODER : std::uint8_t
        {
            INPUT           = 0,
            OUTPUT          = 1,
            ALTERNATE_FUNCT = 2,
            ANALOG_MODE     = 3
        };

        enum MODE_PIN : std::uint8_t
        {
            PIN_NO,
            PIN_OFF
        };

        enum USART : std::uint8_t
        {
            USART_1,
            USART_2
        };

        GPIO(drivers::clock::ClockControl clockControl) : clockControl(clockControl)
        {
            switch (baseAddress) {
                case PORT_A:
                {
                    clockControl.module_enable(drivers::clock::PORT_A_MODULE);
                    break;
                }
                case PORT_D:
                {
                    clockControl.module_enable(drivers::clock::PORT_D_MODULE);
                    break;
                }
            }
        }

        void SetPinSpeed(PIN_NUMBER numberPin, PIN_SEED pinSeed) noexcept
        {
            libs::MWR::setBit(OSPEEDR, pinSeed << (numberPin * 2));
        }

        void SetPinOutputType(PIN_NUMBER numberPin, OUTPUT_TYPE outputType) noexcept
        {
            libs::MWR::setBit(OTYPER,outputType << numberPin);
        }

        void SetPinPull(PIN_NUMBER numberPin, TYPE_PUPDR typePupdr) noexcept
        {
            libs::MWR::setBit(PUPDR, typePupdr << (numberPin*2));
        }

        void SetAFPin(PIN_NUMBER numberPin,ALTERNATE_FUNCTION alternateFunction) noexcept
        {

            if(numberPin <= 7)
            {
                libs::MWR::setBit(AFRLOW, alternateFunction << (numberPin * 4));

            } else
            {
                libs::MWR::setBit(AFRLHIGH,alternateFunction << ((numberPin - 8) * 4));
            }
        }

        void SetPinMode(PIN_NUMBER numberPin, PORT_MODER portModer) noexcept
        {
            libs::MWR::setBit(MODER, static_cast<std::uint32_t>((portModer << numberPin * 2)));
        }

        void  SetOutputPin(PIN_NUMBER pinNumber) noexcept
        {
            libs::MWR::setBit(BSRR,pin(pinNumber));
        }

        void ResetOutputPin(PIN_NUMBER pinNumber) noexcept
        {
            libs::MWR::setBit(BSRR, pin(pinNumber) << 16);
        }

        void SetPin(PIN_NUMBER pinNumber, MODE_PIN modePin) noexcept
        {
            switch (modePin) {
                case PIN_NO:
                {
                    SetOutputPin(pinNumber);
                    break;
                }
                case PIN_OFF:
                {
                    ResetOutputPin(pinNumber);
                    break;
                }
            }
        }

        void USART_init(USART usart) noexcept
        {
            switch (usart) {
                case USART_1:
                {
                    SetPinMode(PIN_9,ALTERNATE_FUNCT);
                    SetPinMode(PIN_10,ALTERNATE_FUNCT);
                    SetPinSpeed(PIN_9,VERY_HIGH_SPEED);
                    SetPinSpeed(PIN_10,VERY_HIGH_SPEED);
                    SetAFPin(PIN_9,AF7);
                    SetAFPin(PIN_10,AF7);
                    break;
                }
                case USART_2:
                {
                    SetPinMode(PIN_2,ALTERNATE_FUNCT);
                    SetPinMode(PIN_3,ALTERNATE_FUNCT);
                    SetPinSpeed(PIN_2,VERY_HIGH_SPEED);
                    SetPinSpeed(PIN_3,VERY_HIGH_SPEED);
                    SetAFPin(PIN_2,AF7);
                    SetAFPin(PIN_3,AF7);
                    break;
                }
            }
        }

        void PIN_init(PIN_NUMBER pinNumber,PORT_MODER portModer) noexcept
        {
            SetPinMode(pinNumber,portModer);
            ResetOutputPin(pinNumber);
            SetPinSpeed(pinNumber,VERY_HIGH_SPEED);
            SetPinOutputType(pinNumber,PUSH_PULL);
            SetPinPull(pinNumber,NO_PULL_UP_PULL_DOWN);
        }
    };

}    // namespace drivers
#endif //UART_GPIO_H
