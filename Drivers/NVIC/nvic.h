//
// Created by koteiko_vv on 02.02.2023.
//
#ifndef UART_NVIC_H
#define UART_NVIC_H

#include "MWR.hpp"
#include "clockControl.hpp"
#include "exti.h"
#include "syscfg.h"

namespace drivers::nvic
{

    enum LINE_IRQn : std::uint8_t
    {
        WWDG,
        PVD,
        TAMP_STAMP,
        RTC_WKUP,
        FLASH,
        RCC,
        EXTI0,
        EXTI1,
        EXTI2,
        EXTI3,
        EXTI4,
        DMA1_Stream0,
        DMA1_Stream1,
        DMA1_Stream2,
        DMA1_Stream3,
        DMA1_Stream4,
        DMA1_Stream5,
        DMA1_Stream6,
        ADC,
        CAN1_TX,
        CAN1_RX0,
        CAN1_RX1,
        CAN1_SCE,
        EXTI9_5,
        TIM1_BRK_TIM9,
        TIM_UP_TIM10,
        TIM1_TRG_COM_TIM11,
        TIM1_CC,
        TIM2,
        TIM3,
        TIM4,
        I2C1_EV,
        I2C1_ER,
        I2C2_EV,
        I2C2_ER,
        SPI1,
        SPI2,
        USART1,
        USART2,
        USART3,
        EXTI15_10,
        RTC_Alarm,
        OTG_FS_WKUP,
        TIM8_BRK_TIM12,
        TIM8_UP_TIM13,
        TIM8_TRG_COM_TIM14,
        TIM8_CC,
        DMA1_Stream7,
        FSMC,
        SDIO,
        TIM5,
        SPI3,
        UART4,
        UART5,
        TIM6_DAC,
        TIM7,
        DMA2_Stream0,
        DMA2_Stream1,
        DMA2_Stream2,
        DMA2_Stream3,
        DMA2_Stream4,
        ETH,
        ETH_WKUP,
        CAN2_TX,
        CAN2_RX0,
        CAN2_RX1,
        CAN2_SCE,
        USB_OTG_FS,
        DMA2_Stream5,
        DMA2_Stream6,
        DMA2_Stream7,
        USART6,
        I2C3_EV,
        I2C3_ER,
        OTG_HS_EP1_OUT,
        OTG_HS_EP1_IN,
        OTG_HS_EP1_WKUP,
        OTG_HS,
        DCMI,
        CRYP,
        HASH_RNG,
        FPU
    };

    class NVIC
    {
        static constexpr std::uint32_t baseAddress = 0xE000E100;

        enum RegisterNVIC : std::uintptr_t
        {
            ISER = baseAddress,            // Interrupt Set Enable Register,
            ICER = baseAddress + 0x080,    // Interrupt Clear Enable Register,
            ISPR = baseAddress + 0x100,    // Interrupt Set Pending Register,
            ICPR = baseAddress + 0x180,    // Interrupt Clear Pending Register
            IABR = baseAddress + 0x200,    // Interrupt Active bit Register
            IP   = baseAddress + 0x300,    // Interrupt Priority Register (8Bit wide)
            STIR = baseAddress + 0x18      // Software Trigger Interrupt Register
        };

    public:
        NVIC();

        static void NVIC_EnableIRQ(LINE_IRQn irq) noexcept;
        static void NVIC_DisableIRQ(LINE_IRQn irq) noexcept;
        static bool NVIC_GetEnabledIRQ(LINE_IRQn irq) noexcept;

        static void         NVIC_SetPriority(LINE_IRQn irq, std::uint32_t priority) noexcept;
        static std::uint8_t NVIC_GetPriority(LINE_IRQn irq) noexcept;

        static bool NVIC_GetPendingIRQ(LINE_IRQn irq) noexcept;
        static void NVIC_SetPendingIRQ(LINE_IRQn irq) noexcept;
        static void NVIC_ClearPendingIRQ(LINE_IRQn irq) noexcept;

        static void NVIC_GetActive(LINE_IRQn irq) noexcept;
        static void NVIC_SetPriorityGrouping(std::uint32_t priority_grouping) noexcept;
        static void NVIC_SystemReset() noexcept;
    };
}    // namespace drivers::nvic

#endif    // UART_NVIC_H
