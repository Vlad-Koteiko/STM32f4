/*! \file gpio.hpp
    \brief class GPIO

    Header file with a description of the gpio class
*/

#ifndef GPIO_H
#define GPIO_H

#include "MWR.hpp"
#include "clockControl.hpp"

/// @brief Namespace for work with GPIO
namespace drivers::port
{
    /*! GPIOs addresses*/
    enum ADDRESSES_PORT : std::uintptr_t
    {
        PORTA = 0x40020000,    ///< address PORTA
        PORTB = 0x40020400,    ///< address PORTB
        PORTC = 0x40020800,    ///< address PORTC
        PORTD = 0x40020C00,    ///< address PORTD
        PORTE = 0x40021000,    ///< address PORTE
        PORTF = 0x40021400,    ///< address PORTF
        PORTG = 0x40021800,    ///< address PORTG
        PORTH = 0x40021C00,    ///< address PORTH
        PORTI = 0x40022000,    ///< address PORTI
        PORTJ = 0x40022400,    ///< address PORTJ
        PORTK = 0x40022800     ///< address PORTK
    };

    /*! Speeds GPIO pin*/
    enum PIN_SEED : std::uint8_t
    {
        LOW_SPEED       = 0b00000000,    ///< low speed
        MEDIOM_SPPED    = 0b00000001,    ///< medioum speed
        HIGH_SPEED      = 0b00000010,    ///< high speed
        VERY_HIGH_SPEED = 0b00000011     ///< very high speed
    };

    /*! Output type */
    enum OUTPUT_TYPE : std::uint8_t
    {
        PUSH_PULL  = 0,    ///< push-pull
        OPEN_DRAIN = 1     ///< open drain
    };

    /*! Type pull-up or pull-down*/
    enum TYPE_PUPDR : std::uint8_t
    {
        NO_PULL_UP_PULL_DOWN = 0,            ///< no pull-up and no pull-down
        PULL_UP              = 1,            ///< pull-up
        PULL_DOWN            = 0b00000010    ///< pull-down
    };

    /*! Alternative functions*/
    enum ALTERNATE_FUNCTION : std::uint8_t
    {
        AF0  = 0,             ///< AF0
        AF1  = 0b00000001,    ///< AF1
        AF2  = 0b00000010,    ///< AF2
        AF3  = 0b00000011,    ///< AF3
        AF4  = 0b00000100,    ///< AF4
        AF5  = 0b00000101,    ///< AF5
        AF6  = 0b00000110,    ///< AF6
        AF7  = 0b00000111,    ///< AF7
        AF8  = 0b00001000,    ///< AF8
        AF9  = 0b00001001,    ///< AF9
        AF10 = 0b00001010,    ///< AF10
        AF11 = 0b00001011,    ///< AF11
        AF12 = 0b00001100,    ///< AF12
        AF13 = 0b00001101,    ///< AF13
        AF14 = 0b00001110,    ///< AF14
        AF15 = 0b00001111,    ///< AF15
    };

    /*! Pin number GPIO*/
    enum PIN_NUMBER : std::uint8_t
    {
        PIN_0  = 0,     ///< pin 0
        PIN_1  = 1,     ///< pin 1
        PIN_2  = 2,     ///< pin 2
        PIN_3  = 3,     ///< pin 3
        PIN_4  = 4,     ///< pin 4
        PIN_5  = 5,     ///< pin 5
        PIN_6  = 6,     ///< pin 6
        PIN_7  = 7,     ///< pin 7
        PIN_8  = 8,     ///< pin 8
        PIN_9  = 9,     ///< pin 9
        PIN_10 = 10,    ///< pin 10
        PIN_11 = 11,    ///< pin 11
        PIN_12 = 12,    ///< pin 12
        PIN_13 = 13,    ///< pin 13
        PIN_14 = 14,    ///< pin 14
        PIN_15 = 15     ///< pin 15
    };

    /*! Port mode*/
    enum PORT_MODER : std::uint8_t
    {
        INPUT           = 0,    ///< input
        OUTPUT          = 1,    ///< output
        ALTERNATE_FUNCT = 2,    ///< alternative function
        ANALOG_MODE     = 3     ///< analog mode
    };

    /*! Statis pin*/
    enum STATUS_PIN : std::uint8_t
    {
        PIN_RESET,    ///< reset
        PIN_SET       ///< set
    };

    /// @brief Class for working with GPIO
    class GPIO
    {
        const drivers::clock::ClockControl& clockControl;    ///< Link to class ClockControl
        std::uintptr_t                      baseAddress;     ///< Base addres GPIO

        [[nodiscard]] static constexpr std::uint16_t pin(std::uint8_t numberPin)
        {
            return (1 << numberPin);
        }

        /*! Registers GPIO*/
        enum RegisterGPIO : std::uintptr_t
        {
            MODER    = 0x00,    ///< GPIO port mode register,               Address offset: 0x00
            OTYPER   = 0x04,    ///< GPIO port output type register,        Address offset: 0x04
            OSPEEDR  = 0x08,    ///< GPIO port output speed register,       Address offset: 0x08
            PUPDR    = 0x0C,    ///< GPIO port pull-up/pull-down register,  Address offset: 0x0C
            IDR      = 0x10,    ///< GPIO port input data register,         Address offset: 0x10
            ODR      = 0x14,    ///< GPIO port output data register,        Address offset: 0x14
            BSRR     = 0x18,    ///< GPIO port bit set/reset register,      Address offset: 0x18
            LCKR     = 0x1C,    ///< GPIO port configuration lock register, Address offset: 0x1C
            AFRLOW   = 0x20,    ///< GPIO port alternate function low reg,  Address offset: 0x20
            AFRLHIGH = 0x24,    ///< GPIO port alternate function high reg, Address offset: 0x24
        };

    public:
        /// @brief Constructor for GPIO
        /// @param clockControl Reference ClockControl
        /// @param port Address port
        GPIO(const drivers::clock::ClockControl& clockControl, ADDRESSES_PORT port);

        /// @brief Set pin speed
        /// @param numberPin Number pin
        /// @param pinSeed Pin speed
        void SetPinSpeed(PIN_NUMBER numberPin, PIN_SEED pinSeed) const noexcept;

        /// @brief Set pin output type
        /// @param numberPin Number pin
        /// @param outputType enum OUTPUT_TYPE
        void SetPinOutputType(PIN_NUMBER numberPin, OUTPUT_TYPE outputType) const noexcept;

        /// @brief Set pin pull
        /// @param numberPin Number pin
        /// @param typePupdr enum TYPE_PUPDR
        void SetPinPull(PIN_NUMBER numberPin, TYPE_PUPDR typePupdr) const noexcept;

        /// @brief Set alternative function
        /// @param numberPin Number pin
        /// @param alternateFunction
        void SetAFPin(PIN_NUMBER numberPin, ALTERNATE_FUNCTION alternateFunction) const noexcept;

        /// @brief Set pin mode
        /// @param numberPin Number pin
        /// @param portModer enum PORT_MODER
        void SetPinMode(PIN_NUMBER numberPin, PORT_MODER portModer) const noexcept;

        /// @brief Set output pin
        /// @param pinNumber Pin number
        void SetOutputPin(PIN_NUMBER pinNumber) const noexcept;

        /// @brief Reset output pin
        /// @param pinNumber Pin number
        void ResetOutputPin(PIN_NUMBER pinNumber) const noexcept;

        /// @brief Set pin
        /// @param pinNumber Pin number
        /// @param modePin Status pin
        void SetPin(PIN_NUMBER pinNumber, STATUS_PIN modePin) const noexcept;

        /// @brief Initialization pin
        /// @param pinNumber Pin number
        /// @param portModer Enum PORT_MODER
        void InitPin(PIN_NUMBER pinNumber, PORT_MODER portModer) const noexcept;

        /// @brief Toggle pin
        /// @param pinNumber Pin number
        void TogglePin(PIN_NUMBER pinNumber) const;

        /// @brief Deinit pin
        /// @param pinNumber Pin number
        void DeinitPin(PIN_NUMBER pinNumber) const;

        /// @brief Read pin
        /// @param pin Number pin
        /// @return 0 - Reset, 1 - Set
        std::uint8_t ReadPin(PIN_NUMBER pin) noexcept;

        /// @brief Read port
        /// @return Value pins port
        std::uint32_t ReadPort() noexcept;
    };

}    // namespace drivers::port
#endif    // UART_GPIO_H
