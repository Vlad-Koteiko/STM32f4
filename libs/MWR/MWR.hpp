//
// Created by koteiko_vv on 06.12.2022.
//
#ifndef MWR_HPP
#define MWR_HPP

#include <cstdint>

namespace  libs
{
    class MWR
    {

    public:
        template<typename T>
        static inline void write_register(std::uintptr_t adress, volatile T value) noexcept
        {
            *reinterpret_cast<volatile std::uint32_t*>(adress) = static_cast<std::uint32_t>(value);
        }

        template<typename T>
        static inline volatile T read_register(std::uintptr_t adress) noexcept
        {
            return *reinterpret_cast<volatile const T *>(adress);
        }

        template<typename T>
        static void inline modifySetRegister(std::uintptr_t adress,T mask) noexcept
        {
           volatile std::uint32_t temp = read_register<std::uint32_t>(adress);
           temp |= mask;
           write_register(adress,temp);
        }

        static void inline modifyResetRegister(std::uintptr_t adress, std::uint32_t mask) noexcept
        {
            using type = std::uint32_t;
            volatile type temp = read_register<type>(adress);
            temp &= ~(mask);
            write_register(adress,temp);
        }

        static void inline setBit(std::uintptr_t adress, std::uint8_t numberBit) noexcept
        {
            modifySetRegister(adress,(1 << numberBit));
        }

        static void inline resetBit(std::uintptr_t adress, std::uint8_t numberBit) noexcept
        {
            modifyResetRegister(adress,(1 << numberBit));
        }

        template<typename T>
        static bool inline readBit(std::uintptr_t adress, std::uint8_t numberBit) noexcept
        {
            return (read_register<T>(adress) & (1 << numberBit));
        }
    };

}


#endif //BLINK_MWR_H
