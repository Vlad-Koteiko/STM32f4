//
// Created by koteiko_vv on 28.03.2023.
//
#ifndef STM32F4_DMA_HPP
#define STM32F4_DMA_HPP

#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::dma
{
    enum ADDRESSES_DMA : std::uintptr_t
    {
        DMA_1 = 0x40026000,
        DMA_2 = 0x40026400,
    };

    namespace constants
    {
        enum NUMBER_STREAM : std::ptrdiff_t
        {
            Stream_0 = 0x10,
            Stream_1 = 0x20,
            Stream_2 = 0x40,
            Stream_3 = 0x58,
            Stream_4 = 0x70,
            Stream_5 = 0x88,
            Stream_6 = 0xA0,
            Stream_7 = 0xB8,
        };

        enum FLAG_INTERRUPT_STATUS : std::uint8_t
        {
            FIFO_ERROR_INTERRUPT        = 0,
            DIRECT_MODE_ERROR_INTERRUPT = 2,
            TRANSFER_ERROR_INTERRUPT    = 3,
            HALF_TRANSFER_INTERRUPT     = 4,
            TRANSFER_COMPLETE_INTERRUPT = 5
        };

        enum FLAG_STREAM_CONFIGURATION: std::uint8_t
        {
            STREAM_ENABLE                           = 0,
            DIRECT_MODE_ERROR_INTERRUPT_ENABLE      = 1,
            TRANSFER_ERROR_INTERRUPT_ENABLE         = 2,
            HALF_TRANSFER_INTERRUPT_ENABLE          = 3,
            TRANSFER_COMPLETE_INTERRUPT_ENABLE      = 4,
            PERIPHERAL_FLOW_CONTROLLER              = 5,
            DATA_TRANSFER_DIRECTION                 = 6,
            CIRCULAR_MODE                           = 8,
            PERIPHERAL_INCREMENT_MODE               = 9,
            MEMORY_INCREMENT_MODE                   = 10,
            PERIPHERAL_DATA_SIZE                    = 11,
            MEMORY_DATA_SIZE                        = 13,
            PERIPHERAL_INCREMENT_OFFSET_SIZE        = 15,
            PRIORITY_LEVEL                          = 16,
            DOUBLE_BUFFER_MODE                      = 18,
            CURRENT_TARGET                          = 19,
            PERIPHERAL_BURST_TRANSFER_CONFIGURATION = 21,
            MEMORY_BURST_TRANSFER_CONFIGURATION     = 23,
            CHANNEL                                 = 25
        };

        enum FLAG_FIFO_CONTROL_REGISTER: std::uint8_t
        {
            FIFO_THRESHOLD_SELECTION    = 0,
            DIRECT_MODE_DISABLE         = 2,
            FIFO_STATUS                 = 3,
            FIFO_ERROR_INTERRUPT_ENABLE = 7
        };

        enum DATA_DIRECTION : std::uint8_t
        {
            PERIPHERAL_TO_MEMORY = 0,
            MEMORY_TO_PERIPHERAL = 1,
            MEMORY_TO_MEMORY     = 2,
        };

        enum DATA_SIZE : std::uint8_t
        {
            BYTE      = 0,
            HALF_WORD = 1,
            WORD      = 2
        };

        enum PRIORITY : std::uint8_t
        {
            LOW       = 0,
            MEDIUM    = 1,
            HIGH      = 2,
            VERY_HIGH = 3
        };

        enum PERIPHERAL_BURST : std::uint8_t
        {
            SINGLE_TRANSFER = 0,
            INCR4           = 1,
            INCR8           = 2,
            INCR16          = 3
        };

        enum CHANNEL_SELECTION : std::uint8_t
        {
            CHANNEL_0 = 0,
            CHANNEL_1 = 1,
            CHANNEL_2 = 2,
            CHANNEL_3 = 3,
            CHANNEL_4 = 4,
            CHANNEL_5 = 5,
            CHANNEL_6 = 6,
            CHANNEL_7 = 7
        };

        enum STATUS : std::uint8_t
        {
            DISABLED = 0,
            ENABLED = 1
        };

        enum  TARGET : std::uint8_t
        {
            MEMORY0,
            MEMORY1
        };

        enum FIFO_THRESHOLD : std::uint8_t
        {
            FIFO_0_25 = 0,
            FIFO_0_50 = 1,
            FIFO_0_75 = 2,
            FIFO_FULL = 3
        };
    };

    struct DMA_Config
    {
        constants::STATUS directModeErrorInterrupt;
        constants::STATUS transferErrorInterrupt;
        constants::STATUS halfTransferInterrupt;
        constants::STATUS peripheralFlowController;
        constants::DATA_DIRECTION dataTransferDirection;
        constants::STATUS circularMode;
        constants::STATUS peripheralIncrement;
        constants::STATUS memoryIncrement;
        constants::DATA_SIZE peripheralDataSize;
        constants::DATA_SIZE memoryDataSize;
        constants::STATUS peripheralIncrementOffsetSize;
        constants::PRIORITY priorityLevel;
        constants::STATUS doubleBufferMode;
        constants::TARGET currentTarget;
        constants::PERIPHERAL_BURST peripheralBurstTransferConfiguration;
        constants::PERIPHERAL_BURST memoryBurstTransferConfiguration;
        constants::CHANNEL_SELECTION channel;

        constants::FIFO_THRESHOLD  FIFOThresholdSelection;
        constants::STATUS directModeDisableInvers;
        constants::STATUS fifoErrorInterrupt;

        constants::NUMBER_STREAM stream;
    };

    class DMA {

        using readWriteRegister = libs::MWR;
        const drivers::clock::ClockControl &clockControl;
        const std::uintptr_t baseAddress;

        enum RegisterDMA : std::ptrdiff_t {
            LISR    = 0,    /*!< DMA low interrupt status register,       Address offset: 0x00 */
            HISR    = 0x04, /*!< DMA high interrupt status register,      Address offset: 0x04 */
            LIFCR   = 0x08, /*!< DMA low interrupt flag clear register,   Address offset: 0x08 */
            HIFCR   = 0x0C, /*!< DMA high interrupt flag clear register,  Address offset: 0x0C */
        };

        enum RegisterSTREAM : std::ptrdiff_t {
            CR   = 0,    /*!< DMA stream 0 configuration register            Address offset: 0x0 */
            NDTR = 0x04, /*!< DMA stream 0 number of data register        Address offset: 0x04 */
            PAR  = 0x08, /*!< DMA stream 0 peripheral address register    Address offset: 0x08 */
            M0AR = 0x0C, /*!< DMA stream 0 memory 0 address register      Address offset: 0x0C */
            M1AR = 0x10, /*!< DMA stream 0 memory 1 address register      Address offset: 0x10 */
            FCR  = 0x14, /*!< DMA stream 0 FIFO control register          Address offset: 0x14 */
        };

        constexpr std::uint32_t getAddress(const drivers::dma::constants::NUMBER_STREAM &numberStream, std::ptrdiff_t offset) const noexcept {
            return static_cast<std::uint32_t>(baseAddress + numberStream + offset);
        }

        constexpr std::uintptr_t getAddress(std::ptrdiff_t offset) const noexcept {
            return (baseAddress + offset);
        }

        constexpr std::uint8_t getFlagFIFOcontrolValue(const drivers::dma::constants::FLAG_FIFO_CONTROL_REGISTER &flagFifoControlRegister, std::uint32_t value) const noexcept {
            return ((value >> flagFifoControlRegister) & flagFifoControlRegister);
        }

        constexpr std::uint8_t getNumberStream(const constants::NUMBER_STREAM& numberStream) const noexcept {
            switch (numberStream) {
                case constants::Stream_0: return 0;
                    break;
                case constants::Stream_1: return 6;
                    break;
                case constants::Stream_2: return 16;
                    break;
                case constants::Stream_3: return 22;
                    break;
                case constants::Stream_4: return 0;
                    break;
                case constants::Stream_5: return 6;
                    break;
                case constants::Stream_6: return 16;
                    break;
                case constants::Stream_7: return 22;
                    break;
                default:       return 99;
                    break;
            }
        }

        bool getFlagInterruptStatus(const constants::NUMBER_STREAM&,const std::uint8_t&, bool) const noexcept;

    public:

        constexpr DMA(const drivers::clock::ClockControl &clockControl, const drivers::dma::ADDRESSES_DMA addressesDma)
                : clockControl(clockControl), baseAddress(addressesDma) {

            baseAddress == DMA_1 ?  clockControl.EnablePeripherals(drivers::clock::constants::DMA1_MODULE) :  clockControl.EnablePeripherals(drivers::clock::constants::DMA2_MODULE);
        }

        /* Interuuupt status register */
        [[nodiscard]] bool getFlagInterrupt(const constants::NUMBER_STREAM&, const constants::FLAG_INTERRUPT_STATUS&) const noexcept;
        void clearFlagInterrupt(const constants::NUMBER_STREAM&, const constants::FLAG_INTERRUPT_STATUS&) const noexcept;

        /* DMA stream x configuration register */
        void setStreamConfigurationRegister(const constants::NUMBER_STREAM&, const constants::FLAG_STREAM_CONFIGURATION&, std::uint8_t) const noexcept;
        void setNumberDataRegister(const constants::NUMBER_STREAM&, std::uint16_t) const noexcept;
        void setPeripheralAddressRegister(const constants::NUMBER_STREAM&, std::uint32_t) const noexcept;
        void setMemoryAddressRegister_0(const constants::NUMBER_STREAM&, std::uint32_t) const noexcept;
        void setMemoryAddressRegister_1(const constants::NUMBER_STREAM&, std::uint32_t) const noexcept;

        /* FIFO status register */
        [[nodiscard]] std::uint8_t getFlagFIFOcontrol(const constants::NUMBER_STREAM&, const constants::FLAG_FIFO_CONTROL_REGISTER&) const noexcept;
        void setFlagFIFOcontrol(const constants::NUMBER_STREAM&, const constants::FLAG_FIFO_CONTROL_REGISTER&, std::uint8_t) const noexcept;

        void enable(const constants::NUMBER_STREAM&) const noexcept;
        void disable(const constants::NUMBER_STREAM&) const noexcept;

        std::uintptr_t getBaseAddress() const noexcept;
    };
}

#endif //STM32F4_DMA_HPP
