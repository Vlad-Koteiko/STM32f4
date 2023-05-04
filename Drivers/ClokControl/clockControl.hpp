//
// Created by koteiko_vv on 06.12.2022.
//
#ifndef CLOCKCONTROL_H
#define CLOCKCONTROL_H

#include "MWR.hpp"
#include "flash.h"
#include <array>

namespace drivers :: clock
{
    enum Frequency : std::uint32_t
    {
        FREQ_168000000 = 168000000,
        FREQ_100000000 = 100000000,
        FREQ_50000000  = 50000000,
        FREQ_48000000  = 48000000,
    };

    namespace constants {

        enum PrescalerAHB : std::uint8_t {
            AHB_OFF = 0,      // system clock not divided
            AHB_DIVISOR_BY_2 = 0b1000, // system clock divided by 2
            AHB_DIVISOR_BY_4 = 0b1001, // system clock divided by 4
            AHB_DIVISOR_BY_8 = 0b1010, // system clock divided by 8
            AHB_DIVISOR_BY_16 = 0b1011, // system clock divided by 16
            AHB_DIVISOR_BY_64 = 0b1100, // system clock divided by 64
            AHB_DIVISOR_BY_128 = 0b1101, // system clock divided by 128
            AHB_DIVISOR_BY_256 = 0b1110, // system clock divided by 256
            AHB_DIVISOR_BY_512 = 0b1111, // system clock divided by 512
        };

        enum PrescalerAPB : std::uint8_t {
            APB_OFF = 0,     // system clock not divided
            APB_DIVISOR_BY_2 = 0b100, // system clock divided by 2
            APB_DIVISOR_BY_4 = 0b101, // system clock divided by 4
            APB_DIVISOR_BY_8 = 0b110, // system clock divided by 8
            APB_DIVISOR_BY_16 = 0b111, // system clock divided by 16
        };

        enum CR_poz : std::uint8_t {
            HSION = 0, // Internal high-speed clock enable
            HSIRDY = 1, // Internal high-speed clock ready flag
            HSITRIM = 3, // Internal high-speed clock trimming [7:3]
            HSICAL = 8, // Internal high-speed clock calibration [15:8]
            HSEON = 16, // HSE clock enable
            HSERDY = 17, // HSE clock ready flag
            HSEBYP = 18, // HSE clock bypass
            CSSON = 19, // Clock security system enable
            PLLON = 24, // Main PLL (PLL) enable
            PLLRDY = 25, // Main PLL (PLL) clock ready flag
            PLLI2SON = 26, // PLLI2S enable
            PLLI2SRDY = 27, // PLLI2S clock ready flag
            PLLISAION = 28, // PLLSAI enable
            PLLSAIRDY = 29, // PLLSAI clock ready flag
        };

        enum PLLCFGR_poz : std::uint8_t {
            PLLM = 0,
            PLLN = 6,
            PLLP = 16,
            PLLSRC = 22,
            PLLQ = 24
        };

        enum CFGR_poz : std::uint8_t {
            SW = 0,  // System clock switch [1:0]
            SWS = 2,  // System clock switch status [3:2]
            HPRE = 4,  //AHB prescaler [7:4]
            PPRE1 = 10, // APB Low speed prescaler (APB1) [12:10]
            PPRE2 = 13, // APB high-speed prescaler (APB2) [15:13]
            RTCPRE = 16, // HSE division factor for RTC clock [20:16]
            MCO1 = 21, // Microcontroller clock output 1 [22:21]
            I2SSRC = 23, // I2S clock selection
            MCO1PRE = 24, // MCO1 prescaler [26:24]
            MCO2PRE = 27, // MCO2 prescaler [29:27]
            MCO2 = 30, // Microcontroller clock output 2 [31:30]
        };

        enum CIR_poz {
            LSIRDYF = 0, // LSI ready interrupt flag
            LSERDYF = 1, // LSE ready interrupt flag
            HSIRDYF = 2, // HSI ready interrupt flag
            HSERDYF = 3, // HSE ready interrupt flag
            PLLRDYF = 4, // Main PLL (PLL) ready interrupt flag
            PLLI2SRDYF = 5, // PLLI2S ready interrupt flag
            PLLSAIRDYF = 6, // PLLSAI Ready Interrupt flag
            CSSF = 7, // Clock security system interrupt flag
            LSIRDYIE = 8, // LSI ready interrupt enable
            LSERDYIE = 9, // LSE ready interrupt enable
            HSIRDYIE = 10, // HSI ready interrupt enable
            HSERDYIE = 11, // HSE ready interrupt enable
            PLLRDYIE = 12, // Main PLL (PLL) ready interrupt enable
            PLLI2SRDYIE = 13, // PLLI2S ready interrupt enable
            PLLSAIRDYIE = 14, // PLLSAI Ready Interrupt Enable
            LSIRDYC = 16, // LSI ready interrupt clear
            LSERDYC = 17, // LSE ready interrupt clear
            HSIRDYC = 18, // HSI ready interrupt clear
            HSERDYC = 19, // HSE ready interrupt clear
            PLLRDYC = 20, // Main PLL(PLL) ready interrupt clear
            PLLI2SRDYC = 21, // PLLI2S ready interrupt clear
            PLLSAIRDYC = 22, // PLLSAI Ready Interrupt Clear
            CSSC = 23, // Clock security system interrupt clear
        };

        enum BDCR_poz : std::uint8_t {
            LSEON = 0,  // External low-speed oscillator enable
            LSERDY = 1,  // External low-speed oscillator ready
            LSEBYP = 2,  // External low-speed oscillator bypass
            RTCSEL = 8,  // RTC clock source selection [9:8]
            RTCEN = 15, // RTC clock enable
            BDRST = 16  // Backup domain software reset
        };

        enum RTC_Source : std::uint8_t {
            NO_CLOCK,
            LSE,
            LSI,
            HSE
        };

        enum CSR_poz : std::uint8_t {
            LSION = 0,  // Internal low-speed oscillator enable
            LSIRDY = 1,  // Internal low-speed oscillator ready
            RMVF = 24, // Remove reset flag
            BORRSTF = 25, // BOR reset flag
            PINRSTF = 26, // PIN reset flag
            PORRSTF = 27, // POR/PDR reset flag
            SFTRSTF = 28, // Software reset flag
            IWDGRSTF = 29, // Independent watchdog reset flag
            WWDGRSTF = 30, // Window watchdog reset flag
            LPWRRSTF = 31  // Low-power reset flag
        };

        enum PERIPHERALS : std::uint8_t {
            // RCC AHB1 peripheral clock register
            PORT_A_MODULE = 0,
            PORT_B_MODULE = 1,
            PORT_C_MODULE = 2,
            PORT_D_MODULE = 3,
            PORT_E_MODULE = 4,
            PORT_F_MODULE = 5,
            PORT_G_MODULE = 6,
            PORT_H_MODULE = 7,
            PORT_I_MODULE = 8,
            CRC_MODULE = 12,
            BKPSRAM_MODULE = 18,
            CCMDATARAM_MODULE = 20,
            DMA1_MODULE = 21,
            DMA2_MODULE = 22,
            ETHMAC_MODULE = 25,
            ETHMACTX_MODULE = 26,
            ETHMACRX_MODULE = 27,
            ETHMACPTP_MODULE = 28,
            OTGHS_MODULE = 29,
            OTGHSULPI_MODULE = 30,

            // RCC AHB2 peripheral clock enable register
            DCMI_MODULE = 0 + 32,
            CRYP_MODULE = 4 + 32,
            HASH_MODULE = 5 + 32,
            RNG_MODULE = 6 + 32,
            OTGFS_MODULE = 7 + 32,

            // RCC APB1 peripheral clock enable register
            TIM2_MODULE = 0 + 64,
            TIM3_MODULE = 1 + 64,
            TIM4_MODULE = 2 + 64,
            TIM5_MODULE = 3 + 64,
            TIM6_MODULE = 4 + 64,
            TIM7_MODULE = 5 + 64,
            TIM12_MODULE = 6 + 64,
            TIM13_MODULE = 7 + 64,
            TIM14_MODULE = 8 + 64,
            WWDG_MODULE = 11 + 64,
            SPI2_MODULE = 14 + 64,
            SPI3_MODULE = 15 + 64,
            USART2_MODULE = 17 + 64,
            USART3_MODULE = 18 + 64,
            UART4_MODULE = 19 + 64,
            UART5_MODULE = 20 + 64,
            I2C1_MODULE = 21 + 64,
            I2C2_MODULE = 22 + 64,
            I2C3_MODULE = 23 + 64,
            CAN1_MODULE = 25 + 64,
            CAN2_MODULE = 26 + 64,
            PWR_MODULE = 28 + 64,
            DAC_MODULE = 29 + 64,
            UART7_MODULE = 30 + 64,
            UART8_MODULE = 31 + 64,

            // RCC APB2 peripheral clock enable register
            TIM1_MODULE = 0 + 96,
            TIM8_MODULE = 1 + 96,
            USART1_MODULE = 4 + 96,
            USART6_MODULE = 5 + 96,
            ADC1_MODULE = 8 + 96,
            ADC2_MODULE = 9 + 96,
            ADC3_MODULE = 10 + 96,
            SDIO_MODULE = 11 + 96,
            SPI1_MODULE = 12 + 96,
            SPI4_MODULE = 13 + 96,
            SYSCF_MODULE = 14 + 96,
            TIM9_MODULE = 16 + 96,
            TIM10_MODULE = 17 + 96,
            TIM11_MODULE = 18 + 96,
            SPI5_MODULE = 20 + 96,
            SPI6_MODULE = 21 + 96,
            SAI1_MODULE = 22 + 96,
            LTDC_MODULE = 26 + 96
        };
    }

    constexpr auto countFreqHCLK =         [](const Frequency& f) constexpr noexcept -> std::uint32_t {return f;};
    constexpr auto countSystemCoreClock =  [](const Frequency& f) constexpr noexcept -> std::uint32_t {return f;};
    constexpr auto countFreqAPB1 =         [](const Frequency& f) constexpr noexcept -> std::uint32_t {
        std::uint32_t temp = f;
        switch (f) {
            case FREQ_48000000:
                temp = temp / 2;
                break;

            case FREQ_50000000:
                temp = temp / 2;
                break;

            case FREQ_100000000:
                temp = temp / 4;
                break;

            case FREQ_168000000:
                temp = temp / 4;
                break;
        }return temp;};

    constexpr auto countFreqAPB2 =         [](const Frequency& f) constexpr noexcept -> std::uint32_t {
        std::uint32_t temp = f;
        switch (f) {
            case FREQ_48000000:
                temp = temp / 1;
                break;

            case FREQ_50000000:
                temp = temp / 2;
                break;

            case FREQ_100000000:
                temp = temp / 2;
                break;

            case FREQ_168000000:
                temp = temp / 2;
                break;
        }return temp;};


    class ClockControl
    {
        static constexpr std::uintptr_t baseRegisterRCC     = 0x40023800;
        static constexpr std::uintptr_t baseRegisterSysTick = 0xE000E010;
        static constexpr std::uint8_t   AHB_2               = 32;
        static constexpr std::uint8_t   AHP_1               = 64;
        static constexpr std::uint8_t   AHP_2               = 96;

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

        class Variables
        {
        public:
           constexpr Variables(const Frequency& f) : systemCoreClock(countSystemCoreClock(f)), freqAPB1(countFreqAPB1(f)),
                                                     freqAPB2(countFreqAPB2(f)),               freqHCLK(countFreqHCLK(f))
            {};

            std::uint32_t  systemCoreClock; //Hz
            std::uint32_t  freqAPB1;        //Hz
            std::uint32_t  freqAPB2;        //Hz
            std::uint32_t  freqHCLK;        //Hz
        };

        Variables variables;

        constexpr std::uint8_t getPeripheralsNumber(const std::uint8_t& ,const std::uint8_t&) const noexcept;

        void setBaseConfig(std::array<std::uint8_t,4>, const constants::PrescalerAHB&, const constants::PrescalerAPB&,
                           const constants::PrescalerAPB&) noexcept;

        void AHB1EnableClock( const std::uint8_t &typeEnableClock)  const noexcept;
        void AHB1DisableClock(const std::uint8_t &disableClock)     const noexcept;
        void AHB2EnableClock( const std::uint8_t &typeEnableClock)  const noexcept;
        void AHB2DisableClock(const std::uint8_t &disableClock)     const noexcept;
        void APB1EnableClock( const std::uint8_t &typeEnableClock)  const noexcept;
        void APB1DisableClock(const std::uint8_t &disableClock)     const noexcept;
        void APB2EnableClock( const std::uint8_t &typeEnableClock)  const noexcept;
        void APB2DisableClock(const std::uint8_t &disableClock)     const noexcept;

    public:

        ClockControl() = delete;

        constexpr ClockControl(const Frequency& f) : variables(f)
        {
            drivers::flash::Flash flash;
            flash.SetLatency(5);
            while (flash.GetLatency() != 5)
            {}

            HSE_Enable();

            while (!HSE_IsReady())
            {}

            HSI_Disable();

            switch (f) {
                case FREQ_48000000:
                    setBaseConfig({96,4,0,4}, constants::AHB_DIVISOR_BY_2, constants::APB_DIVISOR_BY_2, constants::APB_OFF);
                    break;

                case FREQ_50000000:
                    setBaseConfig({50,4,0,4},constants::AHB_OFF,constants::APB_DIVISOR_BY_2,constants::APB_DIVISOR_BY_2);
                    break;

                case FREQ_100000000:
                    setBaseConfig({100,4,0,4},constants::AHB_OFF,constants::APB_DIVISOR_BY_4,constants::APB_DIVISOR_BY_2);
                    break;

                case FREQ_168000000:
                    setBaseConfig({168,4,0,7},constants::AHB_OFF,constants::APB_DIVISOR_BY_4,constants::APB_DIVISOR_BY_2);
                    break;
            }
            InitTickSysTick(variables.freqHCLK,1000);
        }

        constexpr std::uint32_t GetFreqSystemCoreClock() const noexcept  {
            return variables.systemCoreClock;
        }

        constexpr std::uint32_t GetFreqHCLK()  const noexcept{
            return variables.freqHCLK;
        }

        constexpr std::uint32_t GetFreqAPB1() const noexcept  {
            return variables.freqAPB1;
        }

        constexpr std::uint32_t GetFreqAPB2()  const noexcept {
            return variables.freqAPB2;
        }

        void SetCalibTrimming(std::uint32_t value) const noexcept;
        void SetAHBPrescaler(const constants::PrescalerAHB& prescaler) const noexcept;
        void SetAPB1Prescaler(const constants::PrescalerAPB& prescaler) const noexcept;
        void SetAPB2Prescaler(const constants::PrescalerAPB& prescaler) const noexcept;
        void SetSysClkSource(std::uint32_t value) const noexcept;
        void InitTickSysTick(std::uint32_t HCLKFrequency, std::uint32_t ticks) const noexcept;

        /**
         * Включить итсочник внешнего тактирования
         */
        void HSE_Enable() const noexcept;
        /**
         * Отключить источник внешнего тактирования
         */
        void HSE_Disable() const noexcept;
        /**
         * Получить статус работы внешнего тактирование
         * @return true - включен, false - выключен
         */
       [[nodiscard]] bool HSE_Status() const noexcept;
        /**
         * Готовность внешнего источника тактирования
         * @return true - готов, false - неготов
         */
        [[nodiscard]] bool HSE_IsReady() const noexcept;

        std::uint32_t GetSysClkSourse() const noexcept;
        /**
         * Конфигурирование блока умножения PLL
         * @param setPLLN Множитель PLLN (192 <= setPLLN <= 432)
         * @param setPLLM Множитель PLLM (2 <= setPLLM <= 63)
         * @param setPLLP Множитель PLLP (0 <= setPLLP <= 3)
         * @param setPLLQ Множитель PLLQ (2 = setPLLQ <= 15)
         */
        void PLL_Config_Sys(std::uint16_t setPLLN, std::uint16_t setPLLM , std::uint16_t  setPLLP, std::uint16_t setPLLQ) const noexcept;

        /**
         * Выбор источника тактирования
         * @param bit bit=0 - HSI, bit=1 - HSE
         */
        void PLL_SetSource(std::uint8_t bit) const noexcept;
        void mDelay(std::uint32_t Delay) const noexcept;

        void EnablePeripherals(const  constants::PERIPHERALS &name) const noexcept;
        void DisablePeripherals(const constants::PERIPHERALS &name) const noexcept;

        /**
         * Включить внутренний генератор
         */
        void HSI_Enable() const noexcept;
        /**
         * Отключить внутренний генератор
         */
        void HSI_Disable() const noexcept;
        /**
         * Статус работы от внутреннего источника тактирования
         * @return
         */
        [[nodiscard]] bool HSI_Status() const noexcept;
        /**
         * Проверка готовности внутреннего источника тактирования
         * @return  true - готов, false - неготов
         */
        [[nodiscard]] bool  HSI_IsReady() const noexcept;
        /**
         * Включить умножитель тактов
         */
        void PLL_Enable() const noexcept;
        /**
         * Отключить умножитель тактов
         */
        void PLL_Disable() const noexcept;
        /**
         * Готовность умножителя
         * @return true - locked, false - unlocked
         */
        [[nodiscard]] bool PLL_IsReady() const noexcept;

        void LSE_Enable() const noexcept;
        void LSE_Disable() const noexcept;
        [[nodiscard]] bool LSE_IsReady() const noexcept;
        void LSE_SetBypass() const noexcept;
        void LSE_ResetBypass() const noexcept;
        [[nodiscard]] bool LSE_GetBypass() const noexcept;
        void SetSourceClock(constants::RTC_Source s) const noexcept;
        void RTC_Enable() const noexcept;
        void RTC_Disable() const noexcept;
        [[nodiscard]] bool RTC_GetStatus() const noexcept;
        void BackupDomainReset() const noexcept;
        void BackupDomainNoreset() const noexcept;
        [[nodiscard]] bool GetBackupDomainStatus() const noexcept;

        void LSI_Enable() const noexcept;
        void LSI_Disable() const noexcept;
        [[nodiscard]] bool LSI_Status() const noexcept;
        [[nodiscard]] bool LSI_IsReady() const noexcept;
        void RemoveResetFlag() const noexcept;
        [[nodiscard]] bool GetBorResetFlag() const noexcept;
        [[nodiscard]] bool GetPinResetFlag() const noexcept;
        [[nodiscard]] bool GetPorResetFlag() const noexcept;
        [[nodiscard]] bool GetSoftwareResetFlag() const noexcept;
        [[nodiscard]] bool GetIwdgResetFlag() const noexcept;
        [[nodiscard]] bool GetWwdgResetFlag() const noexcept;
        [[nodiscard]] bool GetLowpowerResetFlag() const noexcept;
    };

}    // namespace drivers

#endif    // CLOCKCONTROL_H
