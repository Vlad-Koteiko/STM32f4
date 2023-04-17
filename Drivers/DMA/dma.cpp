//
// Created by koteiko_vv on 28.03.2023.
//

#include "dma.h"

namespace drivers::DMA
{
    DMA::DMA(const drivers::clock::ClockControl &clockControl, const drivers::DMA::ADDRESSES_DMA addressesDma)
                                                        : clockControl(clockControl), baseAddress(addressesDma) {

        baseAddress == DMA_1 ?  clockControl.EnablePeripherals(drivers::clock::DMA_1_MODULE) :  clockControl.EnablePeripherals(drivers::clock::DMA_2_MODULE);
    }

    constexpr std::uint8_t DMA::getNumberStream(const NUMBER_STREAM& numberStream) const noexcept {
        switch (numberStream) {
            case Stream_0: return 0;
                break;
            case Stream_1: return 6;
                break;
            case Stream_2: return 16;
                break;
            case Stream_3: return 22;
                break;
            case Stream_4: return 0;
                break;
            case Stream_5: return 6;
                break;
            case Stream_6: return 16;
                break;
            case Stream_7: return 22;
                break;
            default:       return 99;
                break;
        }
    }

    constexpr std::uint32_t DMA::getAddress(const drivers::DMA::NUMBER_STREAM &numberStream, std::ptrdiff_t offset) const noexcept {
        return static_cast<std::uint32_t>(baseAddress + numberStream + offset);
    }

    constexpr std::uint32_t DMA::getAddress(std::ptrdiff_t offset) const noexcept {
        return static_cast<std::uint32_t>(baseAddress + offset);
    }

    constexpr std::uint8_t DMA::getFlagFIFOcontrolValue(const drivers::DMA::FLAG_FIFO_CONTROL_REGISTER &flagFifoControlRegister, std::uint32_t value) const noexcept {
        return ((value >> flagFifoControlRegister) & flagFifoControlRegister);
    }

    bool DMA::getFlagInterruptStatus(const NUMBER_STREAM& numberStream,const std::uint8_t& offset, bool getFlag) const noexcept{
        bool temp = false;

        if(numberStream == Stream_0 || numberStream == Stream_1 ||
            numberStream == Stream_2 || numberStream == Stream_3)
        {
            if (getFlag)
            {
              temp = libs::MWR::readBit<std::uint32_t>(getAddress(LISR), getNumberStream(numberStream) + offset);

            } else
            {
                libs::MWR::resetBit(getAddress(LISR), getNumberStream(numberStream) + offset);
            }
        } else
        {
            if (getFlag)
            {
                temp = libs::MWR::readBit<std::uint32_t>(getAddress(HISR), getNumberStream(numberStream) + offset);

            } else
            {
                libs::MWR::resetBit(getAddress(HISR), getNumberStream(numberStream) + offset);
            }
        }

        return temp;
    }

    void DMA::enable(const drivers::DMA::NUMBER_STREAM &numberStream) const noexcept {
        setStreamConfigurationRegister(numberStream,STREAM_ENABLE,1);
    }

    void DMA::deseable(const drivers::DMA::NUMBER_STREAM &numberStream) const noexcept {
        setStreamConfigurationRegister(numberStream,STREAM_ENABLE,0);
    }

    bool DMA::getFlagInterrupt(const drivers::DMA::NUMBER_STREAM& numberStream, const drivers::DMA::FLAG_INTERRUPT_STATUS& flag) const noexcept {
       return getFlagInterruptStatus(numberStream,flag, true);
    }

    void DMA::clearFlagInterrupt(const drivers::DMA::NUMBER_STREAM& numberStream, const drivers::DMA::FLAG_INTERRUPT_STATUS& flag) const noexcept {
        getFlagInterruptStatus(numberStream, flag, false);
    }

    void DMA::setStreamConfigurationRegister(const drivers::DMA::NUMBER_STREAM &numberStream,
                                             const drivers::DMA::FLAG_STREAM_CONFIGURATION &flagStreamConfiguration, std::uint8_t value) const noexcept {
        readWriteRegister::modifySetRegister(getAddress(numberStream, CR), value << flagStreamConfiguration);
    }

    void DMA::setNumberDataRegister(const drivers::DMA::NUMBER_STREAM &numberStream, std::uint16_t sizeData) const noexcept {
        readWriteRegister::write_register(getAddress(numberStream,NDTR), sizeData);
    }

    void DMA::setPeripheralAddressRegister(const drivers::DMA::NUMBER_STREAM &numberStream, std::uint32_t address) const noexcept{
        readWriteRegister::write_register(getAddress(numberStream,PAR), address);
    }

    void DMA::setMemoryAddressRegister_0(const drivers::DMA::NUMBER_STREAM &numberStream, std::uint32_t address) const noexcept {
        readWriteRegister::write_register(getAddress(numberStream,M0AR), address);
    }

    void DMA::setMemoryAddressRegister_1(const drivers::DMA::NUMBER_STREAM &numberStream , std::uint32_t address) const noexcept {
        readWriteRegister::write_register(getAddress(numberStream, M1AR), address);
    }

    void DMA::setFlagFIFOcontrol(const drivers::DMA::NUMBER_STREAM &numberStream ,
                                 const drivers::DMA::FLAG_FIFO_CONTROL_REGISTER &flagFifoControlRegister, std::uint8_t value) const noexcept {
        readWriteRegister::modifySetRegister(getAddress(numberStream, FCR),(value << flagFifoControlRegister));
    }

    std::uint8_t DMA::getFlagFIFOcontrol(const drivers::DMA::NUMBER_STREAM &numberStream,
                                         const drivers::DMA::FLAG_FIFO_CONTROL_REGISTER &flagFifoControlRegister) const noexcept {
        std::uint32_t temp = libs::MWR::read_register<std::uint32_t>(getAddress(numberStream,FCR));
        return getFlagFIFOcontrolValue(flagFifoControlRegister,temp);
    }
}




