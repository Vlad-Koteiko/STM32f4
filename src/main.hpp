//
// Created by koteiko_vv on 10.02.2023.
//
#ifndef UART_MAIN_H
#define UART_MAIN_H

#include <cstdint>

#include "MWR.hpp"
#include "basictimer.hpp"
#include "clockControl.hpp"
#include "cout.hpp"
#include "dma.hpp"
#include "exti.h"
#include "gpio.hpp"
#include "iwdg.h"
#include "nvic.h"
#include "syscfg.h"
#include "usart.hpp"

class Bass
{
public:
    void main() noexcept;
};

#endif
