
#include "spi.h"

namespace drivers::spi
{
    SPI::SPI(drivers::clock::ClockControl curClock, ADDRESSES_SPI spi) : clock(curClock), baseAddress(spi){
        switch (spi) {
            case SPI1:
                clock.EnablePeripherals(drivers::clock::SPI1_MODULE); break;
            case SPI2:
                clock.EnablePeripherals(drivers::clock::SPI2_MODULE); break;
            case SPI3:
                clock.EnablePeripherals(drivers::clock::SPI3_MODULE); break;
            case SPI4:
                clock.EnablePeripherals(drivers::clock::SPI4_MODULE); break;
            case SPI5:
                clock.EnablePeripherals(drivers::clock::SPI5_MODULE); break;
            case SPI6:
                clock.EnablePeripherals(drivers::clock::SPI6_MODULE); break;
        }
    }

    void SPI::Enable() noexcept {
        libs::MWR::setBit(baseAddress + CR1, SPE);
    }

    void SPI::Disable() noexcept {
        libs::MWR::resetBit(baseAddress + CR1, SPE);
    }

    bool SPI::IsEnabled() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, SPE);
    }

    void SPI::SetMode(SPI_MODE m) noexcept {
        switch (m) {
            case MASTER: libs::MWR::setBit(baseAddress + CR1, MSTR); break;
            case SLAVE: libs::MWR::resetBit(baseAddress + CR1, MSTR); break;
        }
    }

    SPI_MODE SPI::GetMode() noexcept {
        return (SPI_MODE)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, MSTR);
    }

    void SPI::SetStandard(SPI_STANDART s) noexcept {
        switch (s) {
            case MOTOROLA: libs::MWR::resetBit(baseAddress + CR2, FRF);
            case TI: libs::MWR::setBit(baseAddress + CR2, FRF);
        }
    }

    SPI_STANDART SPI::GetStandard() noexcept {
        return (SPI_STANDART)libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, FRF);
    }

    void SPI::SetClockPhase(CLOCK_PHASE cp) noexcept {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 1 << CPHA);
        libs::MWR::modifySetRegister(baseAddress + CR1, cp << CPHA);
    }

    CLOCK_PHASE SPI::GetClockPhase() noexcept {
        return (CLOCK_PHASE)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, CPHA);
    }

    void SPI::SetClockPolarity(CLOCK_POLARITY cp) noexcept {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 1 << CPOL);
        libs::MWR::modifySetRegister(baseAddress + CR1, cp << CPOL);
    }

    CLOCK_POLARITY SPI::GetClockPolarity() noexcept {
        return (CLOCK_POLARITY)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, CPOL);
    }

    void SPI::SetBaudRatePrescaler(BAUD_RATE_PRESCALER brp) noexcept {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 0x1F << BR);
        libs::MWR::modifySetRegister(baseAddress + CR1, brp << BR);
    }

    BAUD_RATE_PRESCALER SPI::GetBaudRatePrescaler() noexcept {
        return (BAUD_RATE_PRESCALER)((libs::MWR::read_register<std::uint32_t>(baseAddress + CR1) & 0x0038) >> BR );
    }

    void SPI::SetTransferBitOrder(TRANSFER_BIT_ORDER tbo) noexcept {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 1 << LSBFIRST);
        libs::MWR::modifySetRegister(baseAddress + CR1, tbo << LSBFIRST);
    }

    TRANSFER_BIT_ORDER SPI::GetTransferBitOrder() noexcept {
        return (TRANSFER_BIT_ORDER)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, LSBFIRST);
    }

    void SPI::SetTransferDirection(TRANSFER_DIRECTION td) noexcept {
        libs::MWR::modifyResetRegister(baseAddress + CR1, 0xC400);
        switch (td) {
            case FULL_DUPLEX:
                libs::MWR::resetBit(baseAddress + CR1, RXONLY); break;
            case SIMPLEX_RX:
                libs::MWR::setBit(baseAddress + CR1, RXONLY); break;
            case HALF_DUPLEX_RX:
                libs::MWR::modifySetRegister(baseAddress + CR1, 1 << BIDIMODE); break;
            case HALF_DUPLEX_TX:
                libs::MWR::modifySetRegister(baseAddress + CR1, (1 << BIDIMODE) | (1 << BIDIOE)); break;
        }
    }

    TRANSFER_DIRECTION SPI::GetTransferDirection() noexcept {
        bool rxonly = libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, RXONLY);
        bool bidimode = libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, BIDIMODE);
        bool bidioe = libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, BIDIOE);
        if(bidimode == true)
        {
            if(bidioe == true)
                return HALF_DUPLEX_TX;
            else
                return HALF_DUPLEX_RX;
        }

        if(rxonly == true)
            return SIMPLEX_RX;
        else return FULL_DUPLEX;
    }

    void SPI::SetDataWidth(DATA_WIDTH dw) noexcept {
        libs::MWR::resetBit(baseAddress + CR1, DFF);
        libs::MWR::modifySetRegister(baseAddress + CR1, dw << DFF);
    }

    DATA_WIDTH SPI::GetDataWidth() noexcept {
        return (DATA_WIDTH)libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, DFF);
    }

    void SPI::EnableCRC() noexcept {
        libs::MWR::setBit(baseAddress + CR1, CRCEN);
    }

    void SPI::DisableCRC() noexcept {
        libs::MWR::resetBit(baseAddress + CR1, CRCEN);
    }

    bool SPI::IsEnabledCRC() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, CRCEN);
    }

    void SPI::SetCRCNext() noexcept {
        libs::MWR::setBit(baseAddress + CR1, CRCNEXT);
    }

    void SPI::SetCRCPolynomial(std::uint16_t crcPoly) noexcept {
        libs::MWR::write_register(baseAddress + CRCPR, crcPoly);
    }

    std::uint16_t SPI::GetCRCPolynomial() noexcept {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + CRCPR);
    }

    std::uint16_t SPI::GetRxCRC() noexcept {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + RXCRCR);
    }

    std::uint16_t SPI::GetTxCRC() noexcept {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + TXCRCR);
    }

    void SPI::SetNSSMode(NSS n) noexcept {

        libs::MWR::resetBit(baseAddress + CR1, SSM);
        libs::MWR::resetBit(baseAddress + CR2, SSOE);

        switch (n) {
            case SOFT:
                libs::MWR::setBit(baseAddress + CR1, SSM); break;
            case HARD_OUTPUT:
                libs::MWR::setBit(baseAddress + CR2, SSOE); break;
        }
    }

    NSS SPI::GetNSSMode() noexcept {
        bool ssm, ssoe;
        ssm = libs::MWR::readBit<std::uint32_t>(baseAddress + CR1, SSM);
        ssoe = libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, SSOE);

        if(ssm == true)
            return SOFT;
        else
        {
            if(ssoe == true)
                return HARD_OUTPUT;
            else
                return HARD_INPUT;
        }
    }

    bool SPI::IsActiveFlag_RXNE() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, RXNE);
    }

    bool SPI::IsActiveFlag_TXE() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, TXE);
    }

    bool SPI::IsActiveFlag_CRCERR() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, CRCERR);
    }

    bool SPI::IsActiveFlag_MODF() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, MODF);
    }

    bool SPI::IsActiveFlag_OVR() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, OVR);
    }

    bool SPI::IsActiveFlag_BSY() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, BSY);
    }

    bool SPI::IsActiveFlag_FRE() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + SR, FRE);
    }

    void SPI::ClearFlag_CRCERR() noexcept {
        libs::MWR::resetBit(baseAddress + SR, CRCERR);
    }

    void SPI::ClearFlag_MODF() noexcept {
        libs::MWR::read_register<std::uint32_t>(baseAddress + SR);
        libs::MWR::resetBit(baseAddress + CR1, SPE);
    }

    void SPI::ClearFlag_OVR() noexcept {
        libs::MWR::read_register<std::uint32_t>(baseAddress + DR);
        libs::MWR::read_register<std::uint32_t>(baseAddress + SR);
    }

    void SPI::ClearFlag_FRE() noexcept {
        libs::MWR::read_register<std::uint32_t>(baseAddress + SR);
    }

    void SPI::EnableIT_ERR() noexcept {
        libs::MWR::setBit(baseAddress + CR2, ERRIE);
    }

    void SPI::EnableIT_RXNE() noexcept {
        libs::MWR::setBit(baseAddress + CR2, RXNEIE);
    }

    void SPI::EnableIT_TXE() noexcept {
        libs::MWR::setBit(baseAddress + CR2, TXEIE);
    }

    void SPI::DisableIT_ERR() noexcept {
        libs::MWR::resetBit(baseAddress + CR2, ERRIE);
    }

    void SPI::DisableIT_RXNE() noexcept {
        libs::MWR::resetBit(baseAddress + CR2, RXNEIE);
    }

    void SPI::DisableIT_TXE() noexcept {
        libs::MWR::resetBit(baseAddress + CR2, TXEIE);
    }

    bool SPI::IsEnabledIT_ERR() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, ERRIE);
    }

    bool SPI::IsEnabledIT_RXNE() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, RXNEIE);
    }

    bool SPI::IsEnabledIT_TXE() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, TXEIE);
    }

    void SPI::EnableDMAReq_RX() noexcept {
        libs::MWR::setBit(baseAddress + CR2, RXDMAEN);
    }

    void SPI::DisableDMAReq_RX() noexcept {
        libs::MWR::resetBit(baseAddress + CR2, RXDMAEN);
    }

    bool SPI::IsEnabledDMAReq_RX() noexcept {
        return libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, RXDMAEN);
    }

    void SPI::EnableDMAReq_TX() noexcept {
        libs::MWR::setBit(baseAddress + CR2, TXDMAEN);
    }

    void SPI::DisableDMAReq_TX() noexcept {
        libs::MWR::resetBit(baseAddress + CR2, TXDMAEN);
    }

    bool SPI::IsEnabledDMAReq_TX() noexcept {
        return  libs::MWR::readBit<std::uint32_t>(baseAddress + CR2, TXDMAEN);
    }

    std::uint8_t SPI::ReceiveData8() noexcept {
        return libs::MWR::read_register<std::uint8_t>(baseAddress + DR);
    }

    std::uint16_t SPI::ReceiveData16() noexcept {
        return libs::MWR::read_register<std::uint16_t>(baseAddress + DR);
    }

    void SPI::TransmitData8(std::uint8_t txData) noexcept {
        libs::MWR::write_register(baseAddress + DR, txData);
    }

    void SPI::TransmitData16(std::uint16_t txData) noexcept {
        libs::MWR::write_register(baseAddress + DR, txData);
    }
}