//
// Created by koteiko_vv on 17.02.2023.
//

#include "flash.h"

namespace drivers::flash
{
    void Flash::SetLatency(std::uint8_t latency) noexcept {
        libs::MWR::setBit(ACR,latency);
    }

    std::uint8_t Flash::GetLatency() noexcept {
        return (libs::MWR::read_register<std::uint32_t>(ACR) & 7);
    }

    void Flash::InstructionCacheEnable() noexcept {
        libs::MWR::enableNumberBit(ACR,9);
    }

    void Flash::DataCacheEnable() noexcept {
        libs::MWR::enableNumberBit(ACR,10);
    }

    void Flash::PrefetchBufferEnable() noexcept {
        libs::MWR::enableNumberBit(ACR,8);
    }
}