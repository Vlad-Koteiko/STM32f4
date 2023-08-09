//
// Created by koteiko_vv on 23.01.2023.
//
#include "usart.hpp"

namespace drivers::usart
{
    void Usart::init() noexcept
    {
        switch(baseAddress)
        {
            case USART1:
            {
                clockControl.EnablePeripherals(drivers::clock::constants::USART1_MODULE);
                RemapUsart1(U1_TX_PA9_RX_PA10);
                SetBaudRate(baundRateAPB2);
                break;
            }
            case USART2:
            {
                clockControl.EnablePeripherals(drivers::clock::constants::USART2_MODULE);
                RemapUsart2(U2_TX_PA2_RX_PA3);
                SetBaudRate(baundRateAPB1);
                break;
            }
            case USART3:
            {
                clockControl.EnablePeripherals(drivers::clock::constants::USART3_MODULE);
                RemapUsart3(U3_TX_PB10_RX_PB11);
                SetBaudRate(baundRateAPB1);
                break;
            }
            case UART4:
            {
                clockControl.EnablePeripherals(drivers::clock::constants::UART4_MODULE);
                RemapUart4(U4_TX_PA0_RX_PA1);
                SetBaudRate(baundRateAPB1);
                break;
            }

            case UART5:
            {
                clockControl.EnablePeripherals(drivers::clock::constants::UART5_MODULE);
                ConfigGpioForUart(drivers::port::PORTC,
                                  drivers::port::PORTD,
                                  drivers::port::PIN_12,
                                  drivers::port::PIN_2,
                                  drivers::port::AF8);
                SetBaudRate(baundRateAPB1);
                break;
            }

            case USART6:
            {
                clockControl.EnablePeripherals(drivers::clock::constants::USART6_MODULE);
                RemapUsart6(U6_TX_PC6_RX_PC7);
                SetBaudRate(baundRateAPB2);
                break;
            }
        }
        SetWordLength(BIT_8);
        ReceiverEnable(ENABLE);
        TransmitterEnable(ENABLE);
        UsartEnable(ENABLE);
    }

    void Usart::RemapUsart1(const USART1_Remap remap)
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

    void Usart::RemapUsart2(const USART2_Remap remap)
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

    void Usart::RemapUsart3(const USART3_Remap remap)
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

    void Usart::RemapUart4(const UART4_Remap remap)
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

    void Usart::RemapUsart6(const USART6_Remap remap)
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

    void Usart::Remap(std::uint8_t remapPins)
    {
        switch(baseAddress)
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

    void Usart::ConfigGpioForUart(const drivers::port::ADDRESSES_PORT     portTX,
                                  const drivers::port::ADDRESSES_PORT     portRX,
                                  const drivers::port::PIN_NUMBER         pinTX,
                                  const drivers::port::PIN_NUMBER         pinRX,
                                  const drivers::port::ALTERNATE_FUNCTION af) noexcept
    {
        // init GPIO TX
        drivers::port::Gpio port_tx(clockControl, portTX);
        port_tx.SetPinMode(pinTX, drivers::port::ALTERNATE_FUNCT);
        port_tx.SetPinSpeed(pinTX, drivers::port::VERY_HIGH_SPEED);
        port_tx.SetAFPin(pinTX, af);

        // init GPIO RX
        drivers::port::Gpio port_rx(clockControl, portRX);
        port_rx.SetPinMode(pinRX, drivers::port::ALTERNATE_FUNCT);
        port_rx.SetPinSpeed(pinRX, drivers::port::VERY_HIGH_SPEED);
        port_rx.SetAFPin(pinRX, af);
    }

    void Usart::SetWordLength(
        WORD_LENGTH wordLength) noexcept    // This bit determines the word length. It is set or
                                            // cleared by software.
    {
        if(wordLength == BIT_8)
            libs::MWR::resetBit(baseAddress + CR1, M);
        else
            libs::MWR::setBit(baseAddress + CR1, M);
    }

    void Usart::ReceiverEnable(STATUS receiver) noexcept    // This bit enables the receiver. It is
                                                            // set and cleared by software
    {
        if(receiver == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, RE);
        else
            libs::MWR::setBit(baseAddress + CR1, RE);
    }

    void Usart::TransmitterEnable(
        STATUS transmitter) noexcept    // This bit enables the transmitter. It is set and cleared
                                        // by software
    {
        if(transmitter == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, TE);
        else
            libs::MWR::setBit(baseAddress + CR1, TE);
    }

    void Usart::UsartEnable(
        STATUS
            usartEnable) noexcept    // USART prescalers and outputs are stopped and the end of the
                                     // current byte transfer in order to reduce power consumption
    {
        if(usartEnable == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, UE);
        else
            libs::MWR::setBit(baseAddress + CR1, UE);
    }

    void Usart::SetStopBitsLength(
        STOP_BIT stopBit) noexcept    // These bits are used for programming the stop bits.
    {
        libs::MWR::modifyResetRegister(baseAddress + CR2, 3 << STOP);
        libs::MWR::modifySetRegister(baseAddress + CR2, stopBit << STOP);
    }

    void Usart::SetBaudRate(const drivers::usart::Usart::BaundRateType &baundRate) noexcept
    {
        libs::MWR::write_register(
            baseAddress + BRR,
            (std::get<0>(baundRate) << DIV_Mantissa) | (std::get<1>(baundRate) & 0x000F));
    }

    void Usart::sendByte(std::byte byte) noexcept
    {
        libs::MWR::write_register(baseAddress + DR, byte);
    }

    void Usart::TransmitString(const void *value, std::size_t size) noexcept
    {
        std::size_t temp = 0;
        while(size--)
        {
            while(!ReadFlag(TXE))
            {}
            sendByte(static_cast<std::byte>(*(static_cast<const char *>(value) + temp++)));
        }
    }

    std::byte Usart::readByte() noexcept
    {
        return libs::MWR::read_register<std::byte>(baseAddress + DR);
    }

    void Usart::DeInit() noexcept
    {
        switch(baseAddress)
        {
            case USART1:
                clockControl.DisablePeripherals(drivers::clock::constants::USART1_MODULE);
                break;
            case USART2:
                clockControl.DisablePeripherals(drivers::clock::constants::USART2_MODULE);
                break;
            case USART3:
                clockControl.DisablePeripherals(drivers::clock::constants::USART3_MODULE);
                break;
            case UART4:
                clockControl.DisablePeripherals(drivers::clock::constants::UART4_MODULE);
                break;
            case UART5:
                clockControl.DisablePeripherals(drivers::clock::constants::UART5_MODULE);
                break;
            case USART6:
                clockControl.DisablePeripherals(drivers::clock::constants::USART6_MODULE);
                break;
            case UART7:
                clockControl.DisablePeripherals(drivers::clock::constants::UART7_MODULE);
                break;
            case UART8:
                clockControl.DisablePeripherals(drivers::clock::constants::UART8_MODULE);
                break;
        }
    }

    //---------------------------------------------------------------------------------
    bool Usart::ReadFlag(SR_poz flag)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, flag);
    }

    void Usart::ClearFlag(SR_clear_flag flag)
    {
        libs::MWR::resetBit(baseAddress + SR, CTS);
    }
    //---------------------------------------------------------------------------------

    void Usart::SetOversamplingMode(std::uint8_t mode)
    {
        if(mode == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, OVER8);
        else
            libs::MWR::setBit(baseAddress + CR1, OVER8);
    }

    bool Usart::GetOversamplingMode()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, OVER8);
    }

    void Usart::SetWakeUpMethod(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR1, WAKE);
        else
            libs::MWR::setBit(baseAddress + CR1, WAKE);
    }

    bool Usart::GetWakeUpMethod()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, WAKE);
    }

    void Usart::SetParityControl(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, PCE);
        else
            libs::MWR::setBit(baseAddress + CR1, PCE);
    }

    bool Usart::GetParityControl()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, PCE);
    }

    void Usart::SetParitySelection(std::uint8_t x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, PS);
        else
            libs::MWR::setBit(baseAddress + CR1, PS);
    }

    bool Usart::GetParitySelection()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, PS);
    }

    void Usart::ReceiverWakeup(std::uint8_t x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, RWU);
        else
            libs::MWR::setBit(baseAddress + CR1, RWU);
    }

    bool Usart::GetReceiverWakeup()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, RWU);
    }

    void Usart::SendBreak(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR1, SBK);
        else
            libs::MWR::setBit(baseAddress + CR1, SBK);
    }

    bool Usart::GetSendBreak()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, SBK);
    }

    void Usart::EnableInterrupt(INTERRUPT i)
    {
        libs::MWR::setBit(baseAddress + CR1, i);
    }

    void Usart::DisableInterrupt(INTERRUPT i)
    {
        libs::MWR::resetBit(baseAddress + CR1, i);
    }

    bool Usart::GetSourceInterrupt(INTERRUPT i)
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, i);
    }

    void Usart::LinModeEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR2, LINEN);
        else
            libs::MWR::setBit(baseAddress + CR2, LINEN);
    }

    bool Usart::GetLinModeEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, LINEN);
    }

    void Usart::ClockEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR2, CLKEN);
        else
            libs::MWR::setBit(baseAddress + CR2, CLKEN);
    }

    bool Usart::GetClockEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, CLKEN);
    }

    void Usart::ClockPolarity(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR2, CPOL);
        else
            libs::MWR::setBit(baseAddress + CR2, CPOL);
    }

    bool Usart::GetClockPolarity()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, CPOL);
    }

    void Usart::ClockPhase(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR2, CPHA);
        else
            libs::MWR::setBit(baseAddress + CR2, CPHA);
    }

    bool Usart::GetClockPhase()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, CPHA);
    }

    void Usart::LastBitClockPulse(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR2, LBCL);
        else
            libs::MWR::setBit(baseAddress + CR2, LBCL);
    }

    bool Usart::GetLastBitClockPulse()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, LBCL);
    }

    void Usart::SetLBDIE(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR2, LBDIE);
        else
            libs::MWR::setBit(baseAddress + CR2, LBDIE);
    }

    bool Usart::GetLBDIE()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, LBDIE);
    }

    void Usart::LinBreakDetectionLength(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR2, LBDL);
        else
            libs::MWR::setBit(baseAddress + CR2, LBDL);
    }

    bool Usart::GetLinBreakDetectionLength()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, LBDL);
    }

    void Usart::SetAddressUsartNode(std::uint8_t x)
    {
        libs::MWR::modifyResetRegister(baseAddress + CR2, 0xF << ADD);
        libs::MWR::modifySetRegister(baseAddress + CR2, x << ADD);
    }

    std::uint8_t Usart::GetAddressUsartNode()
    {
        return libs::MWR::read_register<std::uint32_t>(baseAddress + CR2) & 0x00000000F;
    }

    void Usart::OneSampleBitMethod(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR3, ONEBIT);
        else
            libs::MWR::setBit(baseAddress + CR3, ONEBIT);
    }

    bool Usart::GetOneSampleBitMethod()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, ONEBIT);
    }

    void Usart::SetCTSIE(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, CTSIE);
        else
            libs::MWR::setBit(baseAddress + CR3, CTSIE);
    }

    bool Usart::GetCTSIE()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, CTSIE);
    }

    void Usart::CTSEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, CTSE);
        else
            libs::MWR::setBit(baseAddress + CR3, CTSE);
    }

    bool Usart::GetCTSEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, CTSE);
    }

    void Usart::RTSEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, RTSE);
        else
            libs::MWR::setBit(baseAddress + CR3, RTSE);
    }

    bool Usart::GetRTSEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, RTSE);
    }

    void Usart::DMAEnableTransmitter(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, DMAT);
        else
            libs::MWR::setBit(baseAddress + CR3, DMAT);
    }

    bool Usart::GetDMAEnableTransmitter()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, DMAT);
    }

    void Usart::DMAEnableReceiver(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, DMAR);
        else
            libs::MWR::setBit(baseAddress + CR3, DMAR);
    }

    bool Usart::GetDMAEnableReceiver()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, DMAR);
    }

    void Usart::SmartcardModeEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, SCEN);
        else
            libs::MWR::setBit(baseAddress + CR3, SCEN);
    }

    bool Usart::GetSmartcardModeEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, SCEN);
    }

    void Usart::SmartcardNACKEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, NACK);
        else
            libs::MWR::setBit(baseAddress + CR3, NACK);
    }

    bool Usart::GetSmartcardNACKEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, NACK);
    }

    void Usart::HalfDuplexSelection(std::uint8_t x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, HDSEL);
        else
            libs::MWR::setBit(baseAddress + CR3, HDSEL);
    }

    bool Usart::GetHalfDuplexSelection()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, HDSEL);
    }

    void Usart::IrDALowPower(std::uint8_t x)
    {
        if(x == 0)
            libs::MWR::resetBit(baseAddress + CR3, IRLP);
        else
            libs::MWR::setBit(baseAddress + CR3, IRLP);
    }

    bool Usart::GetIrDALowPower()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, IRLP);
    }

    void Usart::IrDAModeEnable(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, IREN);
        else
            libs::MWR::setBit(baseAddress + CR3, IREN);
    }

    bool Usart::GetIrDAModeEnable()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, IREN);
    }

    void Usart::SetEIE(STATUS x)
    {
        if(x == DISABLE)
            libs::MWR::resetBit(baseAddress + CR3, EIE);
        else
            libs::MWR::setBit(baseAddress + CR3, EIE);
    }

    bool Usart::GetEIE()
    {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR3, EIE);
    }

    bool Usart::InitUsartDma(const drivers::dma::DMA &usartDma,
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
                {}
                break;
        }
        return rezultInit;
    }

    bool Usart::TransmitDataDma(dma::DMA &dma, std::uint8_t *sendBuffer, std::uint16_t size)
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

    bool Usart::ReceiveDataDma(dma::DMA &dma, std::uint8_t *recvBuffer, std::uint16_t size)
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