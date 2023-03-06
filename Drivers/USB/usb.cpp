//
// Created by koteiko_vv on 21.02.2023.
//
#include "usb.h"

namespace drivers::usb {

    Usb::Usb(drivers::clock::ClockControl &clockControl1) : clockControl(clockControl1) {}

    void Usb::Init() noexcept {

        MspInit();

        libs::MWR::write_register(GAHBCFG, ~0x00000001); // Disable the Interrupts

        CoreInit();

        SetCurremtMode();

        DevInit();

        SetRxFiFo(0x80);

        SetTxFiFo(0, 0x40);

        SetTxFiFo(1, 0x80);
    }

    void Usb::CoreReset() noexcept {

        do {

        } while (!libs::MWR::readBit<std::uint32_t>(GRSTCTL,31));

        libs::MWR::setBit(GRSTCTL,0);

        do {

        } while (libs::MWR::readBit<std::uint32_t>(GRSTCTL,0));

    }

    void Usb::MspInit() {
        drivers::port::GPIO<drivers::port::PORT_A> gpio(clockControl);
        drivers::nvic::NVIC nvic(clockControl);

        gpio.SetPinMode(gpio.PIN_12,gpio.ALTERNATE_FUNCT);
        gpio.SetPinMode(gpio.PIN_11,gpio.ALTERNATE_FUNCT);
        gpio.SetPinSpeed(gpio.PIN_12,gpio.VERY_HIGH_SPEED);
        gpio.SetPinSpeed(gpio.PIN_11,gpio.VERY_HIGH_SPEED);
        gpio.SetAFPin(gpio.PIN_12,gpio.AF10);
        gpio.SetAFPin(gpio.PIN_11,gpio.AF10);

        clockControl.module_enable(drivers::clock::USB_FS_MODULE);
        nvic.NVIC_Enable(nvic.OTG_FS_IRQ);
    }

    void Usb::CoreInit() {

        libs::MWR::setBit(GUSBCFG, 6);

        CoreReset();

        libs::MWR::setBit(GCCFG,16); // Activate the USB Transceiver

    }

    void Usb::SetCurremtMode() {

        libs::MWR::write_register(GUSBCFG,~0x60000000);
        libs::MWR::setBit(GUSBCFG,30);

        while (libs::MWR::readBit<std::uint32_t>(GINTSTS,0))
        {
            clockControl.mDelay(1);
        }

    }

    void Usb::DevInit() {

        for (std::size_t i = 0U; i < 15U; i++)
        {
            libs::MWR::write_register(DIEPTXF + (i * 4) ,0);
        }

        libs::MWR::setBit(DCTL,1);
        libs::MWR::setBit(GCCFG,21);
        libs::MWR::write_register(GCCFG,~0x80000);
        libs::MWR::write_register(GCCFG,~0x40000);

        libs::MWR::setBit(DCFG,0);

        libs::MWR::write_register(DCFG,3);

        //-------------------Flush Tx FIFO--------------------

        while(!libs::MWR::readBit<std::uint32_t>(GRSTCTL,31))
        {}

        libs::MWR::write_register(GRSTCTL,32 | (2 << 6));

        while(libs::MWR::readBit<std::uint32_t>(GRSTCTL,5))
        {}

        //-------------------Flush Rx FIFO--------------------

        while(!libs::MWR::readBit<std::uint32_t>(GRSTCTL,31))
        {}

        libs::MWR::write_register(GRSTCTL, 0x10);

        while(libs::MWR::readBit<std::uint32_t>(GRSTCTL,2))
        {}

        /* Clear all pending Device Interrupts */
        libs::MWR::write_register(DIEPMSK, 0);
        libs::MWR::write_register(DOEPMSK, 0);
        libs::MWR::write_register(DAINTMSK,0);

        for (std::size_t i = 0U; i < 4; i++)
        {
            if (libs::MWR::readBit<std::uint32_t>(DIEPCTL,31))
            {
                if (i == 0U)
                {
                    libs::MWR::write_register(DIEPCTL,(1 << 27));
                }
                else
                {
                    libs::MWR::write_register(DIEPCTL,(1 << 30) | (1 << 27));
                }
            }
            else
            {
                libs::MWR::write_register(DIEPCTL + (i * 0x20),0);
            }
            libs::MWR::write_register(DIEPTSIZ + (i * 0x20),0);
            libs::MWR::write_register(DIEPINT + (i * 0x20),0xFB7FU);
        }

        for (std::size_t i = 0U; i < 4; i++)
        {
            if (libs::MWR::readBit<std::uint32_t>(DOEPCTL,31))
            {
                if (i == 0U)
                {
                    libs::MWR::write_register(DOEPCTL,(1 << 27));
                }
                else
                {
                    libs::MWR::write_register(DOEPCTL,(1 << 30) | (1 << 27));
                }
            }
            else
            {
                libs::MWR::write_register(DOEPCTL + (i * 0x20),0);
            }

            libs::MWR::write_register(DOEPCTL + (i * 0x20),0);
            libs::MWR::write_register(DOEPCTL + (i * 0x20),0xFB7FU);
        }

        libs::MWR::write_register(DIEPMSK,~(0x100));
        libs::MWR::write_register(GINTMSK, 0);           // Disable all interrupts
        libs::MWR::write_register(GINTSTS, 0xBFFFFFFFU); // Clear any pending interrupts

        libs::MWR::setBit(GINTMSK,1 << 4);
        libs::MWR::setBit(GINTMSK,(1 << 11) | (1 << 12) | (1 << 13) | (1 << 18) |
                                   (1 << 19) | (1 << 20) | (1 << 21) | (1 << 31));  //Enable interrupts matching to the Device mode ONLY
        libs::MWR::setBit(DCTL, 2);
    }

    void Usb::SetRxFiFo(std::uint16_t size) {
        libs::MWR::write_register(GRXFSIZ, size);
    }

    void Usb::SetTxFiFo(std::uint8_t fifo, std::uint16_t size) {

        std::size_t i;
        std::uint32_t Tx_offset = libs::MWR::read_register<std::uint32_t>(GRXFSIZ);

        if (fifo == 0U)
        {
            libs::MWR::write_register(DIEPTXF0_HNPTXFSIZ,(static_cast<std::uint32_t>(size << 16) | Tx_offset));
        }
        else
        {
            Tx_offset += (libs::MWR::read_register<std::uint32_t>(DIEPTXF0_HNPTXFSIZ)) >> 16;

            for (i = 0U; i < (fifo - 1U); i++)
            {
                Tx_offset += (libs::MWR::read_register<std::uint32_t>(DIEPTXF + ((fifo -1) * 0x04)) >> 16);
            }

            /* Multiply Tx_Size by 2 to get higher performance */
            libs::MWR::write_register((DIEPTXF + ((fifo -1) * 0x04)),(static_cast<std::uint32_t>(size << 16) | Tx_offset));
        }

    }

    void Usb::RegisterClass(std::uint8_t& add ) {

    }
}