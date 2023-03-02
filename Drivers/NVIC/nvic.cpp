//
// Created by koteiko_vv on 02.02.2023.
//
#include "nvic.h"

namespace drivers::nvic {


    NVIC::NVIC(drivers::ClockControl &clockControl1) :  syscfg(clockControl1)
    {}

    void NVIC::NVIC_Enable(drivers::nvic::NVIC::DEVICE_ID deviceId, drivers::syscfg::SYSCFG::EXIT_PORT exitPort,
                           drivers::syscfg::SYSCFG::EXIT_NUMBER exitNumber) noexcept {

        const std::uint8_t  writeLimit = 31;

        if(deviceId < 32)
        {
            libs::MWR::setBit(ISER, 1 << (deviceId & writeLimit));

        } else if((deviceId < 64) && (deviceId > 31)){

           libs::MWR::setBit(ISER + 4, 1 << (deviceId & writeLimit));
        }

        syscfg.SetSourceEXTI(exitPort, exitNumber);
        exitRtsr.SetLineTrigger(static_cast<drivers::exti::EXIT_RTSR::LINE_NUMBER>(exitNumber));
    }

    void NVIC::NVIC_Enable(drivers::nvic::NVIC::DEVICE_ID deviceId) noexcept {

        const std::uint8_t  writeLimit = 31;

        if(deviceId < 32)
        {
            libs::MWR::setBit(ISER, 1 << (deviceId & writeLimit));

        } else if((deviceId > 31) && (deviceId < 64)){

            libs::MWR::setBit(ISER + 4, 1 << (deviceId & writeLimit));

        } else if( (deviceId > 63) && (deviceId < 96)){

            libs::MWR::setBit(ISER + 8, 1 << (deviceId & writeLimit));
}

    }

    void NVIC::NVIC_SetPriority(drivers::nvic::NVIC::DEVICE_ID deviceId, std::uint32_t priority) noexcept {

        libs::MWR::setBit(IP + (deviceId - 4), static_cast<std::uint8_t>(priority << 4));

    }

}