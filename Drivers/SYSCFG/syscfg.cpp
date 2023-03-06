//
// Created by koteiko_vv on 09.02.2023.
//
#include "syscfg.h"

namespace drivers::syscfg {

    SYSCFG::SYSCFG(drivers::clock::ClockControl &clockControl1) : clockControl(clockControl1) {
        clockControl.module_enable(drivers::clock::SYSCF_MODULE);
    }

    void SYSCFG::SetSourceEXTI(drivers::syscfg::SYSCFG::EXIT_PORT exitPort,
                               drivers::syscfg::SYSCFG::EXIT_NUMBER exitNumber) noexcept {

        if (exitNumber < 4)
        {
            libs::MWR::modifySetRegister(EXTICR, exitPort << (exitNumber * 4));

        } else if((exitNumber < 8) && (exitNumber > 3))
        {
            libs::MWR::modifySetRegister(EXTICR_2, (exitPort << ((exitNumber - 4) * 4)));

        } else if((exitNumber < 12) && (exitNumber > 7))
        {
            libs::MWR::modifySetRegister(EXTICR + 8, exitPort << ((exitNumber - 8) * 4));

        } else if((exitNumber < 16) && (exitNumber > 11))
        {
            libs::MWR::modifySetRegister(EXTICR + 12, exitPort << ((exitNumber - 12) * 4));
        }
    }

}