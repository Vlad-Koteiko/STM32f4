//
// Created by koteiko_vv on 06.12.2022.
//
#ifndef CLOCKCONTROL_H
#define CLOCKCONTROL_H

#include "MWR.hpp"
#include "flash.h"

namespace drivers :: clock
{
    enum Frequency : std::uint32_t
    {
        FREQ_168000000 = 168000000,
        FREQ_100000000 = 100000000,
        FREQ_50000000  = 50000000,
        FREQ_48000000  = 48000000,
    };

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

    enum PrescalerAPB : std::uint8_t
    {
            APB_OFF            = 0,     // system clock not divided
            APB_DIVISOR_BY_2   = 0b100, // system clock divided by 2
            APB_DIVISOR_BY_4   = 0b101, // system clock divided by 4
            APB_DIVISOR_BY_8   = 0b110, // system clock divided by 8
            APB_DIVISOR_BY_16  = 0b111, // system clock divided by 16
    };

    enum CR_poz : std::uint8_t
    {
        HSION     = 0, // Internal high-speed clock enable
        HSIRDY    = 1, // Internal high-speed clock ready flag
        HSITRIM   = 3, // Internal high-speed clock trimming [7:3]
        HSICAL    = 8, // Internal high-speed clock calibration [15:8]
        HSEON     = 16, // HSE clock enable
        HSERDY    = 17, // HSE clock ready flag
        HSEBYP    = 18, // HSE clock bypass
        CSSON     = 19, // Clock security system enable
        PLLON     = 24, // Main PLL (PLL) enable
        PLLRDY    = 25, // Main PLL (PLL) clock ready flag
        PLLI2SON  = 26, // PLLI2S enable
        PLLI2SRDY = 27, // PLLI2S clock ready flag
        PLLISAION = 28, // PLLSAI enable
        PLLSAIRDY = 29, // PLLSAI clock ready flag
    };

    enum PLLCFGR_poz : std::uint8_t
    {
        PLLM = 0,
        PLLN = 6,
        PLLP = 16,
        PLLSRC = 22,
        PLLQ = 24
    };

    enum CFGR_poz : std::uint8_t
    {
        SW      = 0,  // System clock switch [1:0]
        SWS     = 2,  // System clock switch status [3:2]
        HPRE    = 4,  //AHB prescaler [7:4]
        PPRE1   = 10, // APB Low speed prescaler (APB1) [12:10]
        PPRE2   = 13, // APB high-speed prescaler (APB2) [15:13]
        RTCPRE  = 16, // HSE division factor for RTC clock [20:16]
        MCO1    = 21, // Microcontroller clock output 1 [22:21]
        I2SSRC  = 23, // I2S clock selection
        MCO1PRE = 24, // MCO1 prescaler [26:24]
        MCO2PRE = 27, // MCO2 prescaler [29:27]
        MCO2    = 30, // Microcontroller clock output 2 [31:30]
    };

    enum CIR_poz
    {
        LSIRDYF     = 0, // LSI ready interrupt flag
        LSERDYF     = 1, // LSE ready interrupt flag
        HSIRDYF     = 2, // HSI ready interrupt flag
        HSERDYF     = 3, // HSE ready interrupt flag
        PLLRDYF     = 4, // Main PLL (PLL) ready interrupt flag
        PLLI2SRDYF  = 5, // PLLI2S ready interrupt flag
        PLLSAIRDYF  = 6, // PLLSAI Ready Interrupt flag
        CSSF        = 7, // Clock security system interrupt flag
        LSIRDYIE    = 8, // LSI ready interrupt enable
        LSERDYIE    = 9, // LSE ready interrupt enable
        HSIRDYIE    = 10, // HSI ready interrupt enable
        HSERDYIE    = 11, // HSE ready interrupt enable
        PLLRDYIE    = 12, // Main PLL (PLL) ready interrupt enable
        PLLI2SRDYIE = 13, // PLLI2S ready interrupt enable
        PLLSAIRDYIE = 14, // PLLSAI Ready Interrupt Enable
        LSIRDYC     = 16, // LSI ready interrupt clear
        LSERDYC     = 17, // LSE ready interrupt clear
        HSIRDYC     = 18, // HSI ready interrupt clear
        HSERDYC     = 19, // HSE ready interrupt clear
        PLLRDYC     = 20, // Main PLL(PLL) ready interrupt clear
        PLLI2SRDYC  = 21, // PLLI2S ready interrupt clear
        PLLSAIRDYC  = 22, // PLLSAI Ready Interrupt Clear
        CSSC        = 23, // Clock security system interrupt clear
    };

    enum AHB1RSTR_poz
    {
        GPIOARST  = 0,  // IO port A reset
        GPIOBRST  = 1,  // IO port B reset
        GPIOCRST  = 2,  // IO port C reset
        GPIODRST  = 3,  // IO port D reset
        GPIOERST  = 4,  // IO port E reset
        GPIOFRST  = 5,  // IO port F reset
        GPIOGRST  = 6,  // IO port G reset
        GPIOHRST  = 7,  // IO port H reset
        GPIOIRST  = 8,  // IO port I reset
        CRCRST    = 12, // CRC reset
        DMA1RST   = 21, // DMA1 reset
        DMA2RST   = 22, // DMA2 reset
        ETHMACRST = 25, // Ethernet MAC reset
        OTGHSRST  = 26, // USB OTG HS module reset
    };

    enum AHB1ENR_poz
    {
        GPIOAEN      = 0, // IO port A clock enable
        GPIOBEN      = 1, // IO port B clock enable
        GPIOCEN      = 2, // IO port C clock enable
        GPIODEN      = 3, // IO port D clock enable
        GPIOEEN      = 4, // IO port E clock enable
        GPIOFEN      = 5, // IO port F clock enable
        GPIOGEN      = 6, // IO port G clock enable
        GPIOHEN      = 7, // IO port H clock enable
        GPIOIEN      = 8, // IO port I clock enable
        CRCEN        = 12, // CRC clock enable
        BKPSRAMEN    = 18, // Backup SRAM interface clock enable
        CCMDATARAMEN = 20, // CCM data RAM clock enable
        DMA1EN       = 21, // DMA1 clock enable
        DMA2EN       = 22, // DMA2 clock enable
        ETHMACEN     = 25, // Ethernet MAC clock enable
        ETHMACTXEN   = 26, // Ethernet Transmission clock enable
        ETHMACRXEN   = 27, // Ethernet Reception clock enable
        ETHMACPTPEN  = 28, // Ethernet PTP clock enable
        OTGHSEN      = 29, // USB OTG HS clock enable
        OTGHSULPIEN  = 30, // USB OTG HSULPI clock enable

    };

    enum AHB2_RSRT_ENR_poz
    {
        DCMI  = 0, // Camera interface reset
        CRYP  = 4, // Cryptographic module reset
        HASH  = 5, // Hash module reset
        RNG   = 6, // Random number generator module reset
        OTGFS = 7  // USB OTG FS module reset
    };

    enum  CLOCK_AHB_1 : std::uint8_t
    {
        PORTA           = 0,
        PORTB           = 1,
        PORTC           = 2,
        PORTD          = 3,
        PORTE          = 4,
        PORTF          = 5,
        PORTG         = 6,
        PORTH           = 7,
        PORTI          = 8,
        PORTJ          = 9,
        PORTK           = 10,
        CRC_CLOCK = 12,
        BACKUP_SRAM      = 18,
        CCM_DATA_RAM     = 20,
        DMA1         = 21,
        DMA2          = 22
    };

    enum  CLOCK_AHB_2 : std::uint8_t
    {
        USB_OTG_FS_AHB_2           = 7
    };


    enum  APB1_RSTR_ENR_poz : std::uint8_t
    {
        TIM2   = 0,
        TIM3   = 1,
        TIM4   = 2,
        TIM5   = 3,
        TIM6   = 4,
        TIM7   = 5,
        TIM12  = 6,
        TIM13  = 7,
        TIM14  = 8,
        WWDG   = 11,
        SPI2   = 14,
        SPI3   = 15,
        USART2 = 17,
        USART3 = 18,
        UART4  = 19,
        UART5  = 20,
        I2C1   = 21,
        I2C2   = 22,
        I2C3   = 23,
        CAN1   = 25,
        CAN2   = 26,
        PWR    = 28,
        DAC    = 29,
        UART7  = 30,
        UART8  = 31
    };

    enum  APB2_RSTR_ENR_poz : std::uint8_t
    {
        TIM1   = 0,
        TIM8   = 1,
        USART1 = 4,
        USART6 = 5,
        ADC1   = 8,
        ADC2   = 9,
        ADC3   = 10,
        SDIO   = 11,
        SPI1   = 12,
        SPI4   = 13,
        SYSCF  = 14,
        TIM9   = 16,
        TIM10  = 17,
        TIM11  = 18,
        SPI5   = 20,
        SPI6   = 21,
        SAI1   = 22,
        LTDC   = 26
    };

    enum BDCR_poz : std::uint8_t
    {
        LSEON  = 0,  // External low-speed oscillator enable
        LSERDY = 1,  // External low-speed oscillator ready
        LSEBYP = 2,  // External low-speed oscillator bypass
        RTCSEL = 8,  // RTC clock source selection [9:8]
        RTCEN  = 15, // RTC clock enable
        BDRST  = 16  // Backup domain software reset
    };

    enum RTC_Source : std::uint8_t
    {
        NO_CLOCK,
        LSE,
        LSI,
        HSE
    };

    enum CSR_poz : std::uint8_t
    {
        LSION    = 0,  // Internal low-speed oscillator enable
        LSIRDY   = 1,  // Internal low-speed oscillator ready
        RMVF     = 24, // Remove reset flag
        BORRSTF  = 25, // BOR reset flag
        PINRSTF  = 26, // PIN reset flag
        PORRSTF  = 27, // POR/PDR reset flag
        SFTRSTF  = 28, // Software reset flag
        IWDGRSTF = 29, // Independent watchdog reset flag
        WWDGRSTF = 30, // Window watchdog reset flag
        LPWRRSTF = 31  // Low-power reset flag
    };

    enum PERIPHERALS : std::uint8_t
    {
        USART_1_MODULE,
        USART_2_MODULE,
        USART_3_MODULE,
        UART_4_MODULE,
        UART_5_MODULE,
        USART_6_MODULE,
        UART_7_MODULE,
        UART_8_MODULE,
        PORT_A_MODULE,
        PORT_B_MODULE,
        PORT_C_MODULE,
        PORT_D_MODULE,
        PORT_E_MODULE,
        PORT_H_MODULE,
        PORT_I_MODULE,
        SYSCF_MODULE,
        PWR_MODULE,
        USB_FS_MODULE,
        RNG_MODULE,
        TIM6_MODULE,
        TIM7_MODULE,
        SPI1_MODULE,
        SPI2_MODULE,
        SPI3_MODULE,
        SPI4_MODULE,
        SPI5_MODULE,
        SPI6_MODULE
    };

    class ClockControl
    {
        static constexpr std::uintptr_t baseRegisterRCC = 0x40023800;
        static constexpr std::uintptr_t baseRegisterSysTick = 0xE000E010;
        volatile std::uint32_t  systemCoreClock; //Hz
        volatile std::uint32_t  freqAPB1;        //Hz
        volatile std::uint32_t  freqAPB2;        //Hz
        volatile std::uint32_t  freqHCLK;        //Hz

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
        ClockControl();
        ClockControl(Frequency f);
        volatile std::uint32_t GetFreqHCLK();
        volatile std::uint32_t GetFreqSystemCoreClock();
        volatile std::uint32_t GetFreqAPB1();
        volatile std::uint32_t GetFreqAPB2();

        void SetCalibTrimming(std::uint32_t value) noexcept;
        void SetAHBPrescaler(PrescalerAHB prescaler) noexcept;
        void SetAPB1Prescaler(PrescalerAPB prescaler) noexcept;
        void SetAPB2Prescaler(PrescalerAPB prescaler) noexcept;
        void SetSysClkSource(std::uint32_t value) noexcept;
        void InitTickSysTick(std::uint32_t HCLKFrequency, std::uint32_t ticks) noexcept;

        void AHB1EnableClock(AHB1ENR_poz typeEnableClock) const noexcept;
        void AHB1DisableClock(AHB1RSTR_poz disableClock) const noexcept;
        void AHB2EnableClock(AHB2_RSRT_ENR_poz typeEnableClock) const noexcept;
        void AHB2DisableClock(AHB2_RSRT_ENR_poz disableClock) const noexcept;
        void APB1EnableClock(APB1_RSTR_ENR_poz typeEnableClock) const noexcept;
        void APB1DisableClock(APB1_RSTR_ENR_poz disableClock) const noexcept;
        void APB2EnableClock(APB2_RSTR_ENR_poz typeEnableClock) const noexcept;
        void APB2DisableClock(APB2_RSTR_ENR_poz disableClock) const noexcept;

        /**
         * Включить итсочник внешнего тактирования
         */
        void HSE_Enable() noexcept;
        /**
         * Отключить источник внешнего тактирования
         */
        void HSE_Disable() noexcept;
        /**
         * Получить статус работы внешнего тактирование
         * @return true - включен, false - выключен
         */
        bool HSE_Status() noexcept;
        /**
         * Готовность внешнего источника тактирования
         * @return true - готов, false - неготов
         */
        [[nodiscard]] bool HSE_IsReady() noexcept;

        std::uint32_t GetSysClkSourse() noexcept;
        /**
         * Конфигурирование блока умножения PLL
         * @param setPLLN Множитель PLLN (192 <= setPLLN <= 432)
         * @param setPLLM Множитель PLLM (2 <= setPLLM <= 63)
         * @param setPLLP Множитель PLLP (0 <= setPLLP <= 3)
         * @param setPLLQ Множитель PLLQ (2 = setPLLQ <= 15)
         */
        void PLL_Config_Sys(std::uint16_t setPLLN, std::uint16_t setPLLM , std::uint16_t  setPLLP, std::uint16_t setPLLQ);

        /**
         * Выбор источника тактирования
         * @param bit bit=0 - HSI, bit=1 - HSE
         */
        void PLL_SetSource(std::uint8_t bit);
        void SetInternalClockGenerator_16MHz() noexcept;
        void SetExternalClockGenerator_168MHz() noexcept;

        void mDelay(std::uint32_t Delay)const;

        void EnablePeripherals(PERIPHERALS name) const noexcept;
        void DisablePeripherals(PERIPHERALS name) const noexcept;

        /**
         * Включить внутренний генератор
         */
        void HSI_Enable() noexcept;
        /**
         * Отключить внутренний генератор
         */
        void HSI_Disable() noexcept;
        /**
         * Статус работы от внутреннего источника тактирования
         * @return
         */
        bool HSI_Status() noexcept;
        /**
         * Проверка готовности внутреннего источника тактирования
         * @return  true - готов, false - неготов
         */
        bool  HSI_IsReady() noexcept;
        /**
         * Включить умножитель тактов
         */
        void PLL_Enable() noexcept;
        /**
         * Отключить умножитель тактов
         */
        void PLL_Disable() noexcept;
        /**
         * Готовность умножителя
         * @return true - locked, false - unlocked
         */
        [[nodiscard]] bool PLL_IsReady() noexcept;

        void LSE_Enable() noexcept;
        void LSE_Disable() noexcept;
        bool LSE_IsReady() noexcept;
        void LSE_SetBypass() noexcept;
        void LSE_ResetBypass() noexcept;
        bool LSE_GetBypass() noexcept;
        void SetSourceClock(RTC_Source s) noexcept;
        void RTC_Enable() noexcept;
        void RTC_Disable() noexcept;
        bool RTC_GetStatus() noexcept;
        void BackupDomainReset() noexcept;
        void BackupDomainNoreset() noexcept;
        bool GetBackupDomainStatus() noexcept;

        void LSI_Enable() noexcept;
        void LSI_Disable() noexcept;
        bool LSI_Status() noexcept;
        bool LSI_IsReady() noexcept;
        void RemoveResetFlag() noexcept;
        bool GetBorResetFlag() noexcept;
        bool GetPinResetFlag() noexcept;
        bool GetPorResetFlag() noexcept;
        bool GetSoftwareResetFlag() noexcept;
        bool GetIwdgResetFlag() noexcept;
        bool GetWwdgResetFlag() noexcept;
        bool GetLowpowerResetFlag() noexcept;
    };

}    // namespace drivers

#endif    // CLOCKCONTROL_H
