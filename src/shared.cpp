//
// Created by koteiko_vv on 21.11.2023.
//
#include "shared.hpp"

namespace shared
{

    struct Point
    {
        libs::Cout            *cout;
        drivers::usart::Usart *usart;
    };

    static Point point;

    libs::Cout *Data::getCout() noexcept
    {
        return point.cout;
    }

    void Data::setCout(libs::Cout *cout) noexcept
    {
        point.cout = cout;
    }

}    // namespace shared