
#include "can.hpp"

namespace drivers::can
{
    Can::Can(ADDRESSES_CAN address, const drivers::clock::ClockControl& clockCl) :
        baseAddress(address), clockControl(clockCl)
    {
        switch(address)
        {
            case CAN1:
                clockControl.EnablePeripherals(drivers::clock::constants::CAN1_MODULE);
                break;

            case CAN2:
                clockControl.EnablePeripherals(drivers::clock::constants::CAN2_MODULE);
                break;
        }
    }

    bool Can::init(CanInitStruct ci)
    {
        std::uint8_t waitAck    = 0;
        bool         statusInit = false;

        // request initialisation
        libs::MWR::setBit(baseAddress + CAN_MCR, INRQ);

        // wait the acknowledge
        while((libs::MWR::readBit<std::uint32_t>(baseAddress + CAN_MSR, INAK) == false) &&
              (waitAck < delayCan))
        {
            clockControl.mDelay(1);
            waitAck++;
        }

        // exit from sleep mode
        libs::MWR::resetBit(baseAddress + CAN_MCR, SLEEP);

        if(libs::MWR::readBit<std::uint32_t>(baseAddress + CAN_MSR, INAK))
        {
            if(ci.timeTriggeredMode)
                libs::MWR::setBit(baseAddress + CAN_MCR, TTCM);
            else
                libs::MWR::resetBit(baseAddress + CAN_MCR, TTCM);

            if(ci.autoBusOff)
                libs::MWR::setBit(baseAddress + CAN_MCR, ABOM);
            else
                libs::MWR::resetBit(baseAddress + CAN_MCR, ABOM);

            if(ci.autoWakeUp)
                libs::MWR::setBit(baseAddress + CAN_MCR, AWUM);
            else
                libs::MWR::resetBit(baseAddress + CAN_MCR, AWUM);

            if(ci.autoRetransmission)
                libs::MWR::setBit(baseAddress + CAN_MCR, NART);
            else
                libs::MWR::resetBit(baseAddress + CAN_MCR, NART);

            if(ci.receiveFifoLocked)
                libs::MWR::setBit(baseAddress + CAN_MCR, RFLM);
            else
                libs::MWR::resetBit(baseAddress + CAN_MCR, RFLM);

            if(ci.transmitFifoPriority)
                libs::MWR::setBit(baseAddress + CAN_MCR, TXFP);
            else
                libs::MWR::resetBit(baseAddress + CAN_MCR, TXFP);

            libs::MWR::write_register(
                baseAddress + CAN_BTR,
                ((std::uint32_t)ci.mode << 30) | ((std::uint32_t)(ci.syncJumpWidth - 1) << 24) |
                    ((std::uint32_t)(ci.timeSeg1 - 1) << 16) |
                    ((std::uint32_t)(ci.timeSeg2 - 1) << 20) | ((std::uint32_t)ci.prescaler - 1));
            statusInit = true;
        }
        else
        {
            statusInit = false;
        }

        return statusInit;
    }

    void Can::deinit()
    {}

    void Can::configGpioForCan(drivers::port::ADDRESSES_PORT     portH,
                               drivers::port::ADDRESSES_PORT     portL,
                               drivers::port::PIN_NUMBER         pinH,
                               drivers::port::PIN_NUMBER         pinL,
                               drivers::port::ALTERNATE_FUNCTION af)
    {
        // init GPIO CANH
        drivers::port::Gpio port_tx(clockControl, portH);
        port_tx.SetPinMode(pinH, drivers::port::ALTERNATE_FUNCT);
        port_tx.SetPinSpeed(pinH, drivers::port::VERY_HIGH_SPEED);
        port_tx.SetAFPin(pinH, af);

        // init GPIO CANL
        drivers::port::Gpio port_rx(clockControl, portL);
        port_rx.SetPinMode(pinL, drivers::port::ALTERNATE_FUNCT);
        port_rx.SetPinSpeed(pinL, drivers::port::VERY_HIGH_SPEED);
        port_rx.SetAFPin(pinL, af);
    }

    bool Can::start()
    {
        std::uint8_t waitAck    = 0;
        bool         statusInit = false;
        // request leave initialization
        libs::MWR::resetBit(baseAddress + CAN_MCR, INRQ);

        waitAck = 0;

        // wait the acknowledge
        while((libs::MWR::readBit<std::uint32_t>(baseAddress + CAN_MSR, INAK) == true) &&
              (waitAck < delayCan))
        {
            clockControl.mDelay(1);
            waitAck++;
        }

        if(libs::MWR::readBit<std::uint32_t>(baseAddress + CAN_MSR, INAK) == true)
            statusInit = false;
        else
            statusInit = true;

        return statusInit;
    }

    bool Can::stop()
    {
        std::uint8_t waitAck = 0;

        // request initialization
        libs::MWR::setBit(CAN_MCR, INRQ);

        while((libs::MWR::readBit<std::uint32_t>(CAN_MSR, INAK) == false) && (waitAck < delayCan))
        {
            clockControl.mDelay(1);
            waitAck++;
        }

        // exit from sleep mode
        libs::MWR::resetBit(CAN_MCR, SLEEP);
        return true;
    }

    bool Can::addTxMessage(CanTxMsg txMsg, TxMailbox* tmb)
    {
        bool         statusTx = false;
        std::uint8_t emptyTxMailbox;
        // check that all the Tx mailboxes are not full
        if((mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME0) == true) ||
           (mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME1) == true) ||
           (mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME2) == true))
        {
            emptyTxMailbox =
                (mwr::read_register<std::uint32_t>(baseAddress + CAN_TSR) & 0x0300'0000) >> CODE;

            *tmb = static_cast<TxMailbox>(emptyTxMailbox);

            // set up the ID
            if(txMsg.IDE == 0)
            {
                mwr::write_register<std::uint32_t>(
                    baseAddress + CAN_TIxR[emptyTxMailbox],
                    (txMsg.stdId << T_STID_EXID) | ((std::uint32_t)txMsg.RTR << T_RTR));
            }
            else
            {
                mwr::write_register<std::uint32_t>(baseAddress + CAN_TIxR[emptyTxMailbox],
                                                   (txMsg.extId << T_EXID) |
                                                       ((std::uint32_t)txMsg.IDE << T_IDE) |
                                                       ((std::uint32_t)txMsg.RTR << T_RTR));
            }

            // set up the DLC
            mwr::write_register<std::uint32_t>(baseAddress + CAN_TDTxR[emptyTxMailbox], txMsg.DLC);

            // Set up the Transmit Global Time mode
            if(txMsg.transmitGlobalTime)
            {
                mwr::setBit(baseAddress + CAN_TDTxR[emptyTxMailbox], T_TGT);
            }

            // Set up the data field
            mwr::write_register<std::uint32_t>(baseAddress + CAN_TDHxR[emptyTxMailbox],
                                               ((std::uint32_t)txMsg.data[7] << T_DATA7) |
                                                   ((std::uint32_t)txMsg.data[6] << T_DATA6) |
                                                   ((std::uint32_t)txMsg.data[5] << T_DATA5) |
                                                   ((std::uint32_t)txMsg.data[4] << T_DATA4));

            mwr::write_register<std::uint32_t>(baseAddress + CAN_TDLxR[emptyTxMailbox],
                                               ((std::uint32_t)txMsg.data[3] << T_DATA3) |
                                                   ((std::uint32_t)txMsg.data[2] << T_DATA2) |
                                                   ((std::uint32_t)txMsg.data[1] << T_DATA1) |
                                                   ((std::uint32_t)txMsg.data[0] << T_DATA0));

            // Request transmission
            mwr::setBit(baseAddress + CAN_TIxR[emptyTxMailbox], T_TXRQ);

            statusTx = true;
        }
        else
        {
            statusTx = false;
        }

        return statusTx;
    }

    void Can::abortTxRequest(TxMailbox mailBox)
    {
        if(mailBox == TxMailbox_0)
        {
            mwr::setBit(baseAddress + CAN_TSR, ABRQ0);
        }
        if(mailBox == TxMailbox_1)
        {
            mwr::setBit(baseAddress + CAN_TSR, ABRQ1);
        }
        if(mailBox == TxMailbox_2)
        {
            mwr::setBit(baseAddress + CAN_TSR, ABRQ2);
        }
    }

    std::uint8_t Can::getTxMailboxesFreeLevel()
    {
        std::uint8_t freeLevel = 0x00;
        if(mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME0))
        {
            freeLevel++;
        }
        if(mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME1))
        {
            freeLevel++;
        }
        if(mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME2))
        {
            freeLevel++;
        }
        return freeLevel;
    }

    bool Can::isTxMessagePending(TxMailbox mailBox)
    {
        bool value = false;
        if(mailBox == TxMailbox_0)
        {
            value = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME0);
        }
        if(mailBox == TxMailbox_1)
        {
            value = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME1);
        }
        if(mailBox == TxMailbox_2)
        {
            value = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME2);
        }
        return value;
    }

    std::uint16_t Can::getTxTimestamp(TxMailbox mailBox)
    {
        std::uint16_t timeStamp;
        if(mailBox == TxMailbox_0)
        {
            timeStamp =
                mwr::read_register<std::uint32_t>(baseAddress + CAN_TDT0R) & 0xFFFF'0000 >> T_TIME;
        }
        if(mailBox == TxMailbox_1)
        {
            timeStamp =
                mwr::read_register<std::uint32_t>(baseAddress + CAN_TDT1R) & 0xFFFF'0000 >> T_TIME;
        }
        if(mailBox == TxMailbox_2)
        {
            timeStamp =
                mwr::read_register<std::uint32_t>(baseAddress + CAN_TDT2R) & 0xFFFF'0000 >> T_TIME;
        }
        return timeStamp;
    }

    bool Can::getRxMessage(CanRxMsg* msg)
    {
        bool          value      = true;
        std::uint32_t regCanRf0r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RF0R);
        if(regCanRf0r & 0x3 != 0)
        {
            std::uint32_t regCanRi0r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RI0R);
            msg->IDE                 = regCanRi0r & 0x4 >> R_IDE;
            msg->RTR                 = regCanRi0r & 0x2 >> R_RTR;
            if(msg->IDE == 1)
            {
                msg->extId = regCanRi0r & 0xFFFF'FFF8 >> R_EXID;
            }
            else
            {
                msg->stdId = regCanRi0r & 0xFFE0'0000 >> R_STID_EXID;
            }
            std::uint32_t regCanRdt0r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RDT0R);
            msg->filterMatchIndex     = regCanRdt0r & 0x0000'FF00 >> R_FMI;
            msg->timestamp            = regCanRdt0r & 0xFFFF'0000 >> R_TIME;
            msg->DLC                  = regCanRdt0r & 0x0000'000F >> R_DLC;

            std::uint32_t regCanRdl0r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RDL0R);

            msg->data[0] = regCanRdl0r & 0x0000'00FF >> R_DATA0;
            msg->data[1] = regCanRdl0r & 0x0000'FF00 >> R_DATA1;
            msg->data[2] = regCanRdl0r & 0x00FF'0000 >> R_DATA2;
            msg->data[3] = regCanRdl0r & 0xFF00'0000 >> R_DATA3;

            std::uint32_t regCanRdh0r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RDH0R);

            msg->data[4] = regCanRdh0r & 0x0000'00FF >> R_DATA4;
            msg->data[5] = regCanRdh0r & 0x0000'FF00 >> R_DATA5;
            msg->data[6] = regCanRdh0r & 0x00FF'0000 >> R_DATA6;
            msg->data[7] = regCanRdh0r & 0xFF00'0000 >> R_DATA7;

            value = true;
        }
        else
        {
            value = false;
        }
        std::uint32_t regCanRf1r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RF1R);
        if(regCanRf1r & 0x3 != 0)
        {
            std::uint32_t regCanRi1r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RI1R);
            msg->IDE                 = regCanRi1r & 0x4 >> R_IDE;
            msg->RTR                 = regCanRi1r & 0x2 >> R_RTR;
            if(msg->IDE == 1)
            {
                msg->extId = regCanRi1r & 0xFFFF'FFF8 >> R_EXID;
            }
            else
            {
                msg->stdId = regCanRi1r & 0xFFE0'0000 >> R_STID_EXID;
            }
            std::uint32_t regCanRdt1r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RDT1R);
            msg->filterMatchIndex     = regCanRdt1r & 0x0000'FF00 >> R_FMI;
            msg->timestamp            = regCanRdt1r & 0xFFFF'0000 >> R_TIME;
            msg->DLC                  = regCanRdt1r & 0x0000'000F >> R_DLC;

            std::uint32_t regCanRdl1r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RDL1R);

            msg->data[0] = regCanRdl1r & 0x0000'00FF >> R_DATA0;
            msg->data[1] = regCanRdl1r & 0x0000'FF00 >> R_DATA1;
            msg->data[2] = regCanRdl1r & 0x00FF'0000 >> R_DATA2;
            msg->data[3] = regCanRdl1r & 0xFF00'0000 >> R_DATA3;

            std::uint32_t regCanRdh1r = mwr::read_register<std::uint32_t>(baseAddress + CAN_RDH1R);

            msg->data[4] = regCanRdh1r & 0x0000'00FF >> R_DATA4;
            msg->data[5] = regCanRdh1r & 0x0000'FF00 >> R_DATA5;
            msg->data[6] = regCanRdh1r & 0x00FF'0000 >> R_DATA6;
            msg->data[7] = regCanRdh1r & 0xFF00'0000 >> R_DATA7;

            value = true;
        }
        else
        {
            value = false;
        }
        return value;
    }

    bool Can::getRxMessage(CanRxMsg* msg, RxFifo rf)
    {
        bool           valueStatus = true;
        std::uintptr_t canRfxr = rf == RxFifo_0 ? baseAddress + CAN_RF0R : baseAddress + CAN_RF1R;
        std::uint32_t  regCanRfxr = mwr::read_register<std::uint32_t>(canRfxr);

        if(regCanRfxr & 0x3 != 0)
        {
            std::uintptr_t canRixr =
                rf == RxFifo_0 ? baseAddress + CAN_RI0R : baseAddress + CAN_RI1R;
            std::uintptr_t canRdtxr =
                rf == RxFifo_0 ? baseAddress + CAN_RDT0R : baseAddress + CAN_RDT1R;
            std::uintptr_t canRdlxr =
                rf == RxFifo_0 ? baseAddress + CAN_RDL0R : baseAddress + CAN_RDL1R;
            std::uintptr_t canRdhxr =
                rf == RxFifo_0 ? baseAddress + CAN_RDH0R : baseAddress + CAN_RDH1R;

            std::uint32_t regCanRixr = mwr::read_register<std::uint32_t>(canRixr);
            msg->IDE                 = regCanRixr & 0x4 >> R_IDE;
            msg->RTR                 = regCanRixr & 0x2 >> R_RTR;
            msg->extId               = msg->IDE == 1 ? regCanRixr & 0xFFFF'FFF8 >> R_EXID :
                                                       regCanRixr & 0xFFE0'0000 >> R_STID_EXID;

            std::uint32_t regCanRdtxr = mwr::read_register<std::uint32_t>(canRdtxr);
            msg->filterMatchIndex     = regCanRdtxr & 0x0000'FF00 >> R_FMI;
            msg->timestamp            = regCanRdtxr & 0xFFFF'0000 >> R_TIME;
            msg->DLC                  = regCanRdtxr & 0x0000'000F >> R_DLC;

            std::uint32_t regCanRdlxr = mwr::read_register<std::uint32_t>(canRdlxr);
            msg->data[0]              = regCanRdlxr & 0x0000'00FF >> R_DATA0;
            msg->data[1]              = regCanRdlxr & 0x0000'FF00 >> R_DATA1;
            msg->data[2]              = regCanRdlxr & 0x00FF'0000 >> R_DATA2;
            msg->data[3]              = regCanRdlxr & 0xFF00'0000 >> R_DATA3;

            std::uint32_t regCanRdhxr = mwr::read_register<std::uint32_t>(canRdhxr);
            msg->data[4]              = regCanRdhxr & 0x0000'00FF >> R_DATA4;
            msg->data[5]              = regCanRdhxr & 0x0000'FF00 >> R_DATA5;
            msg->data[6]              = regCanRdhxr & 0x00FF'0000 >> R_DATA6;
            msg->data[7]              = regCanRdhxr & 0xFF00'0000 >> R_DATA7;
        }
        else
        {
            valueStatus = false;
        }
        return valueStatus;
    }

    std::uint8_t Can::getRxFifoFillLevel(RxFifo rf)
    {
        std::uint8_t fillLevel = 0x00;
        if(rf == RxFifo_0)
        {
            fillLevel = mwr::read_register<std::uint32_t>(baseAddress + CAN_RF0R) & 0x03;
        }
        else
        {
            fillLevel = mwr::read_register<std::uint32_t>(baseAddress + CAN_RF1R) & 0x03;
        }
        return fillLevel;
    }

    void Can::configureIrq(CAN_IER_REG flag, bool isActive)
    {
        if(isActive)
        {
            mwr::setBit(baseAddress + CAN_IER, flag);
        }
        else
        {
            mwr::resetBit(baseAddress + CAN_IER, flag);
        }
    }

    bool Can::readFlag(Flags f)
    {
        bool statusFlag = false;

        switch(f)
        {
            case F_TX_RQCP0:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, RQCP0);
                break;

            case F_TX_TXOK0:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TXOK0);
                break;

            case F_TX_ALST0:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, ALST0);
                break;

            case F_TX_TERR0:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TERR0);
                break;

            case F_TX_RQCP1:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, RQCP1);
                break;

            case F_TX_TXOK1:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TXOK1);
                break;

            case F_TX_ALST1:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, ALST1);
                break;

            case F_TX_TERR1:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TERR1);
                break;

            case F_TX_RQCP2:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, RQCP2);
                break;

            case F_TX_TXOK2:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TXOK2);
                break;

            case F_TX_ALST2:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, ALST2);
                break;

            case F_TX_TERR2:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TERR2);
                break;

            case F_TX_TME0:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME0);
                break;

            case F_TX_TME1:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME1);
                break;

            case F_TX_TME2:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, TME2);
                break;

            case F_TX_LOW0:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, LOW0);
                break;

            case F_TX_LOW1:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, LOW1);
                break;

            case F_TX_LOW2:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_TSR, LOW2);
                break;

            case F_RX_FF0:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_RF0R, FULL0);
                break;

            case F_RX_FOV0:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_RF0R, FOVR0);
                break;

            case F_RX_FF1:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_RF1R, FULL1);
                break;

            case F_RX_FOV1:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_RF1R, FOVR1);
                break;

            case F_M_INAK:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_MSR, INAK);
                break;

            case F_M_SLAK:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_MSR, SLAK);
                break;

            case F_M_ERRI:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_MSR, ERRI);
                break;

            case F_M_WKU:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_MSR, WKUI);
                break;

            case F_M_SLAKI:
                statusFlag = mwr::readBit<std::uint32_t>(baseAddress + CAN_MSR, SLAKI);
                break;

            default:
                break;
        }

        return statusFlag;
    }

    void Can::clearFlag(Flags f)
    {
        switch(f)
        {
            case F_TX_RQCP0:
                mwr::setBit(baseAddress + CAN_TSR, RQCP0);
                break;

            case F_TX_TXOK0:
                mwr::setBit(baseAddress + CAN_TSR, TXOK0);
                break;

            case F_TX_ALST0:
                mwr::setBit(baseAddress + CAN_TSR, ALST0);
                break;

            case F_TX_TERR0:
                mwr::setBit(baseAddress + CAN_TSR, TERR0);
                break;

            case F_TX_RQCP1:
                mwr::setBit(baseAddress + CAN_TSR, RQCP1);
                break;

            case F_TX_TXOK1:
                mwr::setBit(baseAddress + CAN_TSR, TXOK1);
                break;

            case F_TX_ALST1:
                mwr::setBit(baseAddress + CAN_TSR, ALST1);
                break;

            case F_TX_TERR1:
                mwr::setBit(baseAddress + CAN_TSR, TERR1);
                break;

            case F_TX_RQCP2:
                mwr::setBit(baseAddress + CAN_TSR, RQCP2);
                break;

            case F_TX_TXOK2:
                mwr::setBit(baseAddress + CAN_TSR, TXOK2);
                break;

            case F_TX_ALST2:
                mwr::setBit(baseAddress + CAN_TSR, ALST2);
                break;

            case F_TX_TERR2:
                mwr::setBit(baseAddress + CAN_TSR, TERR2);
                break;

            case F_RX_FF0:
                mwr::setBit(baseAddress + CAN_RF0R, FULL0);
                break;

            case F_RX_FOV0:
                mwr::setBit(baseAddress + CAN_RF0R, FOVR0);
                break;

            case F_RX_FF1:
                mwr::setBit(baseAddress + CAN_RF1R, FULL1);
                break;

            case F_RX_FOV1:
                mwr::setBit(baseAddress + CAN_RF1R, FOVR1);
                break;

            case F_M_ERRI:
                mwr::setBit(baseAddress + CAN_MSR, ERRI);
                break;

            case F_M_WKU:
                mwr::setBit(baseAddress + CAN_MSR, WKUI);
                break;

            case F_M_SLAKI:
                mwr::setBit(baseAddress + CAN_MSR, SLAKI);
                break;

            default:
                break;
        }
    }

    

}    // namespace drivers::can