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
#include "usb.h"
#include <array>
#include <vector>
#include <memory_resource>

void EXTI0_IRQHandler();
void USART1_IRQHandler();
void OTG_FS_IRQHandler();

#endif
