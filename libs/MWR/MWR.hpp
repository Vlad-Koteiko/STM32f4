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

        template<class T>
              static inline void write_register(std::uintptr_t adress, volatile T value) noexcept
        {
            *reinterpret_cast<volatile T *>(adress) = value;
        }

        template<class T>
               static inline volatile T read_register(std::uintptr_t adress) noexcept
        {
            return *reinterpret_cast<volatile const T *>(adress);
        }

        template<class T>
       static void inline setBit(std::uintptr_t adress,T bit)
        {
           volatile T temp = read_register<T>(adress);
           temp |= bit;
           write_register(adress,temp);
        }

       static void inline clearBit(std::uintptr_t adress,std::uint32_t bit)
        {
            using type = std::uint32_t;
            volatile type temp = read_register<type>(adress);
            temp &= ~(bit);
            write_register(adress,temp);
        }
    };

}


#endif //BLINK_MWR_H
