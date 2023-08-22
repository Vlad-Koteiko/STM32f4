//
// Created by koteiko_vv on 25.07.2023.
//

#ifndef BASE_INTERFACE_H
#define BASE_INTERFACE_H

#include <bitset>

namespace base
{
    class BaseInterface
    {
    public:
        virtual void                    sendByte(std::byte byte) noexcept = 0;
        [[nodiscard]] virtual std::byte readByte() noexcept               = 0;
    };
}    // namespace base
#endif    // STM32F4_BASE_INTERFACE_H
