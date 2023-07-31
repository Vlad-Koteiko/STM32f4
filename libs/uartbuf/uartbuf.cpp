#include "uartbuf.hpp"

uartbuf::uartbuf(drivers::usart::Usart *uart) : uart(uart)
{
    // no buffering, overflow on every char
    setp(nullptr, nullptr);
}

int uartbuf::overflow(int c)
{
    // add the char to wherever you want it, for example:
    // DebugConsole.setText(DebugControl.text() + c);
    if(c == traits_type::eof())
    {
        return traits_type::eof();
    }
    while(!uart->ReadFlag(drivers::usart::Usart::SR_poz::TXE))
    {
    }
    uart->sendByte(static_cast<std::byte>(c));
    return c;
}
