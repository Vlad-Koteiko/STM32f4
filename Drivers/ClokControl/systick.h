#ifndef SYSTICK_H
#define SYSTICK_H

#include "MWR.hpp"

namespace drivers::systick
{
    enum CTRL_register : std::uint8_t
    {
        ENABLE_poz = 0,
        TICKINT_poz = 1,
        CLKSOURCE_poz = 2
    };

  class SysTick
  {
      static constexpr std::uintptr_t baseRegisterSysTick = 0xE000E010;
      enum RegisterSysTick : const std::uintptr_t
      {
          CTRL  = baseRegisterSysTick,          // Offset: 0x000 (R/W)  SysTick Control and Status Register
          LOAD  = baseRegisterSysTick + 0x04,   // Offset: 0x004 (R/W)  SysTick Reload Value Register
          VAL   = baseRegisterSysTick + 0x08,   // Offset: 0x008 (R/W)  SysTick Current Value Register
          CALIB = baseRegisterSysTick + 0x0C    // Offset: 0x00C (R/ )  SysTick Calibration Register
      };

public:
      /**
       *  \brief Конструктор таймера SysTick
       *  
       *  \param [in] HCLKFrequency Тактирование шины HCLK (Гц)
       *  \param [in] ticks Количестов тиков (24 бита)
       */
      SysTick(std::uint32_t HCLKFrequency, std::uint32_t ticks);
      void SetClockSourceAHB();
      void SetClockSourceAHB8();
      void EnableInterrupt();
      void DisableInterrupt();
      void Start();
      void Stop();
      void SetValue(uint32_t val);
      void SetLoad(uint32_t val);
  };
}

#endif //SYSTICK_H