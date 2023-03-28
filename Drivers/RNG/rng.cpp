
#include "rng.h"

namespace drivers::rng
{
    RNG::RNG(drivers::clock::ClockControl &clockControl) : clockControl(clockControl){
        clockControl.EnablePeripherals(drivers::clock::RNG_MODULE);
    }

    void RNG::Enable() noexcept {
        libs::MWR::setBit(CR, RNGREN);
    }

    void RNG::Disable() noexcept {
        libs::MWR::resetBit(CR, RNGREN);
    }

    bool RNG::IsEnabled() noexcept {
        return libs::MWR::readBit<std::uint32_t>(CR, RNGREN);
    }

    bool RNG::IsActiveFlag_DRDY() noexcept {
        return libs::MWR::readBit<std::uint32_t>(SR, DRDY);
    }

    bool RNG::IsActiveFlag_CECS() noexcept {
        return libs::MWR::readBit<std::uint32_t>(SR, CECS);
    }

    bool RNG::IsActiveFlag_SECS() noexcept {
        return libs::MWR::readBit<std::uint32_t>(SR, SECS);
    }

    bool RNG::IsActiveFlag_CEIS() noexcept {
        return libs::MWR::readBit<std::uint32_t>(SR, CEIS);
    }

    bool RNG::IsActiveFlag_SEIS() noexcept {
        return libs::MWR::readBit<std::uint32_t>(SR, SEIS);
    }

    void RNG::ClearFlag_CEIS() noexcept {
        libs::MWR::resetBit(SR, CEIS);
    }

    void RNG::ClearFlag_SEIS() noexcept {
        libs::MWR::resetBit(SR, SEIS);
    }

    void RNG::EnableIT() noexcept {
        libs::MWR::setBit(CR, IE);
    }

    void RNG::DisableIT() noexcept {
        libs::MWR::resetBit(CR, IE);
    }

    void RNG::IsEnabledIT() noexcept {
        libs::MWR::readBit<std::uint32_t>(CR, IE);
    }

    std::uint32_t RNG::ReadRandData32() noexcept {
        return libs::MWR::read_register<std::uint32_t>(DR);
    }
}