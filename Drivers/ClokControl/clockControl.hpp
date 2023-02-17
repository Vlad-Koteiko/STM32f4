//
// Created by koteiko_vv on 06.12.2022.
//
#ifndef CLOCKCONTROL_H
#define CLOCKCONTROL_H

#include "MWR.hpp"

namespace drivers
{
    class ClockControl
    {
        static constexpr std::uintptr_t baseRegisterRCC = 0x40023800;
        static constexpr std::uintptr_t baseRegisterSysTick = 0xE000E010;

        enum RegisterRCC : std::uintptr_t
        {
            CR         = baseRegisterRCC,         // RCC clock control register, Address offset: 0x00
            PLLCFGR    = baseRegisterRCC + 0x04,  // RCC PLL configuration register, Address offset: 0x04
            CFGR       = baseRegisterRCC + 0x08,  // RCC clock configuration register, Address offset: 0x08
            CIR        = baseRegisterRCC + 0x0C,  // RCC clock interrupt register, Address offset: 0x0C
            AHB1RSTR   = baseRegisterRCC + 0x10,  // RCC AHB1 peripheral reset register, Address offset: 0x10
            AHB2RSTR   = baseRegisterRCC + 0x14,  // RCC AHB2 peripheral reset register, Address offset: 0x14
            AHB3RSTR   = baseRegisterRCC + 0x18,  // RCC AHB3 peripheral reset register, Address offset: 0x18
            APB1RSTR   = baseRegisterRCC + 0x20,  // RCC APB1 peripheral reset register, Address offset: 0x20
            APB2RSTR   = baseRegisterRCC + 0x24,  // RCC APB2 peripheral reset register, Address offset: 0x24
            AHB1ENR    = baseRegisterRCC + 0x30,  // RCC AHB1 peripheral clock register, Address offset: 0x30
            AHB2ENR    = baseRegisterRCC + 0x34,  // RCC AHB2 peripheral clock register, Address offset: 0x34
            AHB3ENR    = baseRegisterRCC + 0x38,  // RCC AHB3 peripheral clock register, Address offset: 0x38
            APB1ENR    = baseRegisterRCC + 0x40,  // RCC APB1 peripheral clock enable register, Address offset: 0x40
            APB2ENR    = baseRegisterRCC + 0x44,  // RCC APB2 peripheral clock enable register, Address offset: 0x44
            AHB1LPENR  = baseRegisterRCC + 0x50,  // RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50
            AHB2LPENR  = baseRegisterRCC + 0x54,  // RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54
            AHB3LPENR  = baseRegisterRCC + 0x58,  // RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58
            APB1LPENR  = baseRegisterRCC + 0x60,  // RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60
            APB2LPENR  = baseRegisterRCC + 0x64,  // RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64
            BDCR       = baseRegisterRCC + 0x70,  // RCC Backup domain control register, Address offset: 0x70
            CSR        = baseRegisterRCC + 0x74,  // RCC clock control & status register, Address offset: 0x74
            SSCGR      = baseRegisterRCC + 0x80,  // RCC spread spectrum clock generation register, Address offset: 0x80
            PLLI2SCFGR = baseRegisterRCC + 0x84   // RCC PLLI2S configuration register, Address offset: 0x84
        };

        enum RegisterSysTick : const std::uintptr_t
        {
              CTRL  = baseRegisterSysTick,          // Offset: 0x000 (R/W)  SysTick Control and Status Register
              LOAD  = baseRegisterSysTick + 0x04,   // Offset: 0x004 (R/W)  SysTick Reload Value Register
              VAL   = baseRegisterSysTick + 0x08,   // Offset: 0x008 (R/W)  SysTick Current Value Register
              CALIB = baseRegisterSysTick + 0x0C    // Offset: 0x00C (R/ )  SysTick Calibration Register
        };

    public:

        enum PrescalerAHB : std::uint8_t
        {
            AHB_OFF            = 0,      // system clock not divided
            AHB_DIVISOR_BY_2   = 0b1000, // system clock divided by 2
            AHB_DIVISOR_BY_4   = 0b1001, // system clock divided by 4
            AHB_DIVISOR_BY_8   = 0b1010, // system clock divided by 8
            AHB_DIVISOR_BY_16  = 0b1011, // system clock divided by 16
            AHB_DIVISOR_BY_64  = 0b1100, // system clock divided by 64
            AHB_DIVISOR_BY_128 = 0b1101, // system clock divided by 128
            AHB_DIVISOR_BY_256 = 0b1110, // system clock divided by 256
            AHB_DIVISOR_BY_512 = 0b1111, // system clock divided by 512
        };

        enum PrescalerAPB : const std::uint8_t
        {
            APB_OFF            = 0,     // system clock not divided
            APB_DIVISOR_BY_2   = 0b100, // system clock divided by 2
            APB_DIVISOR_BY_4   = 0b101, // system clock divided by 4
            APB_DIVISOR_BY_8   = 0b110, // system clock divided by 8
            APB_DIVISOR_BY_16  = 0b111, // system clock divided by 16
        };

        enum  TYPE_ENABLE_CLOCK_AHB_1 : std::uint8_t
        {
            PORT_A_AHB_1           = 0,
            PORT_B_AHB_1           = 1,
            PORT_C_AHB_1           = 2,
            PORT_D_AHB_1           = 3,
            PORT_E_AHB_1           = 4,
            PORT_F_AHB_1           = 5,
            PORT_G_AHB_1           = 6,
            PORT_H_AHB_1           = 7,
            PORT_I_AHB_1           = 8,
            PORT_J_AHB_1           = 9,
            PORT_K_AHB_1           = 10,
            CRC_CLOCK_ENABLE_AHB_1 = 12,
            BACKUP_SRAM_AHB_1      = 18,
            CCM_DATA_RAM_AHB_1     = 20,
            DMA1_EN_AHB_1          = 21,
            DMA2_EN_AHB_1          = 22
        };

        enum  TYPE_ENABLE_CLOCK_APB_1 : std::uint8_t
        {
            TIMER2_APB_1   = 0,
            TIMER3_APB_1   = 1,
            TIMER4_APB_1   = 2,
            TIMER5_APB_1   = 3,
            TIMER6_APB_1   = 4,
            TIMER7_APB_1   = 5,
            TIMER12_APB_1  = 6,
            TIMER13_APB_1  = 7,
            TIMER14_APB_1  = 8,
            WWDGEN_APB_1   = 11,
            SPI2_APB_1     = 14,
            SPI3_APB_1     = 15,
            USART2_APB_1   = 17,
            USART3_APB_1   = 18,
            USART4_APB_1   = 19,
            USART5_APB_1   = 20,
            I2C1_APB_1     = 21,
            I2C2_APB_1     = 22,
            I2C3_APB_1     = 23,
            CAN1_APB_1     = 25,
            CAN2_APB_1     = 26,
            PWR_APB_1      = 28,
            DAC_APB_1      = 29,
            USART7_APB_1   = 30,
            USART8_APB_1   = 31
        };

        enum  TYPE_ENABLE_CLOCK_APB_2 : std::uint8_t
        {
            USART_1_APB_2 = 4,
            SYSCF = 14
        };

        enum  MODULE : std::uint8_t
        {
            USART_1_MODULE,
            USART_2_MODULE,
            PORT_A_MODULE,
            PORT_H_MODULE,
            SYSCF_MODULE,
            PWR_MODULE,
            EXTI_MODULE
        };

        ClockControl();

        void SetCalibTrimming(std::uint32_t value) noexcept;

        void Enable() noexcept;

        [[nodiscard]] bool IsReady() noexcept;

        void SetAHBPrescaler(PrescalerAHB prescaler) noexcept;

        void SetAPB1Prescaler(PrescalerAPB prescaler) noexcept;

        void SetAPB2Prescaler(PrescalerAPB prescaler) noexcept;

        void SetSysClkSource(std::uint32_t value) noexcept;

        void InitTickSysTick(std::uint32_t HCLKFrequency, std::uint32_t ticks) noexcept;

        void AHB1EnableClock(TYPE_ENABLE_CLOCK_AHB_1 typeEnableClock) noexcept;

        void APB1EnableClock(TYPE_ENABLE_CLOCK_APB_1 typeEnableClock) noexcept;

        void APB2EnableClock(TYPE_ENABLE_CLOCK_APB_2 typeEnableClock) noexcept;

        void ESE_Enable() noexcept;

        bool HSE_IsReady() noexcept;

        bool PLL_IsReady() noexcept;

        std::uint32_t GetSysClkSourse() noexcept;

        void PLL_Config_Sys() noexcept;

        void mDelay(std::uint32_t Delay);

        void module_enable(MODULE module) noexcept;
    };

}    // namespace drivers

#endif    // BLINK_CLOCKCONTROL_H
