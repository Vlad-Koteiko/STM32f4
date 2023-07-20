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
        PORTA = 0x40020000,
        PORTB = 0x40020400,
        PORTC = 0x40020800,
        PORTD = 0x40020C00,
        PORTE = 0x40021000,
        PORTF = 0x40021400,
        PORTG = 0x40021800,
        PORTH = 0x40021C00,
        PORTI = 0x40022000,
        PORTJ = 0x40022400,
        PORTK = 0x40022800
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

    enum STATUS_PIN : std::uint8_t
    {
        PIN_RESET,
        PIN_SET
    };

    class GPIO
    {
        const drivers::clock::ClockControl& clockControl;
        std::uintptr_t                      baseAddress;

        [[nodiscard]] static constexpr std::uint16_t pin(std::uint8_t numberPin)
        {
            return (1 << numberPin);
        }

        enum RegisterGPIO : std::uintptr_t
        {
            MODER    = 0x00,    // GPIO port mode register,               Address offset: 0x00
            OTYPER   = 0x04,    // GPIO port output type register,        Address offset: 0x04
            OSPEEDR  = 0x08,    // GPIO port output speed register,       Address offset: 0x08
            PUPDR    = 0x0C,    // GPIO port pull-up/pull-down register,  Address offset: 0x0C
            IDR      = 0x10,    // GPIO port input data register,         Address offset: 0x10
            ODR      = 0x14,    // GPIO port output data register,        Address offset: 0x14
            BSRR     = 0x18,    // GPIO port bit set/reset register,      Address offset: 0x18
            LCKR     = 0x1C,    // GPIO port configuration lock register, Address offset: 0x1C
            AFRLOW   = 0x20,    // GPIO port alternate function low reg,  Address offset: 0x20
            AFRLHIGH = 0x24,    // GPIO port alternate function high reg, Address offset: 0x24
        };

    public:
        GPIO(const drivers::clock::ClockControl& clockControl, ADDRESSES_PORT port);
        void SetPinSpeed(PIN_NUMBER numberPin, PIN_SEED pinSeed) const noexcept;
        void SetPinOutputType(PIN_NUMBER numberPin, OUTPUT_TYPE outputType) const noexcept;
        void SetPinPull(PIN_NUMBER numberPin, TYPE_PUPDR typePupdr) const noexcept;
        void SetAFPin(PIN_NUMBER numberPin, ALTERNATE_FUNCTION alternateFunction) const noexcept;
        void SetPinMode(PIN_NUMBER numberPin, PORT_MODER portModer) const noexcept;
        void SetOutputPin(PIN_NUMBER pinNumber) const noexcept;
        void ResetOutputPin(PIN_NUMBER pinNumber) const noexcept;
        void SetPin(PIN_NUMBER pinNumber, STATUS_PIN modePin) const noexcept;
        void InitPin(PIN_NUMBER pinNumber, PORT_MODER portModer) const noexcept;
        void TogglePin(PIN_NUMBER pinNumber) const;
        void DeinitPin(PIN_NUMBER pinNumber) const;
        std::uint8_t  ReadPin(PIN_NUMBER pin) noexcept;
        std::uint32_t ReadPort() noexcept;
    };

}    // namespace drivers::port
#endif    // UART_GPIO_H
