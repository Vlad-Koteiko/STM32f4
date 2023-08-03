/*! \file dma.hpp
    \brief class DMA

    Header file with a description of the dma class
*/
#ifndef STM32F4_DMA_HPP
#define STM32F4_DMA_HPP

#include "MWR.hpp"
#include "clockControl.hpp"

/// @brief Namespace for work with DMA
namespace drivers::dma
{
    /// @brief Base address DMA
    enum ADDRESSES_DMA : std::uintptr_t
    {
        DMA_1 = 0x40026000,    ///< Base address DMA1
        DMA_2 = 0x40026400     ///< Base address DMA2
    };

    /// @brief Namespaec constants DMA
    namespace constants
    {
        /// @brief Number stream
        enum NUMBER_STREAM : std::ptrdiff_t
        {
            Stream_0 = 0x10,    ///< Stream 0
            Stream_1 = 0x20,    ///< Stream 1
            Stream_2 = 0x40,    ///< Stream 2
            Stream_3 = 0x58,    ///< Stream 3
            Stream_4 = 0x70,    ///< Stream 4
            Stream_5 = 0x88,    ///< Stream 5
            Stream_6 = 0xA0,    ///< Stream 6
            Stream_7 = 0xB8     ///< Stream 7
        };

        /// @brief Flag interrupt status
        enum FLAG_INTERRUPT_STATUS : std::uint8_t
        {
            FIFO_ERROR_INTERRUPT        = 0,    ///< Fifo error interrupt
            DIRECT_MODE_ERROR_INTERRUPT = 2,    ///< Direct mode error interrupt
            TRANSFER_ERROR_INTERRUPT    = 3,    ///< Transfer error interrupt
            HALF_TRANSFER_INTERRUPT     = 4,    ///< Half transfer interrupt
            TRANSFER_COMPLETE_INTERRUPT = 5     ///< Transfer complete interrupt
        };

        /// @brief Flag stream configuration
        enum FLAG_STREAM_CONFIGURATION : std::uint8_t
        {
            STREAM_ENABLE                      = 0,     ///< Stream enable
            DIRECT_MODE_ERROR_INTERRUPT_ENABLE = 1,     ///< Direct mode error interrupt enable
            TRANSFER_ERROR_INTERRUPT_ENABLE    = 2,     ///< Transfer error interrupt enable
            HALF_TRANSFER_INTERRUPT_ENABLE     = 3,     ///< Half transfer interrupt enable
            TRANSFER_COMPLETE_INTERRUPT_ENABLE = 4,     ///< Transfer complete interrupt enable
            PERIPHERAL_FLOW_CONTROLLER         = 5,     ///< Peripheral flow controller
            DATA_TRANSFER_DIRECTION            = 6,     ///< Data transfer direction
            CIRCULAR_MODE                      = 8,     ///< Circular mode
            PERIPHERAL_INCREMENT_MODE          = 9,     ///< Peripheral increment mode
            MEMORY_INCREMENT_MODE              = 10,    ///< Memory increment mode
            PERIPHERAL_DATA_SIZE               = 11,    ///< Peripheral data size
            MEMORY_DATA_SIZE                   = 13,    ///< Memory data size
            PERIPHERAL_INCREMENT_OFFSET_SIZE   = 15,    ///< Peripheral increment offset size
            PRIORITY_LEVEL                     = 16,    ///< Priority level
            DOUBLE_BUFFER_MODE                 = 18,    ///< Double buffer mode
            CURRENT_TARGET                     = 19,    ///< Current target
            PERIPHERAL_BURST_TRANSFER_CONFIGURATION =
                21,                                     ///< Peripheral burst transfer configuration
            MEMORY_BURST_TRANSFER_CONFIGURATION = 23,    ///< Memory burst transfer configuration
            CHANNEL                             = 25     ///< Channel
        };

        /// @brief Flag fifo control register
        enum FLAG_FIFO_CONTROL_REGISTER : std::uint8_t
        {
            FIFO_THRESHOLD_SELECTION    = 0,    ///< Fifo threshold selection
            DIRECT_MODE_DISABLE         = 2,    ///< Direct mode disable
            FIFO_STATUS                 = 3,    ///< Fifo status
            FIFO_ERROR_INTERRUPT_ENABLE = 7     ///< Fifo error interrupt enable
        };

        /// @brief Data direction
        enum DATA_DIRECTION : std::uint8_t
        {
            PERIPHERAL_TO_MEMORY = 0,    ///< Peripheral to memory
            MEMORY_TO_PERIPHERAL = 1,    ///< Memory to peripheral
            MEMORY_TO_MEMORY     = 2,    ///< Memory to memory
        };

        /// @brief Data size
        enum DATA_SIZE : std::uint8_t
        {
            BYTE      = 0,    ///< Byte
            HALF_WORD = 1,    ///< Half word
            WORD      = 2     ///< Word
        };

        /// @brief Priority
        enum PRIORITY : std::uint8_t
        {
            LOW       = 0,    ///< Low
            MEDIUM    = 1,    ///< Medium
            HIGH      = 2,    ///< High
            VERY_HIGH = 3     ///< Very high
        };

        /// @brief Peripheral burst
        enum PERIPHERAL_BURST : std::uint8_t
        {
            SINGLE_TRANSFER = 0,    ///< Single transfer
            INCR4           = 1,    ///< Increment 4
            INCR8           = 2,    ///< Increment 8
            INCR16          = 3     ///< Increment 16
        };

        /// @brief Channel selection
        enum CHANNEL_SELECTION : std::uint8_t
        {
            CHANNEL_0 = 0,    ///< Channel 0
            CHANNEL_1 = 1,    ///< Channel 1
            CHANNEL_2 = 2,    ///< Channel 2
            CHANNEL_3 = 3,    ///< Channel 3
            CHANNEL_4 = 4,    ///< Channel 4
            CHANNEL_5 = 5,    ///< Channel 5
            CHANNEL_6 = 6,    ///< Channel 6
            CHANNEL_7 = 7     ///< Channel 7
        };

        /// @brief Status
        enum STATUS : std::uint8_t
        {
            DISABLE = 0,    ///< Disable
            ENABLE  = 1     ///< Enable
        };

        /// @brief Target
        enum TARGET : std::uint8_t
        {
            MEMORY0,    ///< Memory 0
            MEMORY1     ///< Memory 1
        };

        /// @brief Fifo threshold
        enum FIFO_THRESHOLD : std::uint8_t
        {
            FIFO_0_25 = 0,    ///< 1/4 full fifo
            FIFO_0_50 = 1,    ///< 1/2 full fifo
            FIFO_0_75 = 2,    ///< 3/4 full fifo
            FIFO_FULL = 3     ///< full fifo
        };
    };                        // namespace constants

    /// @brief Struct for configuration DMA
    struct DMA_Config
    {
        constants::STATUS         directModeErrorInterrupt;     ///< Direct mode error interrupt
        constants::STATUS         transferErrorInterrupt;       ///< Transfer error interrupt
        constants::STATUS         halfTransferInterrupt;        ///< Half transfer interrupt
        constants::STATUS         transferCompleteInterrupt;    ///< Transfer complete interrupt
        constants::STATUS         peripheralFlowController;     ///< Peripheral flow controller
        constants::DATA_DIRECTION dataTransferDirection;        ///< Data transfer direction
        constants::STATUS         circularMode;                 ///< Circular mode
        constants::STATUS         peripheralIncrement;          ///< peripheral increment
        constants::STATUS         memoryIncrement;              ///< memory increment
        constants::DATA_SIZE      peripheralDataSize;           ///< peripheral data size
        constants::DATA_SIZE      memoryDataSize;               ///< memory data size
        constants::STATUS   peripheralIncrementOffsetSize;    ///< peripheral increment offset size
        constants::PRIORITY priorityLevel;                    ///< priority level
        constants::STATUS   doubleBufferMode;                 ///< double buffer mode
        constants::TARGET   currentTarget;                    ///< current target
        constants::PERIPHERAL_BURST
            peripheralBurstTransferConfiguration;    ///< peripheral burst transfer configuration
        constants::PERIPHERAL_BURST
            memoryBurstTransferConfiguration;        ///< memory burst transfer configuration
        constants::CHANNEL_SELECTION channel;        ///< channel

        constants::FIFO_THRESHOLD FIFOThresholdSelection;     ///< FIFO threshold selection
        constants::STATUS         directModeDisableInvers;    ///< direct mode disable inversion
        constants::STATUS         fifoErrorInterrupt;         ///< FIFO error interrupt

        constants::NUMBER_STREAM stream;                      ///< stream
    };

    /// @brief Namespace for work with DMA
    class DMA
    {
        using readWriteRegister = libs::MWR;
        const drivers::clock::ClockControl &clockControl;    ///< Clock controll
        const std::uintptr_t                baseAddress;     ///< base address DMa

        /// @brief Registers DMA
        enum RegisterDMA : std::ptrdiff_t
        {
            LISR  = 0,       ///< DMA low interrupt status register,       Address offset: 0x00
            HISR  = 0x04,    ///< DMA high interrupt status register,      Address offset: 0x04
            LIFCR = 0x08,    ///< DMA low interrupt flag clear register,   Address offset: 0x08
            HIFCR = 0x0C     ///< DMA high interrupt flag clear register,  Address offset: 0x0C
        };

        /// @brief Stream registaers DMA
        enum RegisterSTREAM : std::ptrdiff_t
        {
            CR   = 0,       ///< DMA stream 0 configuration register         Address offset: 0x0
            NDTR = 0x04,    ///< DMA stream 0 number of data register        Address offset: 0x04
            PAR  = 0x08,    ///< DMA stream 0 peripheral address register    Address offset: 0x08
            M0AR = 0x0C,    ///< DMA stream 0 memory 0 address register      Address offset: 0x0C
            M1AR = 0x10,    ///< DMA stream 0 memory 1 address register      Address offset: 0x10
            FCR  = 0x14,    ///< DMA stream 0 FIFO control register          Address offset: 0x14
        };

        constexpr std::uint32_t getAddress(
            const drivers::dma::constants::NUMBER_STREAM &numberStream,
            std::ptrdiff_t                                offset) const noexcept
        {
            return static_cast<std::uint32_t>(baseAddress + numberStream + offset);
        }

        constexpr std::uintptr_t getAddress(std::ptrdiff_t offset) const noexcept
        {
            return (baseAddress + offset);
        }

        constexpr std::uint8_t getFlagFIFOcontrolValue(
            const drivers::dma::constants::FLAG_FIFO_CONTROL_REGISTER &flagFifoControlRegister,
            std::uint32_t                                              value) const noexcept
        {
            return ((value >> flagFifoControlRegister) & flagFifoControlRegister);
        }

        constexpr std::uint8_t getNumberStream(
            const constants::NUMBER_STREAM &numberStream) const noexcept
        {
            switch(numberStream)
            {
                case constants::Stream_0:
                    return 0;
                    break;
                case constants::Stream_1:
                    return 6;
                    break;
                case constants::Stream_2:
                    return 16;
                    break;
                case constants::Stream_3:
                    return 22;
                    break;
                case constants::Stream_4:
                    return 0;
                    break;
                case constants::Stream_5:
                    return 6;
                    break;
                case constants::Stream_6:
                    return 16;
                    break;
                case constants::Stream_7:
                    return 22;
                    break;
                default:
                    return 99;
                    break;
            }
        }

        bool getFlagInterruptStatus(const constants::NUMBER_STREAM &,
                                    const std::uint8_t &,
                                    bool) const noexcept;

    public:
        /// @brief Constructor DMA
        /// @param clockControl Clock controll
        /// @param addressesDma Base address DMA
        DMA(const drivers::clock::ClockControl &clockControl,
            const drivers::dma::ADDRESSES_DMA   addressesDma) :
            clockControl(clockControl), baseAddress(addressesDma)
        {
            baseAddress == DMA_1 ?
                clockControl.EnablePeripherals(drivers::clock::constants::DMA1_MODULE) :
                clockControl.EnablePeripherals(drivers::clock::constants::DMA2_MODULE);
        }

        /// @brief Get status flag interrupt
        /// @param numberStream Number stream
        /// @param flag Name flag
        /// @return status
        [[nodiscard]] bool getFlagInterrupt(
            const constants::NUMBER_STREAM         &numberStream,
            const constants::FLAG_INTERRUPT_STATUS &flag) const noexcept;

        /// @brief Clear flag interrupt
        /// @param numberStream Number stream
        /// @param flag Name flag
        void clearFlagInterrupt(const constants::NUMBER_STREAM         &numberStream,
                                const constants::FLAG_INTERRUPT_STATUS &flag) const noexcept;

        /// @brief Set stream configuration register
        /// @param numberStream Number stream
        /// @param flag Name flag
        /// @param value Value
        void setStreamConfigurationRegister(const constants::NUMBER_STREAM &numberStream,
                                            const constants::FLAG_STREAM_CONFIGURATION &flag,
                                            std::uint8_t value) const noexcept;

        /// @brief Set number data register
        /// @param numberStream Number stream
        /// @param number Number
        void setNumberDataRegister(const constants::NUMBER_STREAM &numberStream,
                                   std::uint16_t                   number) const noexcept;

        /// @brief Set peripheral address
        /// @param numberStream Number stream
        /// @param address Address
        void setPeripheralAddressRegister(const constants::NUMBER_STREAM &numberStream,
                                          std::uint32_t                   address) const noexcept;

        /// @brief Set memory address 0
        /// @param numberStream Number stream
        /// @param address Address
        void setMemoryAddressRegister_0(const constants::NUMBER_STREAM &numberStream,
                                        std::uint32_t                   address) const noexcept;

        /// @brief Set memory address 1
        /// @param numberStream Number stream
        /// @param address Address
        void setMemoryAddressRegister_1(const constants::NUMBER_STREAM &numberStream,
                                        std::uint32_t                   address) const noexcept;

        /// @brief Get flag fifo controll
        /// @param numberStream Number stream
        /// @param flagFifoControlRegister Flag fifo controll register
        /// @return value
        [[nodiscard]] std::uint8_t getFlagFIFOcontrol(
            const constants::NUMBER_STREAM              &numberStream,
            const constants::FLAG_FIFO_CONTROL_REGISTER &flagFifoControlRegister) const noexcept;

        /// @brief Get flag fifo controll
        /// @param numberStream Number stream
        /// @param flagFifoControlRegister Flag fifo controll register
        /// @param value Value
        void setFlagFIFOcontrol(
            const constants::NUMBER_STREAM              &numberStream,
            const constants::FLAG_FIFO_CONTROL_REGISTER &flagFifoControlRegister,
            std::uint8_t                                 value) const noexcept;

        /// @brief Enable stream DMA
        /// @param numberStream Number stream
        void enable(const constants::NUMBER_STREAM &numberStream) const noexcept;

        /// @brief Disable stream DMA
        /// @param numberStream Number stream
        void disable(const constants::NUMBER_STREAM &numberStream) const noexcept;

        /// @brief Get base address DMA
        /// @return Base address
        std::uintptr_t getBaseAddress() const noexcept;

        /// @brief Configuration DMA
        /// @param dmaConfig Struct with configuration DMA
        void ConfigurationDma(DMA_Config dmaConfig) const noexcept;
    };
}    // namespace drivers::dma

#endif    // STM32F4_DMA_HPP
