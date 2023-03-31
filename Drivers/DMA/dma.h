//
// Created by koteiko_vv on 28.03.2023.
//
#ifndef STM32F4_DMA_H
#define STM32F4_DMA_H

#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::DMA
{
    enum ADDRESSES_DMA : std::uintptr_t
    {
        DMA_1 = 0x40026000,
        DMA_2 = 0x40026400,
    };

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

    enum DATA_TRANSFER : std::uint8_t
    {
        PERIPHERAL_TO_MEMORY = 0,
        MEMORY_TO_PERIPHERAL = 1,
        MEMORY_TO_MEMORY = 2,
    };

    class DMA {

        const drivers::clock::ClockControl &clockControl;
        const std::uintptr_t baseAddress;

        enum RegisterDMA : std::ptrdiff_t {
            LISR    = 0,    /*!< DMA low interrupt status register,       Address offset: 0x00 */
            HISR    = 0x04, /*!< DMA high interrupt status register,      Address offset: 0x04 */
            LIFCR   = 0x08, /*!< DMA low interrupt flag clear register,   Address offset: 0x08 */
            HIFCR   = 0x0C, /*!< DMA high interrupt flag clear register,  Address offset: 0x0C */
        };

        enum RegisterSTREAM : std::ptrdiff_t {
            CR   = 0, /*!< DMA stream 0 configuration register            Address offset: 0x0 */
            NDTR = 0x04, /*!< DMA stream 0 number of data register        Address offset: 0x04 */
            PAR  = 0x08, /*!< DMA stream 0 peripheral address register    Address offset: 0x08 */
            M0AR = 0x0C, /*!< DMA stream 0 memory 0 address register      Address offset: 0x0C */
            M1AR = 0x10, /*!< DMA stream 0 memory 1 address register      Address offset: 0x10 */
            FCR  = 0x14, /*!< DMA stream 0 FIFO control register          Address offset: 0x14 */
        };

        std::uint8_t getNumberStream(const NUMBER_STREAM&) const noexcept;

        bool GetFlagInterruptStatus(const NUMBER_STREAM&, std::uint8_t, bool) const noexcept;

    public:

        DMA(const drivers::clock::ClockControl &clockControl,const ADDRESSES_DMA addressesDma);
        void Start_IT(const NUMBER_STREAM& numberStream ,std::uint32_t SrcAddress, std::uint32_t DstAddress, std::uint32_t DataLength) const noexcept;
        void SetConfig(const NUMBER_STREAM& ,const std::uintptr_t SrcAddress, std::uintptr_t DstAddress, std::uint32_t DataLength) const noexcept;

        void Enable(const NUMBER_STREAM&) const noexcept;
        void Deseable(const NUMBER_STREAM&) const noexcept;

        /* Interuuupt status register */
        bool GetFlagFifoError(const NUMBER_STREAM&) const noexcept;
        bool GetFlagDirectModeError(const NUMBER_STREAM&) const noexcept;
        bool GetFlagTransferError(const NUMBER_STREAM&) const noexcept;
        bool GetFlagHalfTransferError(const NUMBER_STREAM&) const noexcept;
        bool GetFlagTransferCompleteError(const NUMBER_STREAM&) const noexcept;

        /* Clear flag interrupt */
        void ClearFlagFifoError(const NUMBER_STREAM&) const noexcept;
        void ClearFlagDirectModeError(const NUMBER_STREAM&) const noexcept;
        void ClearFlagTransferError(const NUMBER_STREAM&) const noexcept;
        void ClearFlagHalfTransferError(const NUMBER_STREAM&) const noexcept;
        void ClearFlagTransferCompleteError(const NUMBER_STREAM&) const noexcept;

        /* DMA stream x configuration register */
        void ChannelSelection(const NUMBER_STREAM&, std::uint8_t) const noexcept;
        void SetTransferCompleteInterruptEnable(const NUMBER_STREAM&) const noexcept;
        void ClearTransferCompleteInterruptEnable(const NUMBER_STREAM&) const noexcept;
        void SetTransferErrorInterruptEnable(const NUMBER_STREAM&) const noexcept;
        void ClearTransferErrorInterruptEnable(const NUMBER_STREAM&) const noexcept;
        void SetDirectModeErrorInterruptEnable(const NUMBER_STREAM&) const noexcept;
        void ClearDirectModeErrorInterruptEnable(const NUMBER_STREAM&) const noexcept;
        void SetHalfTransferInterruptEnable(const NUMBER_STREAM&) const noexcept;
        void ClearHalfTransferInterruptEnable(const NUMBER_STREAM&) const noexcept;
        void SetPeripheralFlowController(const NUMBER_STREAM&) const noexcept;
        void ClearPeripheralFlowController(const NUMBER_STREAM&) const noexcept;
        void DataTransferDirection(const NUMBER_STREAM&,const DATA_TRANSFER&) const noexcept;
        void EnableCircularMode() const noexcept;





    };
}

#endif //STM32F4_DMA_H
