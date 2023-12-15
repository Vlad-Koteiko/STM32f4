//
// Created by koteiko_vv on 21.02.2023.
//
#include "usb.hpp"

namespace drivers::usb
{

    void Usb::init() noexcept
    {
        clockControl.EnablePeripherals(drivers::clock::constants::USB_OTG_FS_MODULE);
        gpioInit();
        nvicEnable();
        coreInit();
        currentMode();
        devInit();
        devDisconnect();
    }

    void Usb::gpioInit() const noexcept
    {
        drivers::port::Gpio portA(clockControl, drivers::port::ADDRESSES_PORT::PORTA);
        portA.SetPinMode(drivers::port::PIN_11, drivers::port::ALTERNATE_FUNCT);
        portA.SetPinSpeed(drivers::port::PIN_11, drivers::port::VERY_HIGH_SPEED);
        portA.SetAFPin(drivers::port::PIN_11, drivers::port::ALTERNATE_FUNCTION::AF10);

        portA.SetPinMode(drivers::port::PIN_12, drivers::port::ALTERNATE_FUNCT);
        portA.SetPinSpeed(drivers::port::PIN_12, drivers::port::VERY_HIGH_SPEED);
        portA.SetAFPin(drivers::port::PIN_12, drivers::port::ALTERNATE_FUNCTION::AF10);
    }

    void Usb::nvicEnable() const noexcept
    {
        drivers::nvic::NVIC::NVIC_EnableIRQ(drivers::nvic::USB_OTG_FS);
    }

    void Usb::coreInit() noexcept
    {
        setBit(RegisterGlobal::GUSBCFG,
               6);    // [bit assignment] Full Speed serial transceiver select
        coreReset();
        setBit(RegisterGlobal::GCCFG, 16);    // [bit assignment] Used to activate the transceiver
    }

    void Usb::coreReset() noexcept
    {
        do
        {
        } while(readBit(GRSTCTL, 31) == 0);    // [bit assignment] Indicates that the AHB master

        setBit(GRSTCTL, 0);    // [bit assignment] Core soft reset

        do    // Core soft reset
        {
        } while(readBit(GRSTCTL, 0) == 1);
    }

    void Usb::currentMode() noexcept
    {
        resetBit(RegisterGlobal::GUSBCFG, 29);    // [bit assignment]  Force host mode
        resetBit(RegisterGlobal::GUSBCFG, 30);    // [bit assignment]  Force device mode
        setBit(RegisterGlobal::GUSBCFG, 30);      // [bit assignment]  Force host mode
        do
        {
        } while(readBit(RegisterGlobal::GINTSTS, 0) !=
                0);    // Current mode of operation = Device mode
    }

    void Usb::devInit() noexcept
    {
        for(std::size_t i = 0; i < 15; ++i)
        {
            writeRegister(RegisterGlobal::DIEPTXF + (i * 0x04),
                          0);    // device IN endpoint transmit FIFO size register
        }
        setBit(RegisterDevice::DCTL,
               1);    // the core generates a device disconnect event to the USB host
        setBit(RegisterGlobal::GCCFG, 21);      // VBUS sensing not available by hardware
        resetBit(RegisterGlobal::GCCFG, 19);    //  VBUS sensing “B” disabled
        resetBit(RegisterGlobal::GCCFG, 18);    //  VBUS sensing “A” disabled
                                                //        writeRegister(RegisterDevice::DCFG, 0);
        libs::MWR::modifySetRegister(RegisterDevice::DCFG,
                                     0x03);    //  Full speed (USB 1.1 transceiver clock is 48 MHz)
        flushTxFIFO(0x10);                     // Flush the TxFIFOs
        flushRxFifo();                         // Flush the RxFIFOs
        interruptsDevice();
    }

    void Usb::flushTxFIFO(std::uint8_t numberDevice) noexcept
    {
        do
        {
        } while(readBit(GRSTCTL, 31) == 0);    // [bit assignment] Indicates that the AHB master
        writeRegister(RegisterGlobal::GRSTCTL, (numberDevice << 6));
        setBit(RegisterGlobal::GRSTCTL, 5);    // TxFIFO flush
        do
        {
        } while(readBit(GRSTCTL, 5) == 1);    // [bit assignment] // TxFIFO flush
    }

    void Usb::flushRxFifo() noexcept
    {
        do
        {
        } while(readBit(GRSTCTL, 31) == 0);    // [bit assignment] Indicates that the AHB master

        setBit(RegisterGlobal::GRSTCTL, 4);    // RxFIFO flush
        do
        {
        } while(readBit(GRSTCTL, 4) == 1);    // [bit assignment] // RxFIFO flush
    }

    void Usb::interruptsDevice() noexcept
    {
        // Clear all pending Device Interrupts
        writeRegister(RegisterDevice::DIEPMSK, 0);
        writeRegister(RegisterDevice::DOEPMSK, 0);
        writeRegister(RegisterDevice::DAINTMSK, 0);

        writeRegister(RegisterGlobal::GINTMSK, 0);              // Disable all interrupts
        writeRegister(RegisterGlobal::GINTSTS, 0xBFFFFFFFU);    // Clear any pending interrupts

        setBit(RegisterGlobal::GINTMSK, 4);     // Receive FIFO non-empty mask
        setBit(RegisterGlobal::GINTMSK, 11);    // USB suspend mask
        setBit(RegisterGlobal::GINTMSK, 12);    // USB reset mask
        setBit(RegisterGlobal::GINTMSK, 13);    // Enumeration done mask
        setBit(RegisterGlobal::GINTMSK, 18);    // IN endpoints interrupt mask
        setBit(RegisterGlobal::GINTMSK, 19);    // OUT endpoints interrupt mask
        setBit(RegisterGlobal::GINTMSK, 20);    // Incomplete isochronous IN transfer mask
        setBit(RegisterGlobal::GINTMSK, 21);    // Incomplete periodic transfer mask
        setBit(RegisterGlobal::GINTMSK, 31);    // Resume/remote wakeup detected interrupt mask
    }

    void Usb::devDisconnect() noexcept
    {
        setBit(RegisterDevice::DCTL, 1);    // Soft disconnect
    }

    void Usb::registerClass() noexcept
    {}

    void Usb::usbCustomHidInterface() noexcept
    {}

    void Usb::start() noexcept
    {
        setBit(RegisterGlobal::GAHBCFG, 0);    // global interrupt mask
        resetBit(RegisterDevice::DCTL, 1);     // device is reconnected
    }

    const std::uint8_t* Usb::getPtrDeviceDesc() noexcept
    {
        return deviceDesc.data();
    }

    std::uint32_t Usb::readFIFO(std::uint8_t count) noexcept
    {
        return readRegister(RegisterGlobal::FIFO_BASE + (count * 4));
    }

    void Usb::readPacket(std::uint8_t* dest, std::uint16_t len)
    {
        std::uint8_t* pDest           = dest;
        std::uint32_t count32b        = static_cast<std::uint32_t>(len) >> 2U;
        std::uint16_t remaining_bytes = len % 4U;
        std::uint32_t i;
        std::uint32_t pData;

        for(i = 0; i < count32b; i++)
        {
            pData = readRegister(RegisterGlobal::FIFO_BASE + (i * sizeof(std::uint32_t)));
            std::copy_n(reinterpret_cast<std::uint8_t*>(&pData), sizeof(pData), pDest);
            pDest = pDest + sizeof(std::uint32_t);
        }

        if(remaining_bytes != 0U)
        {
            std::uint8_t i = 0U;
            pData = libs::MWR::read_register<std::uint32_t>(RegisterGlobal::FIFO_BASE + (i * 4));

            do
            {
                *pDest = (uint8_t)(pData >> (8U * (uint8_t)(i)));
                i++;
                pDest++;
                remaining_bytes--;
            } while(remaining_bytes != 0U);
        }
    }

    void Usb::writePacket(const std::uint8_t* dest, std::uint16_t len)
    {
        const std::uint8_t* pSrc  = dest;
        std::uint32_t       count = (static_cast<std::uint32_t>(len) + 3) / 4;
        std::uint32_t       i;

        for(int i = 0; i < count; i++)
        {
            writeRegister(RegisterGlobal::FIFO_BASE, *reinterpret_cast<const std::uint32_t*>(pSrc));
            pSrc++;
            pSrc++;
            pSrc++;
            pSrc++;
        }

        //        std::uint32_t testData = 0x11223344;
        //        libs::MWR::write_register(RegisterGlobal::FIFO_BASE, testData);
    }

    bool Usb::getFlagInterruptMask(std::uint8_t numberBit)
    {
        std::uint32_t tmpreg;
        tmpreg = readRegister(drivers::usb::RegisterGlobal::GINTSTS);
        tmpreg &= readRegister(drivers::usb::RegisterGlobal::GINTMSK);
        return (tmpreg & (1 << numberBit));
    }

    void Usb::transmit() noexcept
    {
        startEP0();
    }

    void Usb::startEP0() noexcept
    {
        libs::MWR::write_register(drivers::usb::RegisterDevice::DIEPTSIZ, 0);
        libs::MWR::setBit(drivers::usb::RegisterDevice::DIEPTSIZ, 19);
        libs::MWR::modifySetRegister(drivers::usb::RegisterDevice::DIEPTSIZ, 18);

        libs::MWR::setBit(drivers::usb::RegisterDevice::DIEPCTL, 26);
        libs::MWR::setBit(drivers::usb::RegisterDevice::DIEPCTL, 31);
        libs::MWR::setBit(drivers::usb::RegisterDevice::DIEPEMPMSK, 0);
    }

    void Usb::startEP() noexcept
    {}

    void Usb::resetCallback() noexcept
    {}

}    // namespace drivers::usb

void OTG_FS_IRQHandler()
{
    std::uint32_t                 mask_GRXSTSP_PKTSTS   = 0xF << 17;
    std::uint32_t                 mask_OTG_GRXSTSP_BCNT = 0x7FF << 4;
    std::uint8_t                  STS_DATA_UPDT         = 2;
    std::uint8_t                  STS_SETUP_UPDT        = 6;
    std::array<std::uint8_t, 20>  dataSetup;
    std::array<std::uint8_t, 256> dataBuffer;
    std::uint32_t                 xferCount = 0;
    std::uint32_t                 xferBuff  = 0;

    if(drivers::usb::Usb::getFlagInterruptMask(4))
    {
        //        drivers::usb::Usb::resetBit(drivers::usb::RegisterGlobal::GINTMSK, 4);
        //
        std::uint32_t regVal =
            drivers::usb::Usb::readRegister(drivers::usb::RegisterGlobal::GRXSTSP);

        if(((regVal & mask_GRXSTSP_PKTSTS) >> 17) == STS_DATA_UPDT)
        {
            //            if((regVal & mask_OTG_GRXSTSP_BCNT) != 0)
            //            {
            //                //                drivers::usb::Usb::readPacket(
            //                //                    dataBuffer.data(),
            //                //                    static_cast<std::uint16_t>(regVal &
            //                mask_OTG_GRXSTSP_BCNT) >>
            //                //                    4);
            //                //                xferBuff += ((regVal & mask_OTG_GRXSTSP_BCNT) >> 4);
            //                //                xferCount += ((regVal & mask_OTG_GRXSTSP_BCNT) >>
            //                4);
            //            }
        }
        else if(((regVal & mask_GRXSTSP_PKTSTS) >> 17) == STS_SETUP_UPDT)
        {
            dataSetup.fill(0);
            drivers::usb::Usb::readPacket(dataSetup.data(), 8);
            //            xferCount += ((regVal & mask_OTG_GRXSTSP_BCNT) >> 4);
            //        }
            //        for(std::uint8_t i = 0; i < 8; i++)
            //        {
        }

        drivers::usb::Usb::setBit(drivers::usb::RegisterGlobal::GINTMSK, 4);
    }

    if(drivers::usb::Usb::getFlagInterruptMask(19))
    {
        drivers::usb::Usb::transmit();
    }

    if(drivers::usb::Usb::getFlagInterruptMask(18))
    {
        if((libs::MWR::read_register<std::uint32_t>(drivers::usb::RegisterDevice::DTXFSTS)) >= 18)
        {
            drivers::usb::Usb::writePacket(drivers::usb::Usb::getPtrDeviceDesc(), 18);
        }
    }

    if(drivers::usb::Usb::getFlagInterruptMask(12))
    {
        libs::MWR::setBit(drivers::usb::RegisterDevice::DOEPMSK, 3);
        libs::MWR::setBit(drivers::usb::RegisterDevice::DOEPMSK, 0);
        libs::MWR::setBit(drivers::usb::RegisterDevice::DOEPMSK, 1);
        libs::MWR::setBit(drivers::usb::RegisterDevice::DOEPMSK, 5);

        libs::MWR::setBit(drivers::usb::RegisterDevice::DIEPMSK, 3);
        libs::MWR::setBit(drivers::usb::RegisterDevice::DIEPMSK, 0);
        libs::MWR::setBit(drivers::usb::RegisterDevice::DIEPMSK, 1);

        //        shared::Data::getCout()->operator<<(" DOEPMSK = ")
        //            << libs::MWR::read_register<std::uint32_t>(0x50000814) << libs::Cout::ENDL;
    }

    if(drivers::usb::Usb::getFlagInterruptMask(13))
    {
        //        libs::MWR::setBit(drivers::usb::RegisterDevice::DAINTMSK, 16);
        //        libs::MWR::setBit(drivers::usb::RegisterDevice::DAINTMSK, 0);
    }
}