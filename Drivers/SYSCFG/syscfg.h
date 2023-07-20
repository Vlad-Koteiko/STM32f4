//
// Created by koteiko_vv on 09.02.2023.
//
#ifndef SYSCFG_H
#define SYSCFG_H

#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::syscfg
{

    enum MEM_MODE : std::uint8_t
    {
        MAIN_FLASH    = 0,
        SYSTEM_FLASH  = 1,
        FSMC          = 2,
        EMBEDDED_SRAM = 3
    };

    enum EXTI_PORT : std::uint16_t
    {
        PORTA,
        PORTB,
        PORTC,
        PORTD,
        PORTE,
        PORTF,
        PORTG,
        PORTH,
        PORTI
    };

    enum EXTI_NUMBER : std::uint8_t
    {
        EXTI0,
        EXTI1,
        EXTI2,
        EXTI3,
        EXTI4,
        EXTI5,
        EXTI6,
        EXTI7,
        EXTI8,
        EXTI9,
        EXTI10,
        EXTI11,
        EXTI12,
        EXTI13,
        EXTI14,
        EXTI15
    };

    class SYSCFG
    {
        static constexpr std::uint32_t baseAddress = 0x40013800;
        drivers::clock::ClockControl  &clockControl;

        enum RegisterSYSCFG : std::uintptr_t
        {
            MEMRMP = baseAddress,    // SYSCFG memory remap register,                      Address
                                     // offset: 0x00
            PMC =
                baseAddress +
                0x04,    // SYSCFG peripheral mode configuration register,     Address offset: 0x04
            EXTICR1 =
                baseAddress +
                0x08,    // SYSCFG external interrupt configuration registers, Address offset: 0x08
            EXTICR2 = baseAddress + 0x0C,
            EXTICR3 = baseAddress + 0x10,
            EXTICR4 = baseAddress + 0x14,
            CMPCR =
                baseAddress +
                0x20    // SYSCFG Compensation cell control register,         Address offset: 0x20
        };

    public:
        SYSCFG(drivers::clock::ClockControl &clockControl);

        void SetSourceEXTI(EXTI_PORT exitPort, EXTI_NUMBER exitNumber) noexcept;

        void         SetMemoryMap(MEM_MODE mode);
        std::uint8_t GetMemoryMap();

        void MiiPhyEnable();
        void RmiiPhyEnable();

        void CompensationCellEnabled();
        void CompensationCellDisabled();

        bool CompensationCellReady();
    };
}    // namespace drivers::syscfg

#endif    // UART_SYSCFG_H
