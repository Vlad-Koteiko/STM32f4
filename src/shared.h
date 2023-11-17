//
// Created by koteiko_vv on 17.11.2023.
//

#ifndef SHARED_H
#define SHARED_H

#include "cout.hpp"

class SharedData
{
public:
    libs::Cout*            cout;
    drivers::usart::Usart* usart;
};

static SharedData sharedData;

#endif    // STM32F4_SHARED_H
