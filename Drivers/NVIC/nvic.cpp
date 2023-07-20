//
// Created by koteiko_vv on 02.02.2023.
//
#include "nvic.h"

namespace drivers::nvic
{

    NVIC::NVIC()
    {}

    //    void NVIC::NVIC_Enable(drivers::nvic::DEVICE_ID deviceId,
    //    drivers::syscfg::SYSCFG::EXIT_PORT exitPort,
    //                           drivers::syscfg::SYSCFG::EXIT_NUMBER exitNumber) noexcept {
    //
    //        const std::uint8_t  writeLimit = 31;
    //
    //        if(deviceId < 32)
    //        {
    //            libs::MWR::modifySetRegister(ISER, 1 << (deviceId & writeLimit));
    //
    //        }
    //        else if((deviceId < 64) && (deviceId > 31)){
    //
    //           libs::MWR::modifySetRegister(ISER + 4, 1 << (deviceId & writeLimit));
    //        }
    //
    //        syscfg.SetSourceEXTI(exitPort, exitNumber);
    //        exitRtsr.SetLineTrigger(static_cast<drivers::exti::EXIT_RTSR::LINE_NUMBER>(exitNumber));
    //    }

    void NVIC::NVIC_EnableIRQ(LINE_IRQn irq) noexcept
    {
        const std::uint8_t writeLimit = 0x1F;

        if(irq <= 31)
        {
            libs::MWR::modifySetRegister(ISER, 1 << (irq & writeLimit));
        }
        else if((irq >= 32) && (irq <= 63))
        {
            libs::MWR::modifySetRegister(ISER + 4, 1 << (irq & writeLimit));
        }
        else if((irq >= 64) && (irq <= 95))
        {
            libs::MWR::modifySetRegister(ISER + 8, 1 << (irq & writeLimit));
        }
    }

    void NVIC::NVIC_DisableIRQ(LINE_IRQn irq) noexcept
    {
        const std::uint8_t writeLimit = 0x1F;

        if(irq <= 31)
        {
            libs::MWR::modifySetRegister(ICER, 1 << (irq & writeLimit));
        }
        else if((irq < 64) && (irq > 31))
        {
            libs::MWR::modifySetRegister(ICER + 4, 1 << (irq & writeLimit));
        }
        else if((irq >= 64) && (irq <= 95))
        {
            libs::MWR::modifySetRegister(ICER + 8, 1 << (irq & writeLimit));
        }
    }

    bool NVIC::NVIC_GetEnabledIRQ(LINE_IRQn irq) noexcept
    {
        const std::uint8_t writeLimit = 0x1F;
        bool               rez;

        if(irq <= 31)
        {
            rez = libs::MWR::readBit<std::uint32_t>(ISER, (irq & writeLimit));
        }
        else if((irq < 64) && (irq > 31))
        {
            rez = libs::MWR::readBit<std::uint32_t>(ISER + 4, (irq & writeLimit));
        }
        else if((irq >= 64) && (irq <= 95))
        {
            rez = libs::MWR::readBit<std::uint32_t>(ISER + 8, (irq & writeLimit));
        }

        return rez;
    }

    void NVIC::NVIC_SetPriority(LINE_IRQn irq, std::uint32_t priority) noexcept
    {
        // libs::MWR::modifySetRegister(IP + (irq / 4), priority << (irq % 4 * 8));
        libs::MWR::modifyResetRegister(IP + (irq >> 2), 0xFF << (irq % 4 * 8));
        libs::MWR::modifySetRegister(IP + (irq >> 2), priority << (irq % 4 * 8));
    }

    std::uint8_t NVIC::NVIC_GetPriority(LINE_IRQn irq) noexcept
    {
        std::uint32_t buf_reg = libs::MWR::read_register<std::uint32_t>(IP + (irq >> 2));
        buf_reg               = (buf_reg & (0xFF << (irq % 4 * 8))) >> (irq % 4 * 8);
        return static_cast<std::uint8_t>(buf_reg);
    }

    bool NVIC::NVIC_GetPendingIRQ(LINE_IRQn irq) noexcept
    {
        const std::uint8_t writeLimit = 0x1F;
        bool               rez;

        if(irq <= 31)
        {
            rez = libs::MWR::readBit<std::uint32_t>(ISPR, (irq & writeLimit));
        }
        else if((irq < 64) && (irq > 31))
        {
            rez = libs::MWR::readBit<std::uint32_t>(ISPR + 4, (irq & writeLimit));
        }
        else if((irq >= 64) && (irq <= 95))
        {
            rez = libs::MWR::readBit<std::uint32_t>(ISPR + 8, (irq & writeLimit));
        }

        return rez;
    }

    void NVIC::NVIC_SetPendingIRQ(LINE_IRQn irq) noexcept
    {
        const std::uint8_t writeLimit = 0x1F;

        if(irq <= 31)
        {
            libs::MWR::modifySetRegister(ISPR, 1 << (irq & writeLimit));
        }
        else if((irq >= 32) && (irq <= 63))
        {
            libs::MWR::modifySetRegister(ISPR + 4, 1 << (irq & writeLimit));
        }
        else if((irq >= 64) && (irq <= 95))
        {
            libs::MWR::modifySetRegister(ISPR + 8, 1 << (irq & writeLimit));
        }
    }

    void NVIC::NVIC_ClearPendingIRQ(LINE_IRQn irq) noexcept
    {
        const std::uint8_t writeLimit = 0x1F;

        if(irq <= 31)
        {
            libs::MWR::modifySetRegister(ICPR, 1 << (irq & writeLimit));
        }
        else if((irq >= 32) && (irq <= 63))
        {
            libs::MWR::modifySetRegister(ICPR + 4, 1 << (irq & writeLimit));
        }
        else if((irq >= 64) && (irq <= 95))
        {
            libs::MWR::modifySetRegister(ICPR + 8, 1 << (irq & writeLimit));
        }
    }

    void NVIC::NVIC_SystemReset() noexcept
    {
        const std::uintptr_t AIRCR     = 0xE000ED0C;
        const std::uint32_t  KEY_VALUE = 0x05FA0004;

        __asm("DSB");
        libs::MWR::write_register(AIRCR, KEY_VALUE);
        __asm("DSB");
    }
}    // namespace drivers::nvic