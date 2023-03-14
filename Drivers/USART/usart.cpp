//
// Created by koteiko_vv on 23.01.2023.
//
#include "usart.h"

namespace drivers::usart {

    USART::USART(drivers::clock::ClockControl &_clockControl, ADDRESSES_USART adr) : clockControl(_clockControl), baseAddress(adr)
    {
        switch (adr)
        {
            case USART1:
            {
                drivers::port::GPIO portA(clockControl, drivers::port::PORT_A);
                clockControl.APB2EnableClock(drivers::clock::USART1);

                portA.SetPinMode(drivers::port::PIN_9,drivers::port::ALTERNATE_FUNCT);
                portA.SetPinMode(drivers::port::PIN_10, drivers::port::ALTERNATE_FUNCT);
                portA.SetPinSpeed(drivers::port::PIN_9, drivers::port::VERY_HIGH_SPEED);
                portA.SetPinSpeed(drivers::port::PIN_10, drivers::port::VERY_HIGH_SPEED);
                portA.SetAFPin(drivers::port::PIN_9, drivers::port::AF7);
                portA.SetAFPin(drivers::port::PIN_10, drivers::port::AF7);

                DataWidth(BIT_8);
                SetReceiver(RECEIVER_ON);
                SetTransmitter(TRANSMITTER_ON);
                SetBaudRate(RATE_115200, clockControl.GetFreqAPB2());
                UsartEnable(ON);

                break;
            }
            case USART2:
            {
                drivers::port::GPIO portA(clockControl, drivers::port::PORT_A);
                clockControl.APB1EnableClock(drivers::clock::USART2);

                portA.SetPinMode(drivers::port::PIN_2,drivers::port::ALTERNATE_FUNCT);
                portA.SetPinMode(drivers::port::PIN_3, drivers::port::ALTERNATE_FUNCT);
                portA.SetPinSpeed(drivers::port::PIN_2, drivers::port::VERY_HIGH_SPEED);
                portA.SetPinSpeed(drivers::port::PIN_3, drivers::port::VERY_HIGH_SPEED);
                portA.SetAFPin(drivers::port::PIN_2, drivers::port::AF7);
                portA.SetAFPin(drivers::port::PIN_3, drivers::port::AF7);

                DataWidth(BIT_8);
                SetReceiver(RECEIVER_ON);
                SetTransmitter(TRANSMITTER_ON);
                std::uint32_t  f = clockControl.GetFreqAPB1();
                SetBaudRate(RATE_115200, 42000000);
                //SetBaudRate(RATE_115200, 42000000);
                UsartEnable(ON);

                break;
            }
        }
    }

    void USART::DataWidth(WORD_LENGTH wordLength) noexcept     // This bit determines the word length. It is set or cleared by software.
    {
        readWrite::modifySetRegister(baseAddress + CR1, wordLength << M);
        //libs::MWR::setBit(0x4001100C, M);
    }

    void USART::SetReceiver(RECEIVER receiver) noexcept           // This bit enables the receiver. It is set and cleared by software
    {
        readWrite::modifySetRegister(baseAddress + CR1, receiver << RE);
        //readWrite::modifySetRegister(0x4001100C, receiver << RE);
    }

    void USART::SetTransmitter(TRANSMITTER transmitter) noexcept  // This bit enables the transmitter. It is set and cleared by software
    {
        readWrite::modifySetRegister(baseAddress + CR1, transmitter << TE);
    }

    void USART::UsartEnable(USART_ENABLE usartEnable) noexcept   // USART prescalers and outputs are stopped and the end of the current byte transfer in order to reduce power consumption
    {
        USART::readWrite::modifySetRegister(baseAddress + CR1, usartEnable << UE);
    }

    void USART::SetStopBitsLength(STOP_BIT stopBit) noexcept      // These bits are used for programming the stop bits.
    {
        readWrite::modifySetRegister(baseAddress + CR2, stopBit << STOP);
    }

     void USART::SetBaudRate(BAUD_RATE baudRate, std::uint32_t FPCLK)
    {
        float resalt = (static_cast<float>(FPCLK)/static_cast<float>(16 * baudRate));
        std::uint16_t integet = static_cast<std::uint16_t>(resalt);
        std::uint16_t fraction = (static_cast<float>(resalt -  static_cast<float>(integet)) * 16);
        libs::MWR::write_register(baseAddress + BRR,(integet << 4) | (fraction & 0x000F));
    }


    void USART::TransmitData(std::uint8_t value) noexcept
    {
        readWrite::write_register(baseAddress + DR, value);
    }

    void USART::TransmitString(void* value, std::size_t size) noexcept
    {
        std::size_t temp = 0;
        while (size--)
        {
            while (!IsBusyFlag())
            {}
            TransmitData(*(static_cast<char *>(value) + temp++));
        }
    }

    std::uint8_t USART::ReceiveData() noexcept
    {
        return readWrite::read_register<std::uint8_t>(baseAddress + DR);
    }

    bool USART::IsBusyFlag() noexcept
    {
        return readWrite::readBit<std::uint32_t>(baseAddress + SR,TXE);
    }

    void USART::uartInit(BAUD_RATE baudRate,std::uint32_t FPCLK) noexcept
    {
        DataWidth(BIT_8);
        SetReceiver(RECEIVER_ON);
        SetTransmitter(TRANSMITTER_ON);
        SetBaudRate(baudRate,FPCLK);
        USART_ENABLE(USART_ENABLE_ON);
    }

    void USART::EnableInterrupt()
    {
        switch (baseAddress) {
            case USART1:
            {
                drivers::nvic::NVIC::NVIC_Enable(drivers::nvic::NVIC::DEVICE_ID::UASRT_1);
                break;
            }

            case USART2:
            {
                drivers::nvic::NVIC::NVIC_Enable(drivers::nvic::NVIC::DEVICE_ID::UASRT_2);
                break;
            }
        }

        libs::MWR::setBit(baseAddress + CR1,5);
        libs::MWR::setBit(baseAddress + CR1,8);
        libs::MWR::setBit(baseAddress + CR3,8);
    }
}