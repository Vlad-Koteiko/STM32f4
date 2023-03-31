//
// Created by koteiko_vv on 28.03.2023.
//

#include "dma.h"

namespace drivers::DMA
{
    DMA::DMA(const drivers::clock::ClockControl &clockControl, const drivers::DMA::ADDRESSES_DMA addressesDma)
                                                        : clockControl(clockControl), baseAddress(addressesDma) {
        switch(baseAddress) {
            case DMA_1:
                clockControl.EnablePeripherals(drivers::clock::DMA_1_MODULE);
                break;
            case DMA_2:
                clockControl.EnablePeripherals(drivers::clock::DMA_2_MODULE);
                break;
        }
    }

    std::uint8_t DMA::getNumberStream(const NUMBER_STREAM& numberStream) const noexcept {
        std::uint8_t temp;
        switch (numberStream) {
            case Stream_0: temp = 0;
                break;
            case Stream_1: temp = 6;
                break;
            case Stream_2: temp = 16;
                break;
            case Stream_3: temp = 22;
                break;
            case Stream_4: temp = 0;
                break;
            case Stream_5: temp = 6;
                break;
            case Stream_6: temp = 16;
                break;
            case Stream_7: temp = 22;
                break;
        }
        return temp;
    }

    bool DMA::GetFlagInterruptStatus(const NUMBER_STREAM& numberStream, std::uint8_t offset, bool getFlag) const noexcept{
        bool temp = false;
        switch (numberStream) {
            case Stream_0:
            case Stream_1:
            case Stream_2:
            case Stream_3:
                if (getFlag)
                {
                    temp = libs::MWR::readBit<std::uint32_t>(baseAddress + LISR, getNumberStream(numberStream) + offset);

                } else
                {
                    libs::MWR::resetBit(baseAddress + LISR, getNumberStream(numberStream) + offset);
                }
                break;
            case Stream_4:
            case Stream_5:
            case Stream_6:
            case Stream_7:
                if (getFlag)
                {
                    temp = libs::MWR::readBit<std::uint32_t>(baseAddress + HISR, getNumberStream(numberStream) + offset);

                } else
                {
                    libs::MWR::resetBit(baseAddress + HISR, getNumberStream(numberStream) + offset);
                }
                break;
        }
        return temp;
    }

    void DMA::Enable(const NUMBER_STREAM& numberStream) const noexcept{
       libs::MWR::setBit(baseAddress + numberStream + CR, 0);
    }

    void DMA::Deseable(const drivers::DMA::NUMBER_STREAM& numberStream) const noexcept {
        libs::MWR::resetBit(baseAddress + numberStream + CR, 0);
    }

    void DMA::Start_IT(const NUMBER_STREAM& numberStream ,uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength) const noexcept
    {
//        HAL_StatusTypeDef status = HAL_OK;
//
//        /* calculate DMA base and stream number */
//        DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
//
//        /* Check the parameters */
//        assert_param(IS_DMA_BUFFER_SIZE(DataLength));
//
//        /* Process locked */
//        __HAL_LOCK(hdma);
//
//        if(HAL_DMA_STATE_READY == hdma->State)
//        {
//            /* Change DMA peripheral state */
//            hdma->State = HAL_DMA_STATE_BUSY;
//
//            /* Initialize the error code */
//            hdma->ErrorCode = HAL_DMA_ERROR_NONE;

            /* Configure the source, destination address and the data length */
            SetConfig(numberStream,SrcAddress, DstAddress, DataLength);

            /* Clear all interrupt flags at correct offset within the register */
//            regs->IFCR = 0x3FU << hdma->StreamIndex;
//            libs::MWR::write_register(baseAddress + LIFCR, 0x3F << 16);
            ClearFlagDirectModeError(numberStream);
            ClearFlagFifoError(numberStream);
            ClearFlagHalfTransferError(numberStream);
            ClearFlagTransferCompleteError(numberStream);
            ClearFlagTransferError(numberStream);

//
//
            /* Enable Common interrupts*/
//            hdma->Instance->CR  |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;
//            libs::MWR::write_register(baseAddress + numberStream + CR, (1 << 4 | 1 << 2 | 1 << 1));
            SetTransferCompleteInterruptEnable(numberStream);
            SetTransferErrorInterruptEnable(numberStream);
            SetDirectModeErrorInterruptEnable(numberStream);

//            if(hdma->XferHalfCpltCallback != NULL)
//            {
//                hdma->Instance->CR  |= DMA_IT_HT;
//                 libs::MWR::setBit(baseAddress + numberStream + CR, 3);
                   SetHalfTransferInterruptEnable(numberStream);
//            }
//
//            /* Enable the Peripheral */
//            __HAL_DMA_ENABLE(hdma);
            Enable(numberStream);
//        }
//        else
//        {
//            /* Process unlocked */
//            __HAL_UNLOCK(hdma);
//
//            /* Return error status */
//            status = HAL_BUSY;
//        }
    }

    void DMA::SetConfig(const NUMBER_STREAM& numberStream, const std::uintptr_t SrcAddress, std::uintptr_t DstAddress,
                            std::uint32_t DataLength) const noexcept {
        /* Clear DBM bit */
        libs::MWR::resetBit(baseAddress + numberStream + CR, 18);
        /* Configure DMA Stream data length */
        libs::MWR::write_register(baseAddress + numberStream + NDTR,DataLength);
        /* Configure DMA Stream destination address */
        libs::MWR::write_register(baseAddress + numberStream + PAR, DstAddress);
        /* Configure DMA Stream source address */
        libs::MWR::write_register(baseAddress + numberStream + M0AR, SrcAddress);
    }

    bool DMA::GetFlagFifoError(const NUMBER_STREAM& numberStream) const noexcept{
        return GetFlagInterruptStatus(numberStream, 0, true);
    }

    bool DMA::GetFlagDirectModeError(const NUMBER_STREAM& numberStream) const noexcept{
        return GetFlagInterruptStatus(numberStream, 2, true);
    }

    bool DMA::GetFlagTransferError(const NUMBER_STREAM& numberStream) const noexcept{
        return GetFlagInterruptStatus(numberStream, 3, true);
    }

    bool DMA::GetFlagHalfTransferError(const NUMBER_STREAM& numberStream) const noexcept {
        return GetFlagInterruptStatus(numberStream, 4, true);
    }

    bool DMA::GetFlagTransferCompleteError(const NUMBER_STREAM& numberStream) const noexcept {
        return GetFlagInterruptStatus(numberStream, 5, true);
    }

    void DMA::ClearFlagFifoError(const NUMBER_STREAM& numberStream) const noexcept {
        GetFlagInterruptStatus(numberStream, 0, false);
    }

    void DMA::ClearFlagDirectModeError(const NUMBER_STREAM& numberStream) const noexcept {
        GetFlagInterruptStatus(numberStream, 2, false);
    }

    void DMA::ClearFlagTransferError(const drivers::DMA::NUMBER_STREAM& numberStream) const noexcept {
        GetFlagInterruptStatus(numberStream, 3, false);
    }

    void DMA::ClearFlagHalfTransferError(const drivers::DMA::NUMBER_STREAM& numberStream) const noexcept {
        GetFlagInterruptStatus(numberStream, 4, false);
    }

    void DMA::ClearFlagTransferCompleteError(const drivers::DMA::NUMBER_STREAM& numberStream) const noexcept {
        GetFlagInterruptStatus(numberStream, 5, false);
    }

    void DMA::ChannelSelection(const NUMBER_STREAM& numberStream, std::uint8_t channel) const noexcept{
        libs::MWR::write_register(baseAddress + numberStream + CR, static_cast<std::uint32_t>(channel << 25));
    }

    void DMA::SetTransferCompleteInterruptEnable(const NUMBER_STREAM& numberStream) const noexcept {
        libs::MWR::setBit(baseAddress + numberStream + CR, 4);
    }

    void DMA::ClearTransferCompleteInterruptEnable(const NUMBER_STREAM& numberStream) const noexcept {
        libs::MWR::resetBit(baseAddress + numberStream + CR, 4);
    }

    void DMA::SetTransferErrorInterruptEnable(const drivers::DMA::NUMBER_STREAM& numberStream) const noexcept {
        libs::MWR::setBit(baseAddress + numberStream + CR, 2);
    }

    void DMA::ClearTransferErrorInterruptEnable(const NUMBER_STREAM& numberStream) const noexcept {
        libs::MWR::resetBit(baseAddress + numberStream + CR, 2);
    }

    void DMA::SetDirectModeErrorInterruptEnable(const NUMBER_STREAM& numberStream) const noexcept {
        libs::MWR::setBit(baseAddress + numberStream + CR, 1);
    }

    void DMA::ClearDirectModeErrorInterruptEnable(const NUMBER_STREAM& numberStream) const noexcept {
        libs::MWR::resetBit(baseAddress + numberStream + CR, 1);
    }

    void DMA::SetHalfTransferInterruptEnable(const NUMBER_STREAM& numberStream) const noexcept {
        libs::MWR::setBit(baseAddress + numberStream + CR, 3);
    }

    void DMA::ClearHalfTransferInterruptEnable(const NUMBER_STREAM& numberStream) const noexcept {
        libs::MWR::resetBit(baseAddress + numberStream + CR, 3);
    }

    void DMA::DataTransferDirection(const NUMBER_STREAM& numberStream, const DATA_TRANSFER& dataTransfer) const noexcept {
        libs::MWR::modifySetRegister(baseAddress + numberStream + CR, dataTransfer << 6);
    }



}




