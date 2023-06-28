//
// Created by koteiko_vv on 28.03.2023.
//

#include "dma.hpp"
#include "cout.hpp"

extern libs::Cout *globalCout;

namespace drivers::dma
{
    bool DMA::getFlagInterruptStatus(const constants::NUMBER_STREAM& numberStream,const std::uint8_t& offset, bool getFlag) const noexcept{
        bool temp = false;

        if(numberStream == constants::Stream_0 || numberStream == constants::Stream_1 ||
            numberStream == constants::Stream_2 || numberStream == constants::Stream_3)
        {
            if (getFlag)
            {
              temp = libs::MWR::readBit<std::uint32_t>(getAddress(LISR), getNumberStream(numberStream) + offset);

            } else
            {
                libs::MWR::setBit(getAddress(LIFCR), getNumberStream(numberStream) + offset);
            }
        } else
        {
            if (getFlag)
            {
                temp = libs::MWR::readBit<std::uint32_t>(getAddress(HISR), getNumberStream(numberStream) + offset);

            } else
            {
                libs::MWR::setBit(getAddress(HIFCR), (getNumberStream(numberStream) + offset));
            }
        }

        return temp;
    }

    void DMA::enable(const drivers::dma::constants::NUMBER_STREAM &numberStream) const noexcept {
        setStreamConfigurationRegister(numberStream,constants::STREAM_ENABLE,1);
    }

    void DMA::disable(const drivers::dma::constants::NUMBER_STREAM &numberStream) const noexcept {
        setStreamConfigurationRegister(numberStream,constants::STREAM_ENABLE,0);
    }

    bool DMA::getFlagInterrupt(const drivers::dma::constants::NUMBER_STREAM& numberStream, const drivers::dma::constants::FLAG_INTERRUPT_STATUS& flag) const noexcept {
       return getFlagInterruptStatus(numberStream,flag, true);
    }

    void DMA::clearFlagInterrupt(const drivers::dma::constants::NUMBER_STREAM& numberStream, const drivers::dma::constants::FLAG_INTERRUPT_STATUS& flag) const noexcept {
        getFlagInterruptStatus(numberStream, flag, false);
    }

    void DMA::setStreamConfigurationRegister(const drivers::dma::constants::NUMBER_STREAM &numberStream,
                                             const drivers::dma::constants::FLAG_STREAM_CONFIGURATION &flagStreamConfiguration, std::uint8_t value) const noexcept {
        readWriteRegister::modifySetRegister(getAddress(numberStream, CR), value << flagStreamConfiguration);
    }

    void DMA::setNumberDataRegister(const drivers::dma::constants::NUMBER_STREAM &numberStream, std::uint16_t sizeData) const noexcept {
        readWriteRegister::write_register(getAddress(numberStream,NDTR), sizeData);
    }

    void DMA::setPeripheralAddressRegister(const drivers::dma::constants::NUMBER_STREAM &numberStream, std::uint32_t address) const noexcept{
        readWriteRegister::write_register(getAddress(numberStream,PAR), address);
    }

    void DMA::setMemoryAddressRegister_0(const drivers::dma::constants::NUMBER_STREAM &numberStream, std::uint32_t address) const noexcept {
        readWriteRegister::write_register(getAddress(numberStream,M0AR), address);
    }

    void DMA::setMemoryAddressRegister_1(const drivers::dma::constants::NUMBER_STREAM &numberStream , std::uint32_t address) const noexcept {
        readWriteRegister::write_register(getAddress(numberStream, M1AR), address);
    }

    void DMA::setFlagFIFOcontrol(const drivers::dma::constants::NUMBER_STREAM &numberStream ,
                                 const drivers::dma::constants::FLAG_FIFO_CONTROL_REGISTER &flagFifoControlRegister, std::uint8_t value) const noexcept {
        readWriteRegister::modifySetRegister(getAddress(numberStream, FCR),(value << flagFifoControlRegister));
    }

    std::uint8_t DMA::getFlagFIFOcontrol(const drivers::dma::constants::NUMBER_STREAM &numberStream,
                                         const drivers::dma::constants::FLAG_FIFO_CONTROL_REGISTER &flagFifoControlRegister) const noexcept {
        std::uint32_t temp = libs::MWR::read_register<std::uint32_t>(getAddress(numberStream,FCR));
        return getFlagFIFOcontrolValue(flagFifoControlRegister,temp);
    }

    std::uintptr_t DMA::getBaseAddress() const noexcept {
        return baseAddress;
    }

    void DMA::ConfigurationDma(DMA_Config dmaConfig)  const noexcept{
        disable(dmaConfig.stream);

        setStreamConfigurationRegister(dmaConfig.stream, constants::DIRECT_MODE_ERROR_INTERRUPT_ENABLE, dmaConfig.directModeErrorInterrupt);
        setStreamConfigurationRegister(dmaConfig.stream, constants::TRANSFER_ERROR_INTERRUPT_ENABLE, dmaConfig.transferErrorInterrupt);
        setStreamConfigurationRegister(dmaConfig.stream, constants::HALF_TRANSFER_INTERRUPT_ENABLE, dmaConfig.halfTransferInterrupt);
        setStreamConfigurationRegister(dmaConfig.stream, constants::TRANSFER_COMPLETE_INTERRUPT_ENABLE, dmaConfig.transferCompleteInterrupt);
        setStreamConfigurationRegister(dmaConfig.stream, constants::PERIPHERAL_FLOW_CONTROLLER, dmaConfig.peripheralFlowController);
        setStreamConfigurationRegister(dmaConfig.stream, constants::DATA_TRANSFER_DIRECTION, dmaConfig.dataTransferDirection);
        setStreamConfigurationRegister(dmaConfig.stream, constants::CIRCULAR_MODE, dmaConfig.circularMode);
        setStreamConfigurationRegister(dmaConfig.stream, constants::PERIPHERAL_INCREMENT_MODE, dmaConfig.peripheralIncrement);
        setStreamConfigurationRegister(dmaConfig.stream, constants::MEMORY_INCREMENT_MODE, dmaConfig.memoryIncrement);
        setStreamConfigurationRegister(dmaConfig.stream, constants::PERIPHERAL_DATA_SIZE, dmaConfig.peripheralDataSize);
        setStreamConfigurationRegister(dmaConfig.stream, constants::MEMORY_DATA_SIZE, dmaConfig.memoryDataSize);
        setStreamConfigurationRegister(dmaConfig.stream, constants::PERIPHERAL_INCREMENT_OFFSET_SIZE, dmaConfig.peripheralIncrementOffsetSize);
        setStreamConfigurationRegister(dmaConfig.stream, constants::PRIORITY_LEVEL, dmaConfig.priorityLevel);
        setStreamConfigurationRegister(dmaConfig.stream, constants::DOUBLE_BUFFER_MODE, dmaConfig.doubleBufferMode);
        setStreamConfigurationRegister(dmaConfig.stream, constants::CURRENT_TARGET, dmaConfig.currentTarget);
        setStreamConfigurationRegister(dmaConfig.stream, constants::PERIPHERAL_BURST_TRANSFER_CONFIGURATION, dmaConfig.peripheralBurstTransferConfiguration);
        setStreamConfigurationRegister(dmaConfig.stream, constants::MEMORY_BURST_TRANSFER_CONFIGURATION, dmaConfig.memoryBurstTransferConfiguration);
        setStreamConfigurationRegister(dmaConfig.stream, constants::CHANNEL, dmaConfig.channel);

        setFlagFIFOcontrol(dmaConfig.stream, constants::FIFO_THRESHOLD_SELECTION, dmaConfig.FIFOThresholdSelection);
        setFlagFIFOcontrol(dmaConfig.stream, constants::DIRECT_MODE_DISABLE, dmaConfig.directModeDisableInvers);
        setFlagFIFOcontrol(dmaConfig.stream, constants::FIFO_ERROR_INTERRUPT_ENABLE, dmaConfig.fifoErrorInterrupt);
    }
}




