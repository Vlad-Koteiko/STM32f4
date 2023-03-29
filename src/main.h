//
// Created by koteiko_vv on 10.02.2023.
//
#ifndef UART_MAIN_H
#define UART_MAIN_H

#include <cstdint>
#include "MWR.hpp"
#include "cout.hpp"
#include "clockControl.hpp"
#include "gpio.h"
#include "usart.h"
#include "nvic.h"
#include "syscfg.h"
#include "exti.h"
#include "iwdg.h"

void EXTI0_IRQHandler();
void TIM6_DAC_IRQHandler();
void USART2_IRQHandler();
#endif
