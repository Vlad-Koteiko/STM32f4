//
// Created by koteiko_vv on 09.02.2023.
//
#include "syscfg.h"

namespace drivers::syscfg {

    SYSCFG::SYSCFG(ClockControl &clockControl1) : clockControl(clockControl1) {
        clockControl.module_enable(clockControl.SYSCF_MODULE);
    }

    void SYSCFG::SetSourceEXTI(drivers::syscfg::SYSCFG::EXIT_PORT exitPort,
                               drivers::syscfg::SYSCFG::EXIT_NUMBER exitNumber) noexcept {

        if (exitNumber < 4)
        {
            libs::MWR::setBit(EXTICR, exitPort << (exitNumber * 4));

        } else if((exitNumber < 8) && (exitNumber > 3))
        {
            libs::MWR::setBit(EXTICR_2, (exitPort << ((exitNumber - 4) * 4)));

        } else if((exitNumber < 12) && (exitNumber > 7))
        {
            libs::MWR::setBit(EXTICR + 8, exitPort << ((exitNumber - 8) * 4));

        } else if((exitNumber < 16) && (exitNumber > 11))
        {
            libs::MWR::setBit(EXTICR + 12, exitPort << ((exitNumber - 12) * 4));
        }
    }

}