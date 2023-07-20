//
// Created by koteiko_vv on 23.01.2023.
//
#include "usart.hpp"

namespace drivers::usart
{

    //    USART::USART(drivers::clock::ClockControl &_clockControl, ADDRESSES_USART adr,
    //    std::uint8_t remap) : clockControl(_clockControl), baseAddress(adr)
    //    {
    //        switch (adr)
    //        {
    //            case USART1:
    //            {
    //                clockControl.EnablePeripherals(drivers::clock::CONSTANTS::USART1_MODULE);
    //                RemapUsart1(remap);
    //                SetBaudRate(RATE_115200, clockControl.GetFreqAPB2());
    //                break;
    //            }
    //            case USART2:
    //            {
    //                clockControl.EnablePeripherals(drivers::clock::CONSTANTS::USART2_MODULE);
    //                RemapUsart2(remap);
    //                SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
    //                break;
    //            }
    //            case USART3:
    //            {
    //                clockControl.EnablePeripherals(drivers::clock::CONSTANTS::USART3_MODULE);
    //                RemapUsart3(remap);
    //                SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
    //                break;
    //            }
    //            case UART4:
    //            {
    //                clockControl.EnablePeripherals(drivers::clock::CONSTANTS::UART4_MODULE);
    //                RemapUart4(remap);
    //                SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
    //                break;
    //            }
    //
    //            case UART5:
    //            {
    //                clockControl.EnablePeripherals(drivers::clock::CONSTANTS::UART5_MODULE);
    //                ConfigGpioForUart(drivers::port::PORTC, drivers::port::PORTD,
    //                drivers::port::PIN_12, drivers::port::PIN_2, drivers::port::AF8);
    //                SetBaudRate(RATE_115200, clockControl.GetFreqAPB1());
    //                break;
    //            }
    //
    //            case USART6:
    //            {
    //                clockControl.EnablePeripherals(drivers::clock::CONSTANTS::USART6_MODULE);
    //                RemapUsart6(remap);
    //                SetBaudRate(RATE_115200, clockControl.GetFreqAPB2());
    //                break;
    //            }
    //        }
    //        SetWordLength(BIT_8);
    //        ReceiverEnable(ENABLE);
    //        TransmitterEnable(ENABLE);
    //        UsartEnable(ENABLE);
    //    }

    void USART::RemapUsart1(USART1_Remap remap)
    {
        switch(remap)
        {
            case U1_TX_PA9_RX_PA10:
                ConfigGpioForUart(drivers::port::PORTA,
                                  drivers::port::PORTA,
                                  drivers::port::PIN_9,
                                  drivers::port::PIN_10,
                                  drivers::port::AF7);
                break;
            case U1_TX_PA9_RX_PB7:
                ConfigGpioForUart(drivers::port::PORTA,
                                  drivers::port::PORTB,
                                  drivers::port::PIN_9,
                                  drivers::port::PIN_7,
                                  drivers::port::AF7);
                break;
            case U1_TX_PB6_RX_PA10:
                ConfigGpioForUart(drivers::port::PORTB,
                                  drivers::port::PORTA,
                                  drivers::port::PIN_6,
                                  drivers::port::PIN_10,
                                  drivers::port::AF7);
                break;
            case U1_TX_PB6_RX_PB7:
                ConfigGpioForUart(drivers::port::PORTB,
                                  drivers::port::PORTB,
                                  drivers::port::PIN_6,
                                  drivers::port::PIN_7,
                                  drivers::port::AF7);
                break;
        }
    }

    void USART::RemapUsart2(USART2_Remap remap)
    {
        switch(remap)
        {
            case U2_TX_PA2_RX_PA3:
                ConfigGpioForUart(drivers::port::PORTA,
                                  drivers::port::PORTA,
                                  drivers::port::PIN_2,
                                  drivers::port::PIN_3,
                                  drivers::port::AF7);
                break;
            case U2_TX_PA2_RX_PD6:
                ConfigGpioForUart(drivers::port::PORTA,
                                  drivers::port::PORTD,
                                  drivers::port::PIN_2,
                                  drivers::port::PIN_6,
                                  drivers::port::AF7);
                break;
            case U2_TX_PD5_RX_PA3:
                ConfigGpioForUart(drivers::port::PORTD,
                                  drivers::port::PORTA,
                                  drivers::port::PIN_5,
                                  drivers::port::PIN_3,
                                  drivers::port::AF7);
                break;
            case U2_TX_PD5_RX_PD6:
                ConfigGpioForUart(drivers::port::PORTD,
                                  drivers::port::PORTD,
                                  drivers::port::PIN_5,
                                  drivers::port::PIN_6,
                                  drivers::port::AF7);
                break;
        }
    }

    void USART::RemapUsart3(USART3_Remap remap)
    {
        switch(remap)
        {
            case U3_TX_PB10_RX_PB11:
                ConfigGpioForUart(drivers::port::PORTB,
                                  drivers::port::PORTB,
                                  drivers::port::PIN_10,
                                  drivers::port::PIN_11,
                                  drivers::port::AF7);
                break;
            case U3_TX_PB10_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTB,
                                  drivers::port::PORTC,
                                  drivers::port::PIN_10,
                                  drivers::port::PIN_11,
                                  drivers::port::AF7);
                break;
            case U3_TX_PB10_RX_PD9:
                ConfigGpioForUart(drivers::port::PORTB,
                                  drivers::port::PORTD,
                                  drivers::port::PIN_10,
                                  drivers::port::PIN_9,
                                  drivers::port::AF7);
                break;
            case U3_TX_PC10_RX_PB11:
                ConfigGpioForUart(drivers::port::PORTC,
                                  drivers::port::PORTB,
                                  drivers::port::PIN_10,
                                  drivers::port::PIN_11,
                                  drivers::port::AF7);
                break;
            case U3_TX_PC10_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTC,
                                  drivers::port::PORTC,
                                  drivers::port::PIN_10,
                                  drivers::port::PIN_11,
                                  drivers::port::AF7);
                break;
            case U3_TX_PC10_RX_PD9:
                ConfigGpioForUart(drivers::port::PORTC,
                                  drivers::port::PORTD,
                                  drivers::port::PIN_10,
                                  drivers::port::PIN_9,
                                  drivers::port::AF7);
                break;
            case U3_TX_PD8_RX_PB11:
                ConfigGpioForUart(drivers::port::PORTD,
                                  drivers::port::PORTB,
                                  drivers::port::PIN_8,
                                  drivers::port::PIN_11,
                                  drivers::port::AF7);
                break;
            case U3_TX_PD8_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTD,
                                  drivers::port::PORTC,
                                  drivers::port::PIN_8,
                                  drivers::port::PIN_11,
                                  drivers::port::AF7);
                break;
            case U3_TX_PD8_RX_PD9:
                ConfigGpioForUart(drivers::port::PORTD,
                                  drivers::port::PORTC,
                                  drivers::port::PIN_8,
                                  drivers::port::PIN_9,
                                  drivers::port::AF7);
                break;
        }
    }

    void USART::RemapUart4(UART4_Remap remap)
    {
        switch(remap)
        {
            case U4_TX_PA0_RX_PA1:
                ConfigGpioForUart(drivers::port::PORTA,
                                  drivers::port::PORTA,
                                  drivers::port::PIN_0,
                                  drivers::port::PIN_1,
                                  drivers::port::AF8);
                break;
            case U4_TX_PA0_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTA,
                                  drivers::port::PORTC,
                                  drivers::port::PIN_0,
                                  drivers::port::PIN_11,
                                  drivers::port::AF8);
                break;
            case U4_TX_PC10_RX_PA1:
                ConfigGpioForUart(drivers::port::PORTC,
                                  drivers::port::PORTA,
                                  drivers::port::PIN_10,
                                  drivers::port::PIN_1,
                                  drivers::port::AF8);
                break;
            case U4_TX_PC10_RX_PC11:
                ConfigGpioForUart(drivers::port::PORTC,
                                  drivers::port::PORTC,
                                  drivers::port::PIN_10,
                                  drivers::port::PIN_11,
                                  drivers::port::AF8);
                break;
        }
    }

    void USART::RemapUsart6(USART6_Remap remap)
    {
        switch(remap)
        {
            case U6_TX_PC6_RX_PC7:
                ConfigGpioForUart(drivers::port::PORTC,
                                  drivers::port::PORTC,
                                  drivers::port::PIN_6,
                                  drivers::port::PIN_7,
                                  drivers::port::AF8);
                break;
            case U6_TX_PC6_RX_PG9:
                ConfigGpioForUart(drivers::port::PORTC,
                                  drivers::port::PORTG,
                                  drivers::port::PIN_6,
                                  drivers::port::PIN_9,
                                  drivers::port::AF8);
                break;
            case U6_TX_PG14_RX_PC7:
                ConfigGpioForUart(drivers::port::PORTG,
                                  drivers::port::PORTC,
                                  drivers::port::PIN_14,
                                  drivers::port::PIN_7,
                                  drivers::port::AF8);
                break;
            case U6_TX_PG14_RX_PG_9:
                ConfigGpioForUart(drivers::port::PORTG,
                                  drivers::port::PORTG,
                                  drivers::port::PIN_14,
                                  drivers::port::PIN_9,
                                  drivers::port::AF8);
                break;
        }
    }

    void USART::Remap(std::uint8_t remapPins)
    {
        switch(this->baseAddress)
        {
            case USART1:
                RemapUsart1(static_cast<USART1_Remap>(remapPins));
                break;

            case USART2:
                RemapUsart2(static_cast<USART2_Remap>(remapPins));
                break;

            case USART3:
                RemapUsart3(static_cast<USART3_Remap>(remapPins));
                break;

            case UART4:
                RemapUart4(static_cast<UART4_Remap>(remapPins));
                break;

            case USART6:
                RemapUsart6(static_cast<USART6_Remap>(remapPins));
                break;

            default:
                break;
        }
    }

    void USART::ConfigGpioForUart(drivers::port::ADDRESSES_PORT     portTX,
                                  drivers::port::ADDRESSES_PORT     portRX,
                                  drivers::port::PIN_NUMBER         pinTX,
                                  drivers::port::PIN_NUMBER         pinRX,
                                  drivers::port::ALTERNATE_FUNCTION af) noexcept
    {
        // init GPIO TX
        drivers::port::GPIO port_tx(clockControl, portTX);
        port_tx.SetPinMode(pinTX, drivers::port::ALTERNATE_FUNCT);
        port_tx.SetPinSpeed(pinTX, drivers::port::VERY_HIGH_SPEED);
        port_tx.SetAFPin(pinTX, af);

        // init GPIO RX
        drivers::port::GPIO port_rx(clockControl, portRX);
        port_rx.SetPinMode(pinRX, drivers::port::ALTERNATE_FUNCT);
        port_rx.SetPinSpeed(pinRX, drivers::port::VERY_HIGH_SPEED);
        port_rx.SetAFPin(pinRX, af);
    }

    void USART::SetWordLength(
        WORD_LENGTH wordLength) noexcept    // This bit determines the word length. It is set or
                                            // cleared by software.
    {
        if(wordLength == BIT_8)
            libs::MWR::resetBit(baseAddress + CR1, M);
        else
            libs::MWR::setBit(baseAddress + CR1, M);
    }

    void USART::ReceiverEnable(STATUS receiver) noexcept    // This bit enables the receiver. It is
                                                            // set and cleared by software
    {
        if(receiver == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, RE);
        else
            libs::MWR::setBit(baseAddress + CR1, RE);
    }

    void USART::TransmitterEnable(
        STATUS transmitter) noexcept    // This bit enables the transmitter. It is set and cleared
                                        // by software
    {
        if(transmitter == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, TE);
        else
            libs::MWR::setBit(baseAddress + CR1, TE);
    }

    void USART::UsartEnable(
        STATUS
            usartEnable) noexcept    // USART prescalers and outputs are stopped and the end of the
                                     // current byte transfer in order to reduce power consumption
    {
        if(usartEnable == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, UE);
        else
            libs::MWR::setBit(baseAddress + CR1, UE);
    }

    void USART::SetStopBitsLength(
        STOP_BIT stopBit) noexcept    // These bits are used for programming the stop bits.
    {
        //        std::uint32_t bufReg = readWrite::read_register<std::uint32_t>(baseAddress + CR2);
        //        bufReg = (bufReg & 0xFFFCFFF) | (stopBit << CR2_poz::STOP);
        //        readWrite::write_register<std::uint32_t>(baseAddress + CR2, bufReg);

        libs::MWR::modifyResetRegister(baseAddress + CR2, 3 << STOP);
        libs::MWR::modifySetRegister(baseAddress + CR2, stopBit << STOP);
    }

    void USART::SetBaudRate(BAUD_RATE baudRate, std::uint32_t FPCLK) noexcept
    {
        float         resalt   = (static_cast<float>(FPCLK) / static_cast<float>(16 * baudRate));
        std::uint16_t integet  = static_cast<std::uint16_t>(resalt);
        std::uint16_t fraction = (static_cast<float>(resalt - static_cast<float>(integet)) * 16);
        libs::MWR::write_register(baseAddress + BRR,
                                  (integet << DIV_Mantissa) | (fraction & 0x000F));
    }

    void USART::TransmitData(std::uint8_t value) noexcept
    {
        libs::MWR::write_register(baseAddress + DR, value);
    }

    void USART::TransmitString(const void *value, std::size_t size) noexcept
    {
        std::size_t temp = 0;
        while(size--)
        {
            while(!ReadFlag(TXE))
            {
            }
            TransmitData(*(static_cast<const char *>(value) + temp++));
        }
    }

    std::uint8_t USART::ReceiveData() noexcept
    {
        return libs::MWR::read_register<std::uint8_t>(baseAddress + DR);
    }

    void USART::DeInit() noexcept
    {
        switch(baseAddress)
        {
            case USART1:
                clockControl.EnablePeripherals(drivers::clock::constants::USART1_MODULE);
                break;
            case USART2:
                clockControl.EnablePeripherals(drivers::clock::constants::USART2_MODULE);
                break;
            case USART3:
                clockControl.EnablePeripherals(drivers::clock::constants::USART3_MODULE);
                break;
            case UART4:
                clockControl.EnablePeripherals(drivers::clock::constants::UART4_MODULE);
                break;
            case UART5:
                clockControl.EnablePeripherals(drivers::clock::constants::UART5_MODULE);
                break;
            case USART6:
                clockControl.EnablePeripherals(drivers::clock::constants::USART6_MODULE);
                break;
            case UART7:
                clockControl.EnablePeripherals(drivers::clock::constants::UART7_MODULE);
                break;
            case UART8:
                clockControl.EnablePeripherals(drivers::clock::constants::UART8_MODULE);
                break;
        }
    }

    //---------------------------------------------------------------------------------
    bool USART::ReadFlag(SR_poz flag)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, flag);
    }

    void USART::ClearFlag(SR_clear_flag flag)
    {
        libs::MWR::resetBit(baseAddress + SR, CTS);
    }
    //---------------------------------------------------------------------------------

    void USART::SetOversamplingMode(std::uint8_t mode)
    {
        if(mode == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, OVER8);
        else
            libs::MWR::setBit(baseAddress + CR1, OVER8);
    }

    bool USART::GetOversamplingMode()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, OVER8);
    }

    void USART::SetWakeUpMethod(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR1, WAKE);
        else
            libs::MWR::setBit(baseAddress + CR1, WAKE);
    }

    bool USART::GetWakeUpMethod()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, WAKE);
    }

    void USART::SetParityControl(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, PCE);
        else
            libs::MWR::setBit(baseAddress + CR1, PCE);
    }

    bool USART::GetParityControl()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, PCE);
    }

    void USART::SetParitySelection(std::uint8_t x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, PS);
        else
            libs::MWR::setBit(baseAddress + CR1, PS);
    }

    bool USART::GetParitySelection()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, PS);
    }

    void USART::ReceiverWakeup(std::uint8_t x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, RWU);
        else
            libs::MWR::setBit(baseAddress + CR1, RWU);
    }

    bool USART::GetReceiverWakeup()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, RWU);
    }

    void USART::SendBreak(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, SBK);
        else
            libs::MWR::setBit(baseAddress + CR1, SBK);
    }

    bool USART::GetSendBreak()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, SBK);
    }

    void USART::EnableInterrupt(INTERRUPT i)
    {
        libs::MWR::setBit(baseAddress + CR1, i);
    }

    void USART::DisableInterrupt(INTERRUPT i)
    {
        libs::MWR::resetBit(baseAddress + CR1, i);
    }

    bool USART::GetSourceInterrupt(INTERRUPT i)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, i);
    }

    void USART::LinModeEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR2, LINEN);
        else
            libs::MWR::setBit(baseAddress + CR2, LINEN);
    }

    bool USART::GetLinModeEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, LINEN);
    }

    void USART::ClockEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR2, CLKEN);
        else
            libs::MWR::setBit(baseAddress + CR2, CLKEN);
    }

    bool USART::GetClockEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, CLKEN);
    }

    void USART::ClockPolarity(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR2, CPOL);
        else
            libs::MWR::setBit(baseAddress + CR2, CPOL);
    }

    bool USART::GetClockPolarity()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, CPOL);
    }

    void USART::ClockPhase(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR2, CPHA);
        else
            libs::MWR::setBit(baseAddress + CR2, CPHA);
    }

    bool USART::GetClockPhase()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, CPHA);
    }

    void USART::LastBitClockPulse(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR2, LBCL);
        else
            libs::MWR::setBit(baseAddress + CR2, LBCL);
    }

    bool USART::GetLastBitClockPulse()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, LBCL);
    }

    void USART::SetLBDIE(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR2, LBDIE);
        else
            libs::MWR::setBit(baseAddress + CR2, LBDIE);
    }

    bool USART::GetLBDIE()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, LBDIE);
    }

    void USART::LinBreakDetectionLength(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR2, LBDL);
        else
            libs::MWR::setBit(baseAddress + CR2, LBDL);
    }

    bool USART::GetLinBreakDetectionLength()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, LBDL);
    }

    void USART::SetAddressUsartNode(std::uint8_t x)
    {
        libs::MWR::modifyResetRegister(baseAddress + CR2, 0xF << ADD);
        libs::MWR::modifySetRegister(baseAddress + CR2, x << ADD);
    }

    std::uint8_t USART::GetAddressUsartNode()
    {
        return libs::MWR::read_register<std::uint32_t>(baseAddress + CR2) & 0x00000000F;
    }

    void USART::OneSampleBitMethod(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR3, ONEBIT);
        else
            libs::MWR::setBit(baseAddress + CR3, ONEBIT);
    }

    bool USART::GetOneSampleBitMethod()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, ONEBIT);
    }

    void USART::SetCTSIE(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, CTSIE);
        else
            libs::MWR::setBit(baseAddress + CR3, CTSIE);
    }

    bool USART::GetCTSIE()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, CTSIE);
    }

    void USART::CTSEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, CTSE);
        else
            libs::MWR::setBit(baseAddress + CR3, CTSE);
    }

    bool USART::GetCTSEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, CTSE);
    }

    void USART::RTSEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, RTSE);
        else
            libs::MWR::setBit(baseAddress + CR3, RTSE);
    }

    bool USART::GetRTSEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, RTSE);
    }

    void USART::DMAEnableTransmitter(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, DMAT);
        else
            libs::MWR::setBit(baseAddress + CR3, DMAT);
    }

    bool USART::GetDMAEnableTransmitter()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, DMAT);
    }

    void USART::DMAEnableReceiver(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, DMAR);
        else
            libs::MWR::setBit(baseAddress + CR3, DMAR);
    }

    bool USART::GetDMAEnableReceiver()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, DMAR);
    }

    void USART::SmartcardModeEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, SCEN);
        else
            libs::MWR::setBit(baseAddress + CR3, SCEN);
    }

    bool USART::GetSmartcardModeEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, SCEN);
    }

    void USART::SmartcardNACKEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, NACK);
        else
            libs::MWR::setBit(baseAddress + CR3, NACK);
    }

    bool USART::GetSmartcardNACKEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, NACK);
    }

    void USART::HalfDuplexSelection(std::uint8_t x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, HDSEL);
        else
            libs::MWR::setBit(baseAddress + CR3, HDSEL);
    }

    bool USART::GetHalfDuplexSelection()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, HDSEL);
    }

    void USART::IrDALowPower(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR3, IRLP);
        else
            libs::MWR::setBit(baseAddress + CR3, IRLP);
    }

    bool USART::GetIrDALowPower()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, IRLP);
    }

    void USART::IrDAModeEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, IREN);
        else
            libs::MWR::setBit(baseAddress + CR3, IREN);
    }

    bool USART::GetIrDAModeEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, IREN);
    }

    void USART::SetEIE(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, EIE);
        else
            libs::MWR::setBit(baseAddress + CR3, EIE);
    }

    bool USART::GetEIE()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, EIE);
    }

    bool USART::InitUsartDma(const drivers::dma::DMA &usartDma,
                             drivers::dma::DMA_Config dmaConfig,
                             bool                     isTransmit,
                             bool                     isReceive)
    {
        bool rezultInit = false;

        switch(baseAddress)
        {
            case USART1:
                if(usartDma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_2)
                {
                    if(isTransmit == true)
                    {
                        dmaConfig.stream                = dma::constants::Stream_7;
                        dmaConfig.channel               = dma::constants::CHANNEL_4;
                        dmaConfig.peripheralIncrement   = dma::constants::DISABLE;
                        dmaConfig.memoryIncrement       = dma::constants::ENABLE;
                        dmaConfig.dataTransferDirection = dma::constants::MEMORY_TO_PERIPHERAL;
                        dmaConfig.peripheralDataSize    = dma::constants::BYTE;
                        dmaConfig.memoryDataSize        = dma::constants::BYTE;
                        dmaConfig.peripheralIncrementOffsetSize = dma::constants::DISABLE;
                        DMAEnableTransmitter(ENABLE);
                        usartDma.ConfigurationDma(dmaConfig);
                    }
                    if(isReceive == true)
                    {
                        dmaConfig.channel               = dma::constants::CHANNEL_4;
                        dmaConfig.peripheralIncrement   = dma::constants::DISABLE;
                        dmaConfig.memoryIncrement       = dma::constants::ENABLE;
                        dmaConfig.dataTransferDirection = dma::constants::PERIPHERAL_TO_MEMORY;
                        dmaConfig.peripheralDataSize    = dma::constants::BYTE;
                        dmaConfig.memoryDataSize        = dma::constants::BYTE;
                        dmaConfig.peripheralIncrementOffsetSize = dma::constants::DISABLE;
                        DMAEnableReceiver(ENABLE);
                        usartDma.ConfigurationDma(dmaConfig);
                    }
                    rezultInit = true;
                }
                break;

            case USART2:
                if(usartDma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                    if(isTransmit == true)
                    {
                        dmaConfig.stream                = dma::constants::Stream_6;
                        dmaConfig.channel               = dma::constants::CHANNEL_4;
                        dmaConfig.peripheralIncrement   = dma::constants::DISABLE;
                        dmaConfig.memoryIncrement       = dma::constants::ENABLE;
                        dmaConfig.dataTransferDirection = dma::constants::MEMORY_TO_PERIPHERAL;
                        dmaConfig.peripheralDataSize    = dma::constants::BYTE;
                        dmaConfig.memoryDataSize        = dma::constants::BYTE;
                        dmaConfig.peripheralIncrementOffsetSize = dma::constants::DISABLE;
                        DMAEnableTransmitter(ENABLE);
                        usartDma.ConfigurationDma(dmaConfig);
                    }
                    if(isReceive == true)
                    {
                        dmaConfig.stream                = dma::constants::Stream_5;
                        dmaConfig.channel               = dma::constants::CHANNEL_4;
                        dmaConfig.peripheralIncrement   = dma::constants::DISABLE;
                        dmaConfig.memoryIncrement       = dma::constants::ENABLE;
                        dmaConfig.dataTransferDirection = dma::constants::PERIPHERAL_TO_MEMORY;
                        dmaConfig.peripheralDataSize    = dma::constants::BYTE;
                        dmaConfig.memoryDataSize        = dma::constants::BYTE;
                        dmaConfig.peripheralIncrementOffsetSize = dma::constants::DISABLE;
                        DMAEnableReceiver(ENABLE);
                        usartDma.ConfigurationDma(dmaConfig);
                    }
                    rezultInit = true;
                }
                break;

            case USART3:
                if(usartDma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                }
                break;
        }
        return rezultInit;
    }

    bool USART::TransmitDataDma(dma::DMA &dma, std::uint8_t *sendBuffer, std::uint16_t size)
    {
        bool rezultTransmit = false;
        switch(baseAddress)
        {
            case USART1:
                break;
            case USART2:
                if(dma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                    dma.disable(dma::constants::Stream_6);
                    dma.setPeripheralAddressRegister(dma::constants::Stream_6, USART2 + DR);
                    dma.setMemoryAddressRegister_0(dma::constants::Stream_6,
                                                   reinterpret_cast<std::uint32_t>(sendBuffer));
                    dma.setNumberDataRegister(dma::constants::Stream_6, size);
                    rezultTransmit = true;
                    dma.enable(dma::constants::Stream_6);
                }
                break;
        }
        return rezultTransmit;
    }

    bool USART::ReceiveDataDma(dma::DMA &dma, std::uint8_t *recvBuffer, std::uint16_t size)
    {
        bool rezultReceive = false;

        switch(baseAddress)
        {
            case USART1:
                break;
            case USART2:
                if(dma.getBaseAddress() == dma::ADDRESSES_DMA::DMA_1)
                {
                    dma.disable(dma::constants::Stream_5);
                    dma.setPeripheralAddressRegister(dma::constants::Stream_5, USART2 + DR);
                    dma.setMemoryAddressRegister_0(dma::constants::Stream_5,
                                                   reinterpret_cast<std::uint32_t>(recvBuffer));
                    dma.setNumberDataRegister(dma::constants::Stream_5, size);
                    rezultReceive = true;
                    dma.enable(dma::constants::Stream_5);
                }
                break;
        }

        return rezultReceive;
    }

}    // namespace drivers::usart