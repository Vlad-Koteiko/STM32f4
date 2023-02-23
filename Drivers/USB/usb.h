//
// Created by koteiko_vv on 21.02.2023.
//

#ifndef STM32F4_USB_H
#define STM32F4_USB_H

#include "MWR.hpp"
#include "gpio.h"
#include "clockControl.hpp"
#include "nvic.h"

namespace drivers::usb {

    class Usb
    {
        static constexpr std::uintptr_t baseAddress = 0x50000000;
        ClockControl &clockControl;

        enum Register : std::uintptr_t
        {
          GOTGCTL            = baseAddress + 0x04, /*!< USB_OTG Control and Status Register          000h */
          GOTGINT            = baseAddress + 0x04, /*!< USB_OTG Interrupt Register                   004h */
          GAHBCFG            = baseAddress + 0x08, /*!< Core AHB Configuration Register              008h */
          GUSBCFG            = baseAddress + 0x0C, /*!< Core USB Configuration Register              00Ch */
          GRSTCTL            = baseAddress + 0x10, /*!< Core Reset Register                          010h */
          GINTSTS            = baseAddress + 0x14, /*!< Core Interrupt Register                      014h */
          GINTMSK            = baseAddress + 0x18, /*!< Core Interrupt Mask Register                 018h */
          GRXSTSR            = baseAddress + 0x1C, /*!< Receive Sts Q Read Register                  01Ch */
          GRXSTSP            = baseAddress + 0x20, /*!< Receive Sts Q Read & POP Register            020h */
          GRXFSIZ            = baseAddress + 0x24, /*!< Receive FIFO Size Register                   024h */
          DIEPTXF0_HNPTXFSIZ = baseAddress + 0x28, /*!< EP0 / Non Periodic Tx FIFO Size Register     028h */
          HNPTXSTS           = baseAddress + 0x2C, /*!< Non Periodic Tx FIFO/Queue Sts reg           02Ch */
          GCCFG              = baseAddress + 0x38, /*!< General Purpose IO Register                  038h */
          CID                = baseAddress + 0x3C, /*!< User ID Register                             03Ch */
          HPTXFSIZ           = baseAddress + 0x100,/*!< Host Periodic Tx FIFO Size Reg               100h */
          DIEPTXF            = baseAddress + 0x104,/*!< dev Periodic Transmit FIFO                        */

          DCFG               = baseAddress + 0x800, /*!< dev Configuration Register   800h */
          DCTL               = baseAddress + 0x804, /*!< dev Control Register         804h */
          DSTS               = baseAddress + 0x808, /*!< dev Status Register (RO)     808h */
          DIEPMSK            = baseAddress + 0x810, /*!< dev IN Endpoint Mask         810h */
          DOEPMSK            = baseAddress + 0x814, /*!< dev OUT Endpoint Mask        814h */
          DAINT              = baseAddress + 0x818, /*!< dev All Endpoints Itr Reg    818h */
          DAINTMSK           = baseAddress + 0x81C, /*!< dev All Endpoints Itr Mask   81Ch */
          DVBUSDIS           = baseAddress + 0x828, /*!< dev VBUS discharge Register  828h */
          DVBUSPULSE         = baseAddress + 0x82C, /*!< dev VBUS Pulse Register      82Ch */
          DTHRCTL            = baseAddress + 0x830, /*!< dev threshold                830h */
          DIEPEMPMSK         = baseAddress + 0x834, /*!< dev empty msk                834h */
          DEACHINT           = baseAddress + 0x838, /*!< dedicated EP interrupt       838h */
          DEACHMSK           = baseAddress + 0x83C, /*!< dedicated EP msk             83Ch */
          DINEP1MSK          = baseAddress + 0x844, /*!< dedicated EP mask            844h */
          DOUTEP1MSK         = baseAddress + 0x848, /*!< dedicated EP msk                  */

          DOEPCTL           = baseAddress + 0xB00,  /*!< dev OUT Endpoint Control Reg           B00h + (ep_num * 20h) + 00h */
          DOEPINT           = baseAddress + 0xB08,  /*!< dev OUT Endpoint Itr Reg               B00h + (ep_num * 20h) + 08h */
          DOEPTSIZ          = baseAddress + 0xB10,  /*!< dev OUT Endpoint Txfer Size            B00h + (ep_num * 20h) + 10h */
          DOEPDMA           = baseAddress + 0xB14,  /*!< dev OUT Endpoint DMA Address           B00h + (ep_num * 20h) + 14h */

          DIEPCTL            = baseAddress + 0x900, /*!< dev IN Endpoint Control Reg 900h + (ep_num * 20h) + 00h */
          DIEPINT            = baseAddress + 0x908, /*!< dev IN Endpoint Itr Reg     900h + (ep_num * 20h) + 08h */
          DIEPTSIZ           = baseAddress + 0x910, /*!< IN Endpoint Txfer Size      900h + (ep_num * 20h) + 10h */
          DIEPDMA            = baseAddress + 0x914, /*!< IN Endpoint DMA Address Reg 900h + (ep_num * 20h) + 14h */
          DTXFSTS            = baseAddress + 0x918, /*!< IN Endpoint Tx FIFO Status Reg 900h + (ep_num * 20h) + 18h */

        };

        void MspInit();

    public:
        Usb(ClockControl &clockControl1);

        void Init() noexcept;

        void CoreReset() noexcept;

    };
}

#endif //STM32F4_USB_H
