//
// Created by koteiko_vv on 02.02.2023.
//
#include "nvic.h"

namespace drivers::nvic {

    void NVIC::NVIC_Enable(drivers::nvic::NVIC::DEVICE_ID deviceId) noexcept {

        const std::uint8_t  writeLimit = 31;

        if(deviceId < 32)
        {
            libs::MWR::setBit(ISER, 1 << (deviceId & writeLimit));

        } else if((deviceId < 64) && (deviceId > 31)){

           libs::MWR::setBit(ISER + 4, 1 << (deviceId & writeLimit));
        }
    }

    void NVIC::NVIC_SetPriority(drivers::nvic::NVIC::DEVICE_ID deviceId, std::uint32_t priority) noexcept {

        libs::MWR::setBit(IP + (deviceId - 4), static_cast<std::uint8_t>(priority << 4));

    }

}