//
// Created by koteiko_vv on 23.01.2023.
//
#include "usart.h"

namespace drivers::usart {

    USART::USART(drivers::clock::ClockControl &_clockControl, ADDRESSES_USART adr, std::uint8_t remap) : clockControl(_clockControl), baseAddress(adr)
    {
        switch (adr)
        {
            case USART1:
            {
                clockControl.APB2EnableClock(drivers::clock::USART1);
                RemapUsart1(remap);
                SetBaudRate(RATE_115200, clockControl.GetFreqAPB2());
                break;
            }
            case USART2:
            {
                clockControl.APB1EnableClock(drivers::clock::USART2);
                RemapUsart2(remap);
                SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
                break;
            }
            case USART3:
            {
                clockControl.APB1EnableClock(drivers::clock::USART3);
                RemapUsart3(remap);
                SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
                break;
            }
            case UART4:
            {
                clockControl.APB1EnableClock(drivers::clock::UART4);
                RemapUart4(remap);
                SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
                break;
            }

            case UART5:
            {
                clockControl.APB1EnableClock(drivers::clock::UART5);
                ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTD, drivers::port::PIN_12, drivers::port::PIN_2, drivers::port::AF8);
                SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
                break;
            }

            case USART6:
            {
                clockControl.APB2EnableClock(drivers::clock::USART6);
                RemapUsart6(remap);
                SetBaudRate(RATE_115200, clockControl.GetFreqAPB2());
                break;
            }
        }
        DataWidth(BIT_8);
        SetReceiver(RECEIVER_ON);
        SetTransmitter(TRANSMITTER_ON);
        UsartEnable(ON);
    }

    void USART::RemapUsart1(std::uint8_t remap)
    {
        switch (remap) {
            case U1_TX_PA9_RX_PA10:
                ConfigGpioForUart(drivers::port::PORTA, drivers::port::PORTA, drivers::port::PIN_9, drivers::port::PIN_10, drivers::port::AF7); break;
            case U1_TX_PA9_RX_PB7:
                ConfigGpioForUart(drivers::port::PORTA, drivers::port::PORTB, drivers::port::PIN_9, drivers::port::PIN_7, drivers::port::AF7); break;
            case U1_TX_PB6_RX_PA10:
                ConfigGpioForUart(drivers::port::PORTB, drivers::port::PORTA, drivers::port::PIN_6, drivers::port::PIN_10, drivers::port::AF7); break;
            case U1_TX_PB6_RX_PB7:
                ConfigGpioForUart(drivers::port::PORTB, drivers::port::PORTB, drivers::port::PIN_6, drivers::port::PIN_7, drivers::port::AF7); break;
        }
    }

    void USART::RemapUsart2(std::uint8_t remap)
    {
        switch (remap)
        {
            case U2_TX_PA2_RX_PA3:
                ConfigGpioForUart(drivers::port::PORTA, drivers::port::PORTA, drivers::port::PIN_2, drivers::port::PIN_3, drivers::port::AF7); break;
            case U2_TX_PA2_RX_PD6:
                ConfigGpioForUart(drivers::port::PORTA, drivers::port::PORTD, drivers::port::PIN_2, drivers::port::PIN_6, drivers::port::AF7); break;
            case U2_TX_PD5_RX_PA3:
                ConfigGpioForUart(drivers::port::PORTD, drivers::port::PORTA, drivers::port::PIN_5, drivers::port::PIN_3, drivers::port::AF7); break;
            case U2_TX_PD5_RX_PD6:
                ConfigGpioForUart(drivers::port::PORTD, drivers::port::PORTD, drivers::port::PIN_5, drivers::port::PIN_6, drivers::port::AF7); break;
        }
    }

    void USART::RemapUsart3(std::uint8_t remap) {
        switch (remap) {
            case U3_TX_PB10_RX_PB11:
                ConfigGpioForUart(drivers::port::PORTB, drivers::port::PORTB, drivers::port::PIN_10, drivers::port::PIN_11, drivers::port::AF7); break;
            case U3_TX_PB10_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTB, drivers::port::PORTC, drivers::port::PIN_10, drivers::port::PIN_11, drivers::port::AF7); break;
            case U3_TX_PB10_RX_PD9:
                ConfigGpioForUart(drivers::port::PORTB, drivers::port::PORTD, drivers::port::PIN_10, drivers::port::PIN_9, drivers::port::AF7); break;
            case U3_TX_PC10_RX_PB11:
                ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTB, drivers::port::PIN_10, drivers::port::PIN_11, drivers::port::AF7); break;
            case U3_TX_PC10_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTC, drivers::port::PIN_10, drivers::port::PIN_11, drivers::port::AF7); break;
            case U3_TX_PC10_RX_PD9:
                ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTD, drivers::port::PIN_10, drivers::port::PIN_9, drivers::port::AF7); break;
            case U3_TX_PD8_RX_PB11:
                ConfigGpioForUart(drivers::port::PORTD, drivers::port::PORTB, drivers::port::PIN_8, drivers::port::PIN_11, drivers::port::AF7); break;
            case U3_TX_PD8_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTD, drivers::port::PORTC, drivers::port::PIN_8, drivers::port::PIN_11, drivers::port::AF7); break;
            case U3_TX_PD8_RX_PD9:
                ConfigGpioForUart(drivers::port::PORTD, drivers::port::PORTC, drivers::port::PIN_8, drivers::port::PIN_9, drivers::port::AF7); break;
        }
    }

    void USART::RemapUart4(std::uint8_t remap)
    {
        switch (remap) {
            case U4_TX_PA0_RX_PA1:
                ConfigGpioForUart(drivers::port::PORTA, drivers::port::PORTA, drivers::port::PIN_0, drivers::port::PIN_1, drivers::port::AF8); break;
            case U4_TX_PA0_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTA, drivers::port::PORTC, drivers::port::PIN_0, drivers::port::PIN_11, drivers::port::AF8); break;
            case U4_TX_PC10_RX_PA1:
                ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTA, drivers::port::PIN_10, drivers::port::PIN_1, drivers::port::AF8); break;
            case U4_TX_PC10_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTC, drivers::port::PIN_10, drivers::port::PIN_11, drivers::port::AF8); break;
        }
    }

    void USART::RemapUsart6(std::uint8_t remap)
    {
        switch (remap) {
            case U6_TX_PC6_RX_PC7:
                ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTC, drivers::port::PIN_6, drivers::port::PIN_7, drivers::port::AF8); break;
            case U6_TX_PC6_RX_PG9:
                ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTG, drivers::port::PIN_6, drivers::port::PIN_9, drivers::port::AF8); break;
            case U6_TX_PG14_RX_PC7:
                ConfigGpioForUart(drivers::port::PORTG, drivers::port::PORTC, drivers::port::PIN_14, drivers::port::PIN_7, drivers::port::AF8); break;
            case U6_TX_PG14_RX_PG_9:
                ConfigGpioForUart(drivers::port::PORTG, drivers::port::PORTG, drivers::port::PIN_14, drivers::port::PIN_9, drivers::port::AF8); break;
        }
    }

    void USART::ConfigGpioForUart(drivers::port::ADDRESSES_PORT portTX, drivers::port::ADDRESSES_PORT portRX, drivers::port::PIN_NUMBER pinTX, drivers::port::PIN_NUMBER pinRX, drivers::port::ALTERNATE_FUNCTION af) noexcept
    {
        //init GPIO TX
        drivers::port::GPIO port_tx(clockControl, portTX);
        port_tx.SetPinMode(pinTX, drivers::port::ALTERNATE_FUNCT);
        port_tx.SetPinSpeed(pinTX, drivers::port::VERY_HIGH_SPEED);
        port_tx.SetAFPin(pinTX, af);

        //init GPIO RX
        drivers::port::GPIO port_rx(clockControl, portRX);
        port_rx.SetPinMode(pinRX, drivers::port::ALTERNATE_FUNCT);
        port_rx.SetPinSpeed(pinRX, drivers::port::VERY_HIGH_SPEED);
        port_rx.SetAFPin(pinRX, af);
    }

    void USART::DataWidth(WORD_LENGTH wordLength) noexcept     // This bit determines the word length. It is set or cleared by software.
    {
        readWrite::modifySetRegister(baseAddress + CR1, wordLength << M);
    }

    void USART::SetReceiver(RECEIVER receiver) noexcept           // This bit enables the receiver. It is set and cleared by software
    {
        readWrite::modifySetRegister(baseAddress + CR1, receiver << RE);
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

     void USART::SetBaudRate(BAUD_RATE baudRate, std::uint32_t FPCLK) noexcept
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

    void USART::DeInit() noexcept
    {
        switch (baseAddress) {
            case USART1:
                clockControl.DisablePeripherals(drivers::clock::USART_1_MODULE); break;
            case USART2:
                clockControl.DisablePeripherals(drivers::clock::USART_2_MODULE); break;
            case USART3:
                clockControl.DisablePeripherals(drivers::clock::USART_3_MODULE); break;
            case UART4:
                clockControl.DisablePeripherals(drivers::clock::UART_4_MODULE); break;
            case UART5:
                clockControl.DisablePeripherals(drivers::clock::UART_5_MODULE); break;
            case USART6:
                clockControl.DisablePeripherals(drivers::clock::USART_6_MODULE); break;
            case UART7:
                clockControl.DisablePeripherals(drivers::clock::UART_7_MODULE); break;
            case UART8:
                clockControl.DisablePeripherals(drivers::clock::UART_8_MODULE); break;
        }
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

    bool USART::ReadFlag_CTS() {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, CTS);
    }

    void USART::ClearFlag_CTS() {
        libs::MWR::resetBit(baseAddress + SR, CTS);
    }

    bool USART::ReadFlag_LBD() {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, LBD);
    }

    void USART::ClearFlag_LBD(){
        return libs::MWR::resetBit(baseAddress + SR, LBD);
    }

    bool USART::ReadFlag_TXE(){
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, TXE);
    }

    bool USART::ReadFlag_TC() {
        return  libs::MWR::readBit<std::uint32_t>(baseAddress + SR, TC);
    }

    void USART::ClearFlag_TC() {
        libs::MWR::resetBit(baseAddress + SR, TC);
    }

    bool USART::ReadFlag_RXNE() {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, RXNE);
    }

    void USART::ClearFlag_RXNE() {
        return libs::MWR::resetBit(baseAddress + SR, RXNE);
    }

    bool USART::ReadFlag_IDLE() {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, IDLE);
    }

    bool USART::ReadFlag_ORE() {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, ORE);
    }

    bool USART::ReadFlag_NF() {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, NF);
    }

    bool USART::ReadFlag_FE() {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, FE);
    }

    bool USART::ReadFlag_PE() {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, PE);
    }
}