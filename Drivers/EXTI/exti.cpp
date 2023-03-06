//
// Created by koteiko_vv on 09.02.2023.
//
#include "exti.h"

namespace drivers::exti {

    void EXIT_RTSR::SetLineTrigger(drivers::exti::EXIT_RTSR::LINE_NUMBER lineNumber) noexcept {

        libs::MWR::modifyResetRegister(EMR, 1 << lineNumber);
        libs::MWR::modifySetRegister(IMR, 1 << lineNumber);

        libs::MWR::modifyResetRegister(FTSR, 1 << lineNumber);
        libs::MWR::modifySetRegister(RTSR, 1 << lineNumber);
    }

    void EXIT_RTSR::ClearFlag(drivers::exti::EXIT_RTSR::LINE_NUMBER lineNumber) noexcept {

        libs::MWR::modifyResetRegister(PR, 0 << lineNumber);

    }

}
