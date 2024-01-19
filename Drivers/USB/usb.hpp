//
// Created by koteiko_vv on 21.02.2023.
//

#ifndef STM32F4_USB_HPP
#define STM32F4_USB_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

#include "MWR.hpp"
#include "clockControl.hpp"
#include "cout.hpp"
#include "gpio.hpp"
#include "nvic.h"
#include "shared.hpp"
#include "usart.hpp"

namespace drivers::usb
{
    static constexpr std::uintptr_t baseAddress = 0x50'000'000;

    // clang-format off
    enum RegisterGlobal : std::uintptr_t
    {
        GOTGCTL            = baseAddress,         /*!< USB_OTG Control and Status Register          000h */
        GOTGINT            = baseAddress + 0x04,  /*!< USB_OTG Interrupt Register                   004h */
        GAHBCFG            = baseAddress + 0x08,  /*!< Core AHB Configuration Register              008h */
        GUSBCFG            = baseAddress + 0x0C,  /*!< Core USB Configuration Register              00Ch */
        GRSTCTL            = baseAddress + 0x10,  /*!< Core Reset Register                          010h */
        GINTSTS            = baseAddress + 0x14,  /*!< Core Interrupt Register                      014h */
        GINTMSK            = baseAddress + 0x18,  /*!< Core Interrupt Mask Register                 018h */
        GRXSTSR            = baseAddress + 0x1C,  /*!< Receive Sts Q Read Register                  01Ch */
        GRXSTSP            = baseAddress + 0x20,  /*!< Receive Sts Q Read & POP Register            020h */
        GRXFSIZ            = baseAddress + 0x24,  /*!< Receive FIFO Size Register                   024h */
        DIEPTXF0_HNPTXFSIZ = baseAddress + 0x28,  /*!< EP0 / Non Periodic Tx FIFO Size Register     028h */
        HNPTXSTS           = baseAddress + 0x2C,  /*!< Non Periodic Tx FIFO/Queue Sts reg           02Ch */
        GCCFG              = baseAddress + 0x38,  /*!< General Purpose IO Register                  038h */
        CID                = baseAddress + 0x3C,  /*!< User ID Register                             03Ch */
        HPTXFSIZ           = baseAddress + 0x100, /*!< Host Periodic Tx FIFO Size Reg               100h */
        DIEPTXF            = baseAddress + 0x104, /*!< dev Periodic Transmit FIFO                        */
        FIFO_BASE          = baseAddress + 0x1000

    };

    enum RegisterDevice : std::uintptr_t
    {
        DCFG       = baseAddress + 0x800, /*!< dev Configuration Register   800h */
        DCTL       = baseAddress + 0x804, /*!< dev Control Register         804h */
        DSTS       = baseAddress + 0x808, /*!< dev Status Register (RO)     808h */
        DIEPMSK    = baseAddress + 0x810, /*!< dev IN Endpoint Mask         810h */
        DOEPMSK    = baseAddress + 0x814, /*!< dev OUT Endpoint Mask        814h */
        DAINT      = baseAddress + 0x818, /*!< dev All Endpoints Itr Reg    818h */
        DAINTMSK   = baseAddress + 0x81C, /*!< dev All Endpoints Itr Mask   81Ch */
        DVBUSDIS   = baseAddress + 0x828, /*!< dev VBUS discharge Register  828h */
        DVBUSPULSE = baseAddress + 0x82C, /*!< dev VBUS Pulse Register      82Ch */
        DTHRCTL    = baseAddress + 0x830, /*!< dev threshold                830h */
        DIEPEMPMSK = baseAddress + 0x834, /*!< dev empty msk                834h */
        DEACHINT   = baseAddress + 0x838, /*!< dedicated EP interrupt       838h */
        DEACHMSK   = baseAddress + 0x83C, /*!< dedicated EP msk             83Ch */
        DINEP1MSK  = baseAddress + 0x844, /*!< dedicated EP mask            844h */
        DOUTEP1MSK = baseAddress + 0x884, /*!< dedicated EP msk                  */
        DIEPINT    = baseAddress + 0x908, /*!< device endpoint-x interrupt       */
        DIEPTSIZ   = baseAddress + 0x910,
        DIEPCTL    = baseAddress + 0x900,
        DOEPCTL    = baseAddress + 0xB00,
        D0EPTSIZ   = baseAddress + 0xB10,
        DOEPINT    = baseAddress + 0xB08,
        DTXFSTS    = baseAddress + 0x918
    };

    struct SetupRequest
    {
        std::uint8_t bmRequest;
        std::uint8_t bRequest;
        std::uint16_t wValue;
        std::uint16_t wIndex;
        std::uint16_t wLength;
    };

    struct GlobalConfig
    {
        SetupRequest setupRequest;
        const std::uint8_t *pointerWrite;
        std::size_t sizeWrite;
        std::size_t count;
    };

    static bool sink = true;
    static drivers::usb::GlobalConfig globalConfig;

    // clang-format on

    class Usb
    {
        static constexpr std::array<std::uint8_t, 18> deviceDesc = { 0x12, 0x01, 0x00, 0x02, 0x00,
                                                                     0x00, 0x00, 0x40, 0x83, 0x04,
                                                                     0x50, 0x57, 0x00, 0x02, 0x01,
                                                                     0x02, 0x03, 0x01 };

        static constexpr std::array<std::uint8_t, 41> costomHidDescriptor = {
            0x09, 0x02, 41,   0,    1,    1,    0,    0xC0, 0x09, 0x04, 0,   0,    0x02, 0x03,
            0,    0,    0,    0x09, 0x21, 0x11, 0x01, 0x00, 0x01, 0x22, 108, 0x00, 0x07, 0x05,
            0x81, 0x03, 0x02, 0,    0x05, 0x07, 0x05, 0x01, 0x03, 0x02, 0,   0x5
        };

        const drivers::clock::ClockControl& clockControl;

        void                 gpioInit() const noexcept;
        void                 nvicEnable() const noexcept;
        void                 coreInit() noexcept;
        void                 coreReset() noexcept;
        void                 currentMode() noexcept;
        void                 devInit() noexcept;
        void                 flushTxFIFO(std::uint8_t numberDevice) noexcept;
        void                 flushRxFifo() noexcept;
        void                 interruptsDevice() noexcept;
        void                 devDisconnect() noexcept;
        static std::uint32_t readFIFO(std::uint8_t count) noexcept;
        static void          startEP0(std::size_t size, bool flag) noexcept;
        static void          startEP() noexcept;
        static void          getDescriptor(drivers::usb::GlobalConfig& globalConfig) noexcept;
        static void          setAddress(drivers::usb::GlobalConfig& globalConfig) noexcept;
        static void          setConfig(drivers::usb::GlobalConfig& globalConfig) noexcept;

    public:
        Usb(const drivers::clock::ClockControl& clockControlInit,
            drivers::usart::Usart&              usartInst) :
            clockControl(clockControlInit)
        {}

        static void setBit(std::uintptr_t address, std::uint8_t numberBit) noexcept
        {
            libs::MWR::setBit(address, numberBit);
        }

        static void resetBit(std::uintptr_t address, std::uint8_t numberBit) noexcept
        {
            libs::MWR::resetBit(address, numberBit);
        }

        static bool readBit(std::uintptr_t address, std::uint8_t numberBit) noexcept
        {
            return libs::MWR::readBit<std::uint32_t>(address, numberBit);
        }

        static std::uint32_t readRegister(std::uintptr_t address) noexcept
        {
            return libs::MWR::read_register<std::uint32_t>(address);
        }

        static void writeRegister(std::uintptr_t address, std::uint32_t data) noexcept
        {
            libs::MWR::write_register(address, data);
        }

        void                       init() noexcept;
        void                       registerClass() noexcept;
        void                       usbCustomHidInterface() noexcept;
        void                       start() noexcept;
        static const std::uint8_t* getPtrDeviceDesc() noexcept;
        static const std::uint8_t* getPtrCostomHidDescriptor() noexcept;
        static std::size_t         getSizeCostomHidDescriptor() noexcept;
        static void                readPacket(std::uint8_t* dest, std::uint16_t len);
        static void                readSetupConfig(drivers::usb::GlobalConfig& globalConfig);
        static void                writePacket(const std::uint8_t* dest, std::uint16_t len);
        static bool                getFlagInterruptMask(std::uint8_t numberBit);
        static void                transmit(std::size_t size, bool flag) noexcept;
        static void                resetCallback() noexcept;
        static void                setupStage(drivers::usb::GlobalConfig& globalConfig) noexcept;
        static void                stdDevReg(drivers::usb::GlobalConfig& globalConfig) noexcept;
        static void                stdItfReg(drivers::usb::GlobalConfig& globalConfig) noexcept;
        static std::uint32_t       USB_ReadDevInEPInterrupt() noexcept;
        static std::uint32_t       USB_ReadDevOutEPInterrupt() noexcept;
    };

    // clang-format off

    enum RegisterUsbInep : std::uintptr_t
    {
//        DIEPCTL       = baseAddress + 0x900,
    };
}    // namespace drivers::usb
#endif    // STM32F4_USB_HPP
