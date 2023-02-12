//
// Created by koteiko_vv on 09.02.2023.
//
#include "exti.h"

namespace drivers::exti {

    void EXIT_RTSR::SetLineTrigger(drivers::exti::EXIT_RTSR::LINE_NUMBER lineNumber) noexcept {

        libs::MWR::clearBit(EMR, 1 << lineNumber);
        libs::MWR::setBit(IMR, 1 << lineNumber);

        libs::MWR::clearBit(FTSR, 1 << lineNumber);
        libs::MWR::setBit(RTSR, 1 << lineNumber);
    }

    void EXIT_RTSR::ClearFlag(drivers::exti::EXIT_RTSR::LINE_NUMBER lineNumber) noexcept {

        libs::MWR::clearBit(PR, 0 << lineNumber);

    }

}
