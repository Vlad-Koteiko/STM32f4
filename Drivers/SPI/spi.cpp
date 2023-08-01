
#include "spi.hpp"

namespace drivers::spi
{
    SPI::SPI(drivers::clock::ClockControl curClock, ADDRESSES_SPI spi) :
        clock(curClock), baseAddress(spi)
    {
        switch(spi)
        {
            case SPI1:
                clock.EnablePeripherals(drivers::clock::constants::SPI1_MODULE);
                break;
            case SPI2:
                clock.EnablePeripherals(drivers::clock::constants::SPI2_MODULE);
                break;
            case SPI3:
                clock.EnablePeripherals(drivers::clock::constants::SPI3_MODULE);
                break;
            case SPI4:
                clock.EnablePeripherals(drivers::clock::constants::SPI4_MODULE);
                break;
            case SPI5:
                clock.EnablePeripherals(drivers::clock::constants::SPI5_MODULE);
                break;
            case SPI6:
                clock.EnablePeripherals(drivers::clock::constants::SPI6_MODULE);
                break;
        }
    }

    SPI::SPI(drivers::clock::ClockControl curClock, ADDRESSES_SPI spi, std::uint8_t remap, NSS n) :
        clock(curClock), baseAddress(spi)
    {
        switch(spi)
        {
            case SPI1:
                clock.EnablePeripherals(drivers::clock::constants::SPI1_MODULE);
                if(n == SOFT)
                {
                    RemapSPI1(remap);
                }
                else
                {
                    RemapSPI1Nss(remap);
                }
                break;
            case SPI2:
                clock.EnablePeripherals(drivers::clock::constants::SPI2_MODULE);
                if(n == SOFT)
                {
                    RemapSPI2(remap);
                }
                else
                {
                    RemapSPI2Nss(remap);
                }
                break;
            case SPI3:
                clock.EnablePeripherals(drivers::clock::constants::SPI3_MODULE);
                if(n == SOFT)
                {
                    RemapSPI3(remap);
                }
                else
                {
                    RemapSPI3Nss(remap);
                }
                break;
#ifdef STM32F_42x_43x
            case SPI4:
                break;
            case SPI5:
                break;
            case SPI6:
                break;
#endif
        }
    }

    void SPI::ConfigGpioForSpi(drivers::port::ADDRESSES_PORT     portMOSI,
                               drivers::port::PIN_NUMBER         pinMOSI,
                               drivers::port::ADDRESSES_PORT     portMISO,
                               drivers::port::PIN_NUMBER         pinMISO,
                               drivers::port::ADDRESSES_PORT     portSCK,
                               drivers::port::PIN_NUMBER         pinSCK,
                               drivers::port::ALTERNATE_FUNCTION af)
    {
        // MOSI
        drivers::port::Gpio port_mosi(clock, portMOSI);
        port_mosi.SetPinMode(pinMOSI, drivers::port::ALTERNATE_FUNCT);
        port_mosi.SetPinSpeed(pinMOSI, drivers::port::VERY_HIGH_SPEED);
        port_mosi.SetAFPin(pinMOSI, af);

        // MISO
        drivers::port::Gpio port_miso(clock, portMISO);
        port_miso.SetPinMode(pinMISO, drivers::port::ALTERNATE_FUNCT);
        port_miso.SetPinSpeed(pinMOSI, drivers::port::VERY_HIGH_SPEED);
        port_miso.SetAFPin(pinMISO, af);

        // SCK
        drivers::port::Gpio port_sck(clock, portSCK);
        port_sck.SetPinMode(pinSCK, drivers::port::ALTERNATE_FUNCT);
        port_sck.SetPinSpeed(pinSCK, drivers::port::VERY_HIGH_SPEED);
        port_sck.SetAFPin(pinSCK, af);
    }

    void SPI::ConfigGpioForSpi(drivers::port::ADDRESSES_PORT     portMOSI,
                               drivers::port::PIN_NUMBER         pinMOSI,
                               drivers::port::ADDRESSES_PORT     portMISO,
                               drivers::port::PIN_NUMBER         pinMISO,
                               drivers::port::ADDRESSES_PORT     portSCK,
                               drivers::port::PIN_NUMBER         pinSCK,
                               drivers::port::ADDRESSES_PORT     portNSS,
                               drivers::port::PIN_NUMBER         pinNSS,
                               drivers::port::ALTERNATE_FUNCTION af)
    {
        ConfigGpioForSpi(portMOSI, pinMOSI, portMISO, pinMISO, portSCK, pinSCK, af);

        // NSS
        drivers::port::Gpio port_nss(clock, portNSS);
        port_nss.SetPinMode(pinNSS, drivers::port::ALTERNATE_FUNCT);
        port_nss.SetPinSpeed(pinNSS, drivers::port::VERY_HIGH_SPEED);
        port_nss.SetAFPin(pinNSS, af);
    }

    void SPI::RemapSPI1(std::uint8_t remap)
    {
        switch(remap)
        {
            case SPI1_PA7_PA6_PA5_PA4:
            case SPI1_PA7_PA6_PA5_PA15:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::AF5);
                break;
            case SPI1_PA7_PA6_PB3_PA4:
            case SPI1_PA7_PA6_PB3_PA15:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::AF5);
                break;
            case SPI1_PA7_PB4_PA5_PA4:
            case SPI1_PA7_PB4_PA5_PA15:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::AF5);
                break;
            case SPI1_PA7_PB4_PB3_PA4:
            case SPI1_PA7_PB4_PB3_PA15:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::AF5);
                break;
            case SPI1_PB5_PA6_PA5_PA4:
            case SPI1_PB5_PA6_PA5_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::AF5);
                break;
            case SPI1_PB5_PA6_PB3_PA4:
            case SPI1_PB5_PA6_PB3_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::AF5);
                break;

            case SPI1_PB5_PB4_PA5_PA4:
            case SPI1_PB5_PB4_PA5_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::AF5);
                break;
            case SPI1_PB5_PB4_PB3_PA4:
            case SPI1_PB5_PB4_PB3_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::AF5);
                break;
        }
    }

    void SPI::RemapSPI1Nss(std::uint8_t remap)
    {
        switch(remap)
        {
            case SPI1_PA7_PA6_PA5_PA4:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI1_PA7_PA6_PA5_PA15:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI1_PA7_PA6_PB3_PA4:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI1_PA7_PA6_PB3_PA15:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI1_PA7_PB4_PA5_PA4:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI1_PA7_PB4_PA5_PA15:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI1_PA7_PB4_PB3_PA4:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI1_PA7_PB4_PB3_PA15:
                ConfigGpioForSpi(port::PORTA,
                                 port::PIN_7,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI1_PB5_PA6_PA5_PA4:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI1_PB5_PA6_PA5_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI1_PB5_PA6_PB3_PA4:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI1_PB5_PA6_PB3_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_6,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI1_PB5_PB4_PA5_PA4:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI1_PB5_PB4_PA5_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTA,
                                 port::PIN_5,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;

            case SPI1_PB5_PB4_PB3_PA4:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI1_PB5_PB4_PB3_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
        }
    }

    void SPI::RemapSPI2(std::uint8_t remap)
    {
        switch(remap)
        {
            case SPI2_PB15_PB14_PB10_PB9:
            case SPI2_PB15_PB14_PB10_PB11:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::AF5);
                break;

            case SPI2_PB15_PB14_PB12_PB9:
            case SPI2_PB15_PB14_PB12_PB11:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::AF5);
                break;

            case SPI2_PB15_PC2_PB10_PB9:
            case SPI2_PB15_PC2_PB10_PB11:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::AF5);
                break;

            case SPI2_PB15_PC2_PB12_PB9:
            case SPI2_PB15_PC2_PB12_PB11:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::AF5);
                break;

            case SPI2_PC3_PB14_PB10_PB9:
            case SPI2_PC3_PB14_PB10_PB11:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::AF5);
                break;

            case SPI2_PC3_PB14_PB12_PB9:
            case SPI2_PC3_PB14_PB12_PB11:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::AF5);
                break;

            case SPI2_PC3_PC2_PB10_PB9:
            case SPI2_PC3_PC2_PB10_PB11:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::AF5);
                break;

            case SPI2_PC3_PC2_PB12_PB9:
            case SPI2_PC3_PC2_PB12_PB11:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::AF5);
                break;

            default:
                break;
        }
    }

    void SPI::RemapSPI2Nss(std::uint8_t remap)
    {
        switch(remap)
        {
            case SPI2_PB15_PB14_PB10_PB9:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::PORTB,
                                 port::PIN_9,
                                 port::AF5);
                break;
            case SPI2_PB15_PB14_PB10_PB11:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::PORTB,
                                 port::PIN_11,
                                 port::AF5);
                break;
            case SPI2_PB15_PB14_PB12_PB9:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_9,
                                 port::AF5);
                break;
            case SPI2_PB15_PB14_PB12_PB11:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_11,
                                 port::AF5);
                break;
            case SPI2_PB15_PC2_PB10_PB9:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::PORTB,
                                 port::PIN_9,
                                 port::AF5);
                break;
            case SPI2_PB15_PC2_PB10_PB11:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::PORTB,
                                 port::PIN_11,
                                 port::AF5);
                break;
            case SPI2_PB15_PC2_PB12_PB9:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_9,
                                 port::AF5);
                break;
            case SPI2_PB15_PC2_PB12_PB11:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_15,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_11,
                                 port::AF5);
                break;
            case SPI2_PC3_PB14_PB10_PB9:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::AF5);
                break;
            case SPI2_PC3_PB14_PB10_PB11:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::PORTB,
                                 port::PIN_11,
                                 port::AF5);
                break;
            case SPI2_PC3_PB14_PB12_PB9:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_9,
                                 port::AF5);
                break;
            case SPI2_PC3_PB14_PB12_PB11:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTB,
                                 port::PIN_14,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_11,
                                 port::AF5);
                break;
            case SPI2_PC3_PC2_PB10_PB9:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::PORTB,
                                 port::PIN_9,
                                 port::AF5);
                break;
            case SPI2_PC3_PC2_PB10_PB11:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_10,
                                 port::PORTB,
                                 port::PIN_11,
                                 port::AF5);
                break;
            case SPI2_PC3_PC2_PB12_PB9:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_9,
                                 port::AF5);
                break;
            case SPI2_PC3_PC2_PB12_PB11:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_3,
                                 port::PORTC,
                                 port::PIN_2,
                                 port::PORTB,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_11,
                                 port::AF5);
                break;

            default:

                break;
        }
    }

    void SPI::RemapSPI3(std::uint8_t remap)
    {
        switch(remap)
        {
            case SPI3_PB5_PB4_PB3_PA4:
            case SPI3_PB5_PB4_PB3_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::AF5);
                break;

            case SPI3_PB5_PB4_PC10_PA4:
            case SPI3_PB5_PB4_PC10_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::AF5);
                break;

            case SPI3_PB5_PC11_PB3_PA4:
            case SPI3_PB5_PC11_PB3_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::AF5);
                break;

            case SPI3_PB5_PC11_PC10_PA4:
            case SPI3_PB5_PC11_PC10_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::AF5);
                break;

            case SPI3_PC12_PB4_PB3_PA4:
            case SPI3_PC12_PB4_PB3_PA15:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::AF5);
                break;

            case SPI3_PC12_PB4_PC10_PA4:
            case SPI3_PC12_PB4_PC10_PA15:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::AF5);
                break;

            case SPI3_PC12_PC11_PB3_PA4:
            case SPI3_PC12_PC11_PB3_PA15:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::AF5);
                break;

            case SPI3_PC12_PC11_PC10_PA4:
            case SPI3_PC12_PC11_PC10_PA15:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::AF5);
                break;

            default:
                break;
        }
    }

    void SPI::RemapSPI3Nss(std::uint8_t remap)
    {
        switch(remap)
        {
            case SPI3_PB5_PB4_PB3_PA4:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI3_PB5_PB4_PB3_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI3_PB5_PB4_PC10_PA4:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI3_PB5_PB4_PC10_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI3_PB5_PC11_PB3_PA4:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI3_PB5_PC11_PB3_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI3_PB5_PC11_PC10_PA4:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI3_PB5_PC11_PC10_PA15:
                ConfigGpioForSpi(port::PORTB,
                                 port::PIN_5,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI3_PC12_PB4_PB3_PA4:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI3_PC12_PB4_PB3_PA15:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI3_PC12_PB4_PC10_PA4:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI3_PC12_PB4_PC10_PA15:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTB,
                                 port::PIN_4,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI3_PC12_PC11_PB3_PA4:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI3_PC12_PC11_PB3_PA15:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTB,
                                 port::PIN_3,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
            case SPI3_PC12_PC11_PC10_PA4:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::PORTA,
                                 port::PIN_4,
                                 port::AF5);
                break;
            case SPI3_PC12_PC11_PC10_PA15:
                ConfigGpioForSpi(port::PORTC,
                                 port::PIN_12,
                                 port::PORTC,
                                 port::PIN_11,
                                 port::PORTC,
                                 port::PIN_10,
                                 port::PORTA,
                                 port::PIN_15,
                                 port::AF5);
                break;
        }
    }

    void SPI::Enable() noexcept
    {
        libs::MWR::setBit(baseAddress + CR1, SPE);
    }

    void SPI::Disable() noexcept
    {
        libs::MWR::resetBit(baseAddress + CR1, SPE);
    }

    bool SPI::IsEnabled() noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, SPE);
    }

    void SPI::SetMode(SPI_MODE m) noexcept
    {
        libs::MWR::resetBit(baseAddress + CR1, MSTR);
        switch(m)
        {
            case MASTER:
                libs::MWR::setBit(baseAddress + CR1, MSTR);
                break;
            case SLAVE:
                libs::MWR::resetBit(baseAddress + CR1, MSTR);
                break;
        }
    }

    SPI_MODE SPI::GetMode() noexcept
    {
        return (SPI_MODE)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, MSTR);
    }

    void SPI::SetStandard(SPI_STANDART s) noexcept
    {
        libs::MWR::resetBit(baseAddress + CR2, FRF);
        switch(s)
        {
            case MOTOROLA:
                libs::MWR::resetBit(baseAddress + CR2, FRF);
                break;
            case TI:
                libs::MWR::setBit(baseAddress + CR2, FRF);
                break;
        }
    }

    SPI_STANDART SPI::GetStandard() noexcept
    {
        return (SPI_STANDART)libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, FRF);
    }

    void SPI::SetClockPhase(CLOCK_PHASE cp) noexcept
    {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 0x1);
        libs::MWR::modifySetRegister(baseAddress + CR1, cp);
    }

    CLOCK_PHASE SPI::GetClockPhase() noexcept
    {
        return (CLOCK_PHASE)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, CPHA);
    }

    void SPI::SetClockPolarity(CLOCK_POLARITY cp) noexcept
    {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 1 << CPOL);
        libs::MWR::modifySetRegister(baseAddress + CR1, cp << CPOL);
    }

    CLOCK_POLARITY SPI::GetClockPolarity() noexcept
    {
        return (CLOCK_POLARITY)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, CPOL);
    }

    void SPI::SetBaudRatePrescaler(BAUD_RATE_PRESCALER brp) noexcept
    {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 0x7 << BR);
        libs::MWR::modifySetRegister(baseAddress + CR1, brp << BR);
    }

    BAUD_RATE_PRESCALER SPI::GetBaudRatePrescaler() noexcept
    {
        return (BAUD_RATE_PRESCALER)((libs::MWR::read_register<std::uint32_t>(baseAddress + CR1) &
                                      0x0038) >>
                                     BR);
    }

    void SPI::SetTransferBitOrder(TRANSFER_BIT_ORDER tbo) noexcept
    {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 1 << LSBFIRST);
        libs::MWR::modifySetRegister(baseAddress + CR1, tbo << LSBFIRST);
    }

    TRANSFER_BIT_ORDER SPI::GetTransferBitOrder() noexcept
    {
        return (TRANSFER_BIT_ORDER)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, LSBFIRST);
    }

    void SPI::SetTransferDirection(TRANSFER_DIRECTION td) noexcept
    {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 0xC400);
        switch(td)
        {
            case FULL_DUPLEX:
                libs::MWR::resetBit(baseAddress + CR1, RXONLY);
                break;
            case SIMPLEX_RX:
                libs::MWR::setBit(baseAddress + CR1, RXONLY);
                break;
            case HALF_DUPLEX_RX:
                libs::MWR::modifySetRegister(baseAddress + CR1, 1 << BIDIMODE);
                break;
            case HALF_DUPLEX_TX:
                libs::MWR::modifySetRegister(baseAddress + CR1, (1 << BIDIMODE) | (1 << BIDIOE));
                break;
        }
    }

    TRANSFER_DIRECTION SPI::GetTransferDirection() noexcept
    {
        bool rxonly   = libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, RXONLY);
        bool bidimode = libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, BIDIMODE);
        bool bidioe   = libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, BIDIOE);

        TRANSFER_DIRECTION td;
        if(bidimode == true)
        {
            if(bidioe == true)
                td = HALF_DUPLEX_TX;
            else
                td = HALF_DUPLEX_RX;
        }

        if(rxonly == true)
            td = SIMPLEX_RX;
        else
            td = FULL_DUPLEX;

        return td;
    }

    void SPI::SetDataWidth(DATA_WIDTH dw) noexcept
    {
        libs::MWR::resetBit(baseAddress + CR1, DFF);
        libs::MWR::modifySetRegister(baseAddress + CR1, dw << DFF);
    }

    DATA_WIDTH SPI::GetDataWidth() noexcept
    {
        return (DATA_WIDTH)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, DFF);
    }

    void SPI::EnableCRC() noexcept
    {
        libs::MWR::setBit(baseAddress + CR1, CRCEN);
    }

    void SPI::DisableCRC() noexcept
    {
        libs::MWR::resetBit(baseAddress + CR1, CRCEN);
    }

    bool SPI::IsEnabledCRC() noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, CRCEN);
    }

    void SPI::SetCRCNext() noexcept
    {
        libs::MWR::setBit(baseAddress + CR1, CRCNEXT);
    }

    void SPI::SetCRCPolynomial(std::uint16_t crcPoly) noexcept
    {
        libs::MWR::write_register(baseAddress + CRCPR, crcPoly);
    }

    std::uint16_t SPI::GetCRCPolynomial() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + CRCPR);
    }

    std::uint16_t SPI::GetRxCRC() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + RXCRCR);
    }

    std::uint16_t SPI::GetTxCRC() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + TXCRCR);
    }

    void SPI::SetNSSMode(NSS n) noexcept
    {
        libs::MWR::resetBit(baseAddress + CR1, SSM);
        libs::MWR::resetBit(baseAddress + CR2, SSOE);

        switch(n)
        {
            case SOFT:
                libs::MWR::setBit(baseAddress + CR1, SSM);
                break;
            case HARD_OUTPUT:
                libs::MWR::setBit(baseAddress + CR2, SSOE);
                break;
        }
    }

    NSS SPI::GetNSSMode() noexcept
    {
        bool ssm, ssoe;
        ssm  = libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, SSM);
        ssoe = libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, SSOE);

        NSS ns;

        if(ssm == true)
            ns = SOFT;
        else
        {
            if(ssoe == true)
                ns = HARD_OUTPUT;
            else
                ns = HARD_INPUT;
        }

        return ns;
    }

    bool SPI::IsActiveFlag(SR_poz flag) noexcept
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, flag);
    }

    void SPI::ClearFlag(CLEAR_FLAG flag) noexcept
    {
        switch(flag)
        {
            case CLF_CRCERR:
                ClearFlag_CRCERR();
                break;

            case CLF_MODF:
                ClearFlag_MODF();
                break;
                ;

            case CLF_OVR:
                ClearFlag_OVR();
                break;

            case CLF_FRE:
                ClearFlag_FRE();
                break;

            default:
                break;
        }
    }

    void SPI::ClearFlag_CRCERR() noexcept
    {
        libs::MWR::resetBit(baseAddress + SR, CRCERR);
    }

    void SPI::ClearFlag_MODF() noexcept
    {
        std::uint32_t reset = libs::MWR::read_register<std::uint32_t>(baseAddress + SR);
        libs::MWR::resetBit(baseAddress + CR1, SPE);
    }

    void SPI::ClearFlag_OVR() noexcept
    {
        std::uint32_t reset = libs::MWR::read_register<std::uint32_t>(baseAddress + DR);
        reset               = libs::MWR::read_register<std::uint32_t>(baseAddress + SR);
    }

    void SPI::ClearFlag_FRE() noexcept
    {
        std::uint32_t reset = libs::MWR::read_register<std::uint32_t>(baseAddress + SR);
    }

    void SPI::EnableIT(INT_FLAGS flag)
    {
        libs::MWR::setBit(baseAddress + CR2, flag);
    }

    void SPI::DisableIT(INT_FLAGS flag)
    {
        libs::MWR::resetBit(baseAddress + CR2, flag);
    }

    bool SPI::IsEnabledIT(INT_FLAGS flag)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, flag);
    }

    void SPI::DMAEnableTransmitter(STATUS s)
    {
        if(s == STATUS::ENABLE)
        {
            libs::MWR::setBit(baseAddress + CR2, TXDMAEN);
        }
        else
        {
            libs::MWR::resetBit(baseAddress + CR2, TXDMAEN);
        }
    }

    bool SPI::IsEnbaledDMATransmitter()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, TXDMAEN);
    }

    void SPI::DMAEnableReceiver(STATUS s)
    {
        if(s == STATUS::ENABLE)
        {
            libs::MWR::setBit(baseAddress + CR2, RXDMAEN);
        }
        else
        {
            libs::MWR::resetBit(baseAddress + CR2, RXDMAEN);
        }
    }

    bool SPI::IsEnabledDMAReceiver()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, RXDMAEN);
    }

    void SPI::sendByte(std::byte byte) noexcept
    {
        while(IsActiveFlag(TXE) == 0)
        {
        }
        libs::MWR::write_register(baseAddress + DR, byte);
    }

    std::byte SPI::readByte() noexcept
    {
        return libs::MWR::read_register<std::byte>(baseAddress + DR);
    }

    std::uint8_t SPI::ReceiveData8() noexcept
    {
        return libs::MWR::read_register<std::uint8_t>(baseAddress + DR);
    }

    std::uint16_t SPI::ReceiveData16() noexcept
    {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + DR);
    }

    void SPI::ReceiveArray8(void *rxData, std::uint16_t sizeArray) noexcept
    {
        std::size_t temp = 0;
        while(sizeArray--)
        {
            while(IsActiveFlag(RXNE))
            {
                *(static_cast<char *>(rxData) + temp++) = ReceiveData8();
            }
        }
    }

    void SPI::TransmitData8(std::uint8_t txData) noexcept
    {
        while(IsActiveFlag(TXE) == 0)
        {
        }
        libs::MWR::write_register(baseAddress + DR, txData);
    }

    void SPI::TransmitData16(std::uint16_t txData) noexcept
    {
        while(IsActiveFlag(TXE))
        {
        }
        libs::MWR::write_register(baseAddress + DR, txData);
    }

    void SPI::TransmitArray8(std::uint8_t *txData, std::uint32_t sizeArray) noexcept
    {
        std::size_t temp = 0;
        while(sizeArray--)
        {
            while(!IsActiveFlag(TXE))
            {
            }
            TransmitData8(txData[temp]);
            temp++;
        }
    }

    void SPI::TransmitReceiveArray(std::uint8_t *txData, std::uint8_t *rxData, std::size_t size)
    {
        std::size_t temp = 0;
        while(size--)
        {
            while(!IsActiveFlag(TXE))
            {
            }
            TransmitData8(txData[temp]);
            while(!IsActiveFlag(RXNE))
            {
            }
            rxData[temp] = ReceiveData8();
            temp++;
        }
    }

    void SPI::TransmitReceiveArray(void *txData, void *rxData, std::size_t size)
    {
        std::size_t temp = 0;
        DATA_WIDTH  dw   = GetDataWidth();

        while(size--)
        {
            while(!IsActiveFlag(TXE))
            {
            }
            if(dw == BIT8)
            {
                TransmitData8(*(static_cast<uint8_t *>(txData) + temp));
                temp++;
            }
            else
            {
                TransmitData16(*(static_cast<uint16_t *>(txData) + temp));
                temp += 2;
            }

            while(!IsActiveFlag(RXNE))
            {
            }

            if(dw == BIT8)
            {
                *(static_cast<uint8_t *>(rxData) + temp) = ReceiveData8();
                temp++;
            }
            else
            {
                *(static_cast<uint16_t *>(rxData) + temp) = ReceiveData16();
                temp += 2;
            }
        }
    }

    bool SPI::InitSpiDma(const drivers::dma::DMA &spiDma,
                         drivers::dma::DMA_Config dmaConfig,
                         bool                     isTransmit,
                         bool                     isReceive,
                         bool                     isDefault)
    {
        bool rezultInit = false;

        switch(baseAddress)
        {
            case SPI1:
                if(spiDma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_2)
                {
                    dmaConfig.channel                       = dma::constants::CHANNEL_3;
                    dmaConfig.peripheralIncrement           = dma::constants::DISABLE;
                    dmaConfig.memoryIncrement               = dma::constants::ENABLE;
                    dmaConfig.dataTransferDirection         = dma::constants::MEMORY_TO_PERIPHERAL;
                    dmaConfig.peripheralIncrementOffsetSize = dma::constants::DISABLE;

                    if(isTransmit == true)
                    {
                        if(isDefault == true)
                        {
                            dmaConfig.stream             = dma::constants::Stream_3;
                            dmaConfig.peripheralDataSize = dma::constants::BYTE;
                            dmaConfig.memoryDataSize     = dma::constants::BYTE;
                        }

                        DMAEnableTransmitter(ENABLE);
                        spiDma.ConfigurationDma(dmaConfig);
                    }
                    if(isReceive == true)
                    {
                        if(isDefault == true)
                        {
                            dmaConfig.stream             = dma::constants::Stream_0;
                            dmaConfig.peripheralDataSize = dma::constants::BYTE;
                            dmaConfig.memoryDataSize     = dma::constants::BYTE;
                        }
                        DMAEnableReceiver(ENABLE);
                        spiDma.ConfigurationDma(dmaConfig);
                    }
                }
                break;
            case SPI2:
                if(spiDma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                    dmaConfig.channel                       = dma::constants::CHANNEL_0;
                    dmaConfig.peripheralIncrement           = dma::constants::DISABLE;
                    dmaConfig.memoryIncrement               = dma::constants::ENABLE;
                    dmaConfig.dataTransferDirection         = dma::constants::MEMORY_TO_PERIPHERAL;
                    dmaConfig.peripheralIncrementOffsetSize = dma::constants::DISABLE;

                    if(isTransmit == true)
                    {
                        if(isDefault == true)
                        {
                            dmaConfig.peripheralDataSize = dma::constants::BYTE;
                            dmaConfig.memoryDataSize     = dma::constants::BYTE;
                        }
                        dmaConfig.stream = dma::constants::Stream_4;
                        DMAEnableTransmitter(ENABLE);
                        spiDma.ConfigurationDma(dmaConfig);
                    }
                    if(isReceive == true)
                    {
                        if(isDefault == true)
                        {
                            dmaConfig.peripheralDataSize = dma::constants::BYTE;
                            dmaConfig.memoryDataSize     = dma::constants::BYTE;
                        }
                        dmaConfig.stream = dma::constants::Stream_3;
                        DMAEnableReceiver(ENABLE);
                        spiDma.ConfigurationDma(dmaConfig);
                    }
                }
                break;
            case SPI3:
                if(spiDma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                    dmaConfig.channel                       = dma::constants::CHANNEL_0;
                    dmaConfig.peripheralIncrement           = dma::constants::DISABLE;
                    dmaConfig.memoryIncrement               = dma::constants::ENABLE;
                    dmaConfig.dataTransferDirection         = dma::constants::MEMORY_TO_PERIPHERAL;
                    dmaConfig.peripheralIncrementOffsetSize = dma::constants::DISABLE;

                    if(isTransmit == true)
                    {
                        if(isDefault == true)
                        {
                            dmaConfig.stream             = dma::constants::Stream_5;
                            dmaConfig.peripheralDataSize = dma::constants::BYTE;
                            dmaConfig.memoryDataSize     = dma::constants::BYTE;
                        }
                        DMAEnableTransmitter(ENABLE);
                        spiDma.ConfigurationDma(dmaConfig);
                    }
                    if(isReceive == true)
                    {
                        if(isDefault == true)
                        {
                            dmaConfig.stream             = dma::constants::Stream_0;
                            dmaConfig.peripheralDataSize = dma::constants::BYTE;
                            dmaConfig.memoryDataSize     = dma::constants::BYTE;
                        }
                        DMAEnableReceiver(ENABLE);
                        spiDma.ConfigurationDma(dmaConfig);
                    }
                }
                break;
        }
        return rezultInit;
    }

    bool SPI::TransmitDataDma(dma::DMA                     &dma,
                              dma::constants::NUMBER_STREAM stream,
                              std::uint8_t                 *sendBuffer,
                              std::uint16_t                 size)
    {
        bool rezultTransmit = false;
        switch(baseAddress)
        {
            case SPI1:
                if(dma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_2)
                {
                    dma.disable(stream);
                    dma.setPeripheralAddressRegister(stream, SPI1 + DR);
                    dma.setMemoryAddressRegister_0(stream,
                                                   reinterpret_cast<std::uint32_t>(sendBuffer));
                    dma.setNumberDataRegister(stream, size);
                    rezultTransmit = true;
                    dma.enable(stream);
                }
                break;

            case SPI2:
                if(dma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                    dma.disable(dma::constants::Stream_4);
                    dma.setPeripheralAddressRegister(dma::constants::Stream_4, SPI2 + DR);
                    dma.setMemoryAddressRegister_0(dma::constants::Stream_4,
                                                   reinterpret_cast<std::uint32_t>(sendBuffer));
                    dma.setNumberDataRegister(dma::constants::Stream_4, size);
                    rezultTransmit = true;
                    dma.enable(dma::constants::Stream_4);
                }
                break;

            case SPI3:
                if(dma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                    dma.disable(stream);
                    dma.setPeripheralAddressRegister(stream, SPI3 + DR);
                    dma.setMemoryAddressRegister_0(stream,
                                                   reinterpret_cast<std::uint32_t>(sendBuffer));
                    dma.setNumberDataRegister(stream, size);
                    rezultTransmit = true;
                    dma.enable(stream);
                }
                break;
        }
        return rezultTransmit;
    }

    bool SPI::ReceiveDataDma(dma::DMA                     &dma,
                             dma::constants::NUMBER_STREAM stream,
                             std::uint8_t                 *recvBuffer,
                             std::uint16_t                 size)
    {
        bool rezultReceive = false;

        switch(baseAddress)
        {
            case SPI1:
                if(dma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_2)
                {
                    dma.disable(stream);
                    dma.setPeripheralAddressRegister(stream, SPI1 + DR);
                    dma.setMemoryAddressRegister_0(stream,
                                                   reinterpret_cast<std::uint32_t>(recvBuffer));
                    dma.setNumberDataRegister(stream, size);

                    dma.enable(stream);
                    rezultReceive = true;
                }
                break;

            case SPI2:
                if(dma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                    dma.disable(dma::constants::Stream_3);
                    dma.setPeripheralAddressRegister(dma::constants::Stream_3, SPI2 + DR);
                    dma.setMemoryAddressRegister_0(dma::constants::Stream_3,
                                                   reinterpret_cast<std::uint32_t>(recvBuffer));
                    dma.setNumberDataRegister(dma::constants::Stream_3, size);
                    rezultReceive = true;
                    dma.enable(dma::constants::Stream_3);
                }
                break;

            case SPI3:
                if(dma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                    dma.disable(stream);
                    dma.setPeripheralAddressRegister(stream, SPI3 + DR);
                    dma.setMemoryAddressRegister_0(stream,
                                                   reinterpret_cast<std::uint32_t>(recvBuffer));
                    dma.setNumberDataRegister(stream, size);
                    rezultReceive = true;
                    dma.enable(stream);
                }
                break;
        }

        return rezultReceive;
    }
}    // namespace drivers::spi