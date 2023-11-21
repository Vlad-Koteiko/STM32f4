//
// Created by koteiko_vv on 17.11.2023.
//

#ifndef SHARED_H
#define SHARED_H
#include "cout.hpp"

namespace shared
{

    struct Data
    {
        static libs::Cout* getCout() noexcept;
        static void        setCout(libs::Cout*) noexcept;
    };
}    // namespace shared
#endif    // STM32F4_SHARED_H
