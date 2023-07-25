#pragma once

#include <ostream>
#include <streambuf>

#include "usart.hpp"

class uartbuf : public std::streambuf
{
    drivers::usart::Usart* uart;

public:
    explicit uartbuf(drivers::usart::Usart* uart);

    // write multiple characters  MUST USE CONST CHAR* ?

    int_type overflow(int_type c = traits_type::eof()) override;
};