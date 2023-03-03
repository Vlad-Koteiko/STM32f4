//
// Created by komiak_ap on 01.03.2023.
//

#ifndef STM32F4_IWDG_H
#define STM32F4_IWDG_H

#include <chrono>

#include "MWR.hpp"

/**
namespace drivers::iwdg
@brief Драйвер Watchdog-таймера
*/

namespace drivers::iwdg
{
    /**
     @brief Перечисление делителей для Watchdog-таймера
     */

    enum PrescalerIWDG : std::uint8_t
    {
        IWDG_PRESCALER_4    = 0b000,    //divider /4
        IWDG_PRESCALER_8    = 0b001,    //divider /8
        IWDG_PRESCALER_16   = 0b010,    //divider /16
        IWDG_PRESCALER_32   = 0b011,    //divider /32
        IWDG_PRESCALER_64   = 0b100,    //divider /64
        IWDG_PRESCALER_128  = 0b101,    //divider /128
        IWDG_PRESCALER_256  = 0b110,    //divider /256
        //IWDH_PRESCALER_256  = 0b111    //divider /256
    };

    /**
     @brief Класс реализующий функционал Watchdog-таймера
     */
    class IWDG {

        static constexpr std::uint32_t baseAddress = 0x40003000;
        enum RegisterIWDG : std::uintptr_t
        {
            IWDG_KR     = baseAddress,          //Key register,         Address offset: 0x00
            IWDG_PR     = baseAddress + 0x04,   //Prescaler register,   Address offset: 0x04
            IWDG_RLR    = baseAddress + 0x08,   //Reload register,      Address offset: 0x08
            IWDG_SR     = baseAddress + 0x0C    //Status register       Address offset: 0x0C
        };

        enum KeyIWDG : std::uint32_t
        {
            KEY_RELOAD              = 0x0000AAAAU,  //IWDG Reload Counter Enable
            KEY_ENABLE              = 0x0000CCCCU,  //IWDG Peripheral Enable
            KEY_WR_ACCESS_ENABLE    = 0x00005555U,  //IWDG KR Write Access Enable
            KEY_WR_ACCESS_DISABLE   = 0x00000000U,  //IWDG KR Write Access Disable
        };

        enum StatusRegPoz : std::uint8_t
        {
            PVU = 0,
            RVU = 1
        };

    public:
        /**
         * @brief Конструктор
         * @param prescaller Значение делителя
         * @param reloadValue Значение перезагрузки
         */
        IWDG(PrescalerIWDG prescaller, std::int16_t reloadValue);
        /**
         * @brief Конструктор
         * @param period Время периода Watchdog-таймера
         */
        IWDG(std::chrono::milliseconds period);
        /**
         * Обновить обратный отсчет watchdog-таймера
         */
        void Refresh();
        /**
         * Включение разрешение записи в регистры
         */
        void EnableWriteAccess();
        /**
         * Отключение разрешения записи в регистры
         */
        void DisableWriteAccess();
        /**
         * Установка делителя
         * @param pres Значение делителя
         */
        void SetPrescaler(PrescalerIWDG pres);
        /**
         * Получение значение делителя из регистра
         * @return значение делителя (bin)
         */
        std::uint32_t GetPrescaler();
        /**
         * Установка значения перезагрузки
         * @param counter
         */
        void SetReloadCounter(std::uint32_t counter);
        std::uint32_t GetReloadCounter();
        bool IsActiveFlagPVU();
        bool IsActiveFlagRVU();
        bool IsReady();
    };
}

#endif //STM32F4_IWDG_H
