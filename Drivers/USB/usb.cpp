//
// Created by koteiko_vv on 21.02.2023.
//
#include "usb.h"

namespace drivers::usb {

    Usb::Usb(drivers::ClockControl &clockControl1) : clockControl(clockControl1) {}

    void Usb::Init() noexcept {

        MspInit();

        libs::MWR::clearBit(GAHBCFG, 0x00000001); // Disable the Interrupts

        CoreInit();

        SetCurremtMode();

        DevInit();

        DevDisconnect();

        SetRxFiFo(0x80);

        SetTxFiFo(0, 0x40);

        SetTxFiFo(1, 0x80);
    }



    void Usb::CoreReset() noexcept {

        do {

        } while (!libs::MWR::readBit<std::uint32_t>(GRSTCTL,31));

        libs::MWR::enableNumberBit(GRSTCTL,0);

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

        clockControl.module_enable(ClockControl::USB_FS_MODULE);
        nvic.NVIC_Enable(nvic.OTG_FS_IRQ);
    }

    void Usb::CoreInit() {

        libs::MWR::enableNumberBit(GUSBCFG, 6);

        CoreReset();

        libs::MWR::enableNumberBit(GCCFG,16); // Activate the USB Transceiver

    }

    void Usb::SetCurremtMode() {

        libs::MWR::clearBit(GUSBCFG,0x60000000);
        libs::MWR::enableNumberBit(GUSBCFG,30);

        do
        {
            clockControl.mDelay(1);
        }while (libs::MWR::readBit<std::uint32_t>(GINTSTS,0));

    }

    void Usb::DevInit() {

        for (std::size_t i = 0U; i < 15U; i++)
        {
            libs::MWR::write_register(DIEPTXF + (i * 4) ,0);
        }

        libs::MWR::enableNumberBit(DCTL,1);
        libs::MWR::enableNumberBit(GCCFG,21);
        libs::MWR::clearBit(GCCFG,0x80000);
        libs::MWR::clearBit(GCCFG,0x40000);
        libs::MWR::setBit(DCFG,0);

        libs::MWR::setBit(DCFG,3);

        //-------------------Flush Tx FIFO--------------------


        libs::MWR::write_register(GRSTCTL,(32 | (16 << 6)));

        while(libs::MWR::readBit<std::uint32_t>(GRSTCTL,5))
        {}

        //-------------------Flush Rx FIFO--------------------

        libs::MWR::write_register(GRSTCTL, 0x10);

        while(libs::MWR::readBit<std::uint32_t>(GRSTCTL,4))
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

        libs::MWR::clearBit(DIEPMSK,0x0100);
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

    void Usb::DevDisconnect()

    {
        libs::MWR::clearBit(PCGCCTL,0x0100);
        libs::MWR::setBit(DCTL, 0x02);
    }

    void Usb::RegisterClass(std::uint8_t& add ) {

    }

    void Usb::DevConnect() {
        libs::MWR::clearBit(PCGCCTL, 3);
        libs::MWR::clearBit(DCTL, 2);
    }

    void Usb::PCD_Start() {

        //--------------LOCK-------------------------


        //-------------------------------------------

        libs::MWR::setBit(GAHBCFG,1);
        Usb::DevConnect();

    }

    void Usb::StartUSB()
    {
        PCD_Start();
    }

    //-------------------------------------------------------------------------------------------

    void Usb::MX_USB_DEVICE_Init(void) {
        /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */

        /* USER CODE END USB_DEVICE_Init_PreTreatment */

        /* Init Device Library, add supported class and start the library. */
        USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

//        USBD_RegisterClass(&hUsbDeviceFS, &USBD_CUSTOM_HID);

//        USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceFS,
//                                              &USBD_CustomHID_fops_FS);

//        USBD_Start(&hUsbDeviceFS);

        /* USER CODE BEGIN USB_DEVICE_Init_PostTreatment */

        /* USER CODE END USB_DEVICE_Init_PostTreatment */
    }

    Usb::USBD_StatusTypeDef Usb::USBD_Init(USBD_HandleTypeDef *pdev,
                                 USBD_DescriptorsTypeDef *pdesc, uint8_t id)
    {
        USBD_StatusTypeDef ret;

        /* Check whether the USB Host handle is valid */
        if (pdev == NULL)
        {
#if (USBD_DEBUG_LEVEL > 1U)
            USBD_ErrLog("Invalid Device handle");
#endif
            return USBD_FAIL;
        }

        /* Unlink previous class resources */
        pdev->pClass = NULL;
        pdev->pUserData = NULL;
        pdev->pConfDesc = NULL;

        /* Assign USBD Descriptors */
        if (pdesc != NULL)
        {
            pdev->pDesc = pdesc;
        }

        /* Set Device initial State */
        pdev->dev_state = USBD_STATE_DEFAULT;
        pdev->id = id;

        /* Initialize low level driver */
        ret = USBD_LL_Init(pdev);

        return ret;
    }

    Usb::USBD_StatusTypeDef Usb::USBD_LL_Init(USBD_HandleTypeDef *pdev)
    {
        /* Init USB Ip. */
        if (pdev->id == DEVICE_FS) {
            /* Link the driver to the stack. */
            hpcd_USB_OTG_FS.pData = pdev;
            pdev->pData = &hpcd_USB_OTG_FS;

            hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
            hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
            hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
            hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
            hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
            hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
            hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
            hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
            hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
            hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
            HAL_PCD_Init(&hpcd_USB_OTG_FS);

            HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS, 0x80);
            HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 0x40);
            HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 0x80);
        }
        return USBD_OK;
    }

    Usb::HAL_StatusTypeDef Usb::HAL_PCD_Init(PCD_HandleTypeDef *hpcd)
    {
        USB_OTG_GlobalTypeDef *USBx;
        uint8_t i;

        /* Check the PCD handle allocation */
        if (hpcd == NULL)
        {
            return HAL_ERROR;
        }

        /* Check the parameters */
        assert_param(IS_PCD_ALL_INSTANCE(hpcd->Instance));

        USBx = hpcd->Instance;

        if (hpcd->State == HAL_PCD_STATE_RESET)
        {
            /* Allocate lock resource and initialize it */
            hpcd->Lock = HAL_UNLOCKED;

            /* Init the low level hardware : GPIO, CLOCK, NVIC... */
//            HAL_PCD_MspInit(hpcd);
//            MspInit();
        }
        MspInit();

        hpcd->State = HAL_PCD_STATE_BUSY;

        /* Disable DMA mode for FS instance */
        if ((USBx->CID & (0x1U << 8)) == 0U)
        {
            hpcd->Init.dma_enable = 0U;
        }

        /* Disable the Interrupts */
        __HAL_PCD_DISABLE(hpcd);

        /*Init the Core (common init.) */
        if (USB_CoreInit(hpcd->Instance, hpcd->Init) != HAL_OK)
        {
            hpcd->State = HAL_PCD_STATE_ERROR;
            return HAL_ERROR;
        }

        /* Force Device Mode*/
        (void)USB_SetCurrentMode(hpcd->Instance, USB_DEVICE_MODE);

        /* Init endpoints structures */
        for (i = 0U; i < hpcd->Init.dev_endpoints; i++)
        {
            /* Init ep structure */
            hpcd->IN_ep[i].is_in = 1U;
            hpcd->IN_ep[i].num = i;
            hpcd->IN_ep[i].tx_fifo_num = i;
            /* Control until ep is activated */
            hpcd->IN_ep[i].type = EP_TYPE_CTRL;
            hpcd->IN_ep[i].maxpacket = 0U;
            hpcd->IN_ep[i].xfer_buff = 0U;
            hpcd->IN_ep[i].xfer_len = 0U;
        }

        for (i = 0U; i < hpcd->Init.dev_endpoints; i++)
        {
            hpcd->OUT_ep[i].is_in = 0U;
            hpcd->OUT_ep[i].num = i;
            /* Control until ep is activated */
            hpcd->OUT_ep[i].type = EP_TYPE_CTRL;
            hpcd->OUT_ep[i].maxpacket = 0U;
            hpcd->OUT_ep[i].xfer_buff = 0U;
            hpcd->OUT_ep[i].xfer_len = 0U;
        }

        /* Init Device */
        if (USB_DevInit(hpcd->Instance, hpcd->Init) != HAL_OK)
        {
            hpcd->State = HAL_PCD_STATE_ERROR;
            return HAL_ERROR;
        }

        hpcd->USB_Address = 0U;
        hpcd->State = HAL_PCD_STATE_READY;
#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)
        /* Activate LPM */
  if (hpcd->Init.lpm_enable == 1U)
  {
    (void)HAL_PCDEx_ActivateLPM(hpcd);
  }
#endif /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */
        (void)USB_DevDisconnect(hpcd->Instance);

        return HAL_OK;
    }

    void Usb::HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
    {
        /* Prevent unused argument(s) compilation warning */
        UNUSED(hpcd);

        /* NOTE : This function should not be modified, when the callback is needed,
                  the HAL_PCD_MspInit could be implemented in the user file
         */
    }

    Usb::HAL_StatusTypeDef Usb::USB_EnableGlobalInt(USB_OTG_GlobalTypeDef *USBx)
    {
        USBx->GAHBCFG |= USB_OTG_GAHBCFG_GINT;
        return HAL_OK;
    }

    Usb::HAL_StatusTypeDef Usb::USB_DisableGlobalInt(USB_OTG_GlobalTypeDef *USBx)
    {
        USBx->GAHBCFG &= ~USB_OTG_GAHBCFG_GINT;
        return HAL_OK;
    }

    Usb::HAL_StatusTypeDef Usb::USB_CoreInit(USB_OTG_GlobalTypeDef *USBx, USB_OTG_CfgTypeDef cfg)
    {
        HAL_StatusTypeDef ret;

        if (cfg.phy_itface == USB_OTG_ULPI_PHY)
        {
            USBx->GCCFG &= ~(USB_OTG_GCCFG_PWRDWN);

            /* Init The ULPI Interface */
            USBx->GUSBCFG &= ~(USB_OTG_GUSBCFG_TSDPS | USB_OTG_GUSBCFG_ULPIFSLS | USB_OTG_GUSBCFG_PHYSEL);

            /* Select vbus source */
            USBx->GUSBCFG &= ~(USB_OTG_GUSBCFG_ULPIEVBUSD | USB_OTG_GUSBCFG_ULPIEVBUSI);
            if (cfg.use_external_vbus == 1U)
            {
                USBx->GUSBCFG |= USB_OTG_GUSBCFG_ULPIEVBUSD;
            }

            /* Reset after a PHY select */
            ret = USB_CoreReset(USBx);
        }
        else /* FS interface (embedded Phy) */
        {
            /* Select FS Embedded PHY */
            USBx->GUSBCFG |= USB_OTG_GUSBCFG_PHYSEL;

            /* Reset after a PHY select */
            ret = USB_CoreReset(USBx);

            if (cfg.battery_charging_enable == 0U)
            {
                /* Activate the USB Transceiver */
                USBx->GCCFG |= USB_OTG_GCCFG_PWRDWN;
            }
            else
            {
                /* Deactivate the USB Transceiver */
                USBx->GCCFG &= ~(USB_OTG_GCCFG_PWRDWN);
            }
        }

        if (cfg.dma_enable == 1U)
        {
            USBx->GAHBCFG |= USB_OTG_GAHBCFG_HBSTLEN_2;
            USBx->GAHBCFG |= USB_OTG_GAHBCFG_DMAEN;
        }

        return ret;
    }

    Usb::HAL_StatusTypeDef Usb::USB_CoreReset(USB_OTG_GlobalTypeDef *USBx)
    {
        __IO uint32_t count = 0U;

        /* Wait for AHB master IDLE state. */
        do
        {
            if (++count > 200000U)
            {
                return HAL_TIMEOUT;
            }
        } while ((USBx->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0U);

        /* Core Soft Reset */
        count = 0U;
        USBx->GRSTCTL |= USB_OTG_GRSTCTL_CSRST;

        do
        {
            if (++count > 200000U)
            {
                return HAL_TIMEOUT;
            }
        } while ((USBx->GRSTCTL & USB_OTG_GRSTCTL_CSRST) == USB_OTG_GRSTCTL_CSRST);

        return HAL_OK;
    }

    Usb::HAL_StatusTypeDef Usb::USB_SetCurrentMode(USB_OTG_GlobalTypeDef *USBx, USB_OTG_ModeTypeDef mode)
    {
        uint32_t ms = 0U;

        USBx->GUSBCFG &= ~(USB_OTG_GUSBCFG_FHMOD | USB_OTG_GUSBCFG_FDMOD);

        if (mode == USB_HOST_MODE)
        {
            USBx->GUSBCFG |= USB_OTG_GUSBCFG_FHMOD;

            do
            {
                ms++;
            } while ((USB_GetMode(USBx) != (uint32_t)USB_HOST_MODE) && (ms < 50U));
        }
        else if (mode == USB_DEVICE_MODE)
        {
            USBx->GUSBCFG |= USB_OTG_GUSBCFG_FDMOD;

            do
            {
                ms++;
            } while ((USB_GetMode(USBx) != (uint32_t)USB_DEVICE_MODE) && (ms < 50U));
        }
        else
        {
            return HAL_ERROR;
        }

        if (ms == 50U)
        {
            return HAL_ERROR;
        }

        return HAL_OK;
    }

    uint32_t Usb::USB_GetMode(USB_OTG_GlobalTypeDef *USBx)
    {
        return ((USBx->GINTSTS) & 0x1U);
    }

    Usb::HAL_StatusTypeDef Usb::USB_DevInit(USB_OTG_GlobalTypeDef *USBx, USB_OTG_CfgTypeDef cfg)
    {
        HAL_StatusTypeDef ret = HAL_OK;
        uint32_t USBx_BASE = (uint32_t)USBx;
        uint32_t i;

        for (i = 0U; i < 15U; i++)
        {
            USBx->DIEPTXF[i] = 0U;
        }

#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)
        /* VBUS Sensing setup */
  if (cfg.vbus_sensing_enable == 0U)
  {
    USBx_DEVICE->DCTL |= USB_OTG_DCTL_SDIS;

    /* Deactivate VBUS Sensing B */
    USBx->GCCFG &= ~USB_OTG_GCCFG_VBDEN;

    /* B-peripheral session valid override enable */
    USBx->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN;
    USBx->GOTGCTL |= USB_OTG_GOTGCTL_BVALOVAL;
  }
  else
  {
    /* Enable HW VBUS sensing */
    USBx->GCCFG |= USB_OTG_GCCFG_VBDEN;
  }
#else
        /* VBUS Sensing setup */
        if (cfg.vbus_sensing_enable == 0U)
        {
            /*
             * Disable HW VBUS sensing. VBUS is internally considered to be always
             * at VBUS-Valid level (5V).
             */
            USBx_DEVICE->DCTL |= USB_OTG_DCTL_SDIS;
            USBx->GCCFG |= USB_OTG_GCCFG_NOVBUSSENS;
            USBx->GCCFG &= ~USB_OTG_GCCFG_VBUSBSEN;
            USBx->GCCFG &= ~USB_OTG_GCCFG_VBUSASEN;
        }
        else
        {
            /* Enable HW VBUS sensing */
            USBx->GCCFG &= ~USB_OTG_GCCFG_NOVBUSSENS;
            USBx->GCCFG |= USB_OTG_GCCFG_VBUSBSEN;
        }
#endif /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */

        /* Restart the Phy Clock */
        USBx_PCGCCTL = 0U;

        /* Device mode configuration */
        USBx_DEVICE->DCFG |= DCFG_FRAME_INTERVAL_80;

        if (cfg.phy_itface == USB_OTG_ULPI_PHY)
        {
            if (cfg.speed == USBD_HS_SPEED)
            {
                /* Set Core speed to High speed mode */
                (void)USB_SetDevSpeed(USBx, USB_OTG_SPEED_HIGH);
            }
            else
            {
                /* Set Core speed to Full speed mode */
                (void)USB_SetDevSpeed(USBx, USB_OTG_SPEED_HIGH_IN_FULL);
            }
        }
        else
        {
            /* Set Core speed to Full speed mode */
            (void)USB_SetDevSpeed(USBx, USB_OTG_SPEED_FULL);
        }

        /* Flush the FIFOs */
        if (USB_FlushTxFifo(USBx, 0x10U) != HAL_OK) /* all Tx FIFOs */
        {
            ret = HAL_ERROR;
        }

        if (USB_FlushRxFifo(USBx) != HAL_OK)
        {
            ret = HAL_ERROR;
        }

        /* Clear all pending Device Interrupts */
        USBx_DEVICE->DIEPMSK = 0U;
        USBx_DEVICE->DOEPMSK = 0U;
        USBx_DEVICE->DAINTMSK = 0U;

        for (i = 0U; i < cfg.dev_endpoints; i++)
        {
            if ((USBx_INEP(i)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) == USB_OTG_DIEPCTL_EPENA)
            {
                if (i == 0U)
                {
                    USBx_INEP(i)->DIEPCTL = USB_OTG_DIEPCTL_SNAK;
                }
                else
                {
                    USBx_INEP(i)->DIEPCTL = USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK;
                }
            }
            else
            {
                USBx_INEP(i)->DIEPCTL = 0U;
            }

            USBx_INEP(i)->DIEPTSIZ = 0U;
            USBx_INEP(i)->DIEPINT  = 0xFB7FU;
        }

        for (i = 0U; i < cfg.dev_endpoints; i++)
        {
            if ((USBx_OUTEP(i)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) == USB_OTG_DOEPCTL_EPENA)
            {
                if (i == 0U)
                {
                    USBx_OUTEP(i)->DOEPCTL = USB_OTG_DOEPCTL_SNAK;
                }
                else
                {
                    USBx_OUTEP(i)->DOEPCTL = USB_OTG_DOEPCTL_EPDIS | USB_OTG_DOEPCTL_SNAK;
                }
            }
            else
            {
                USBx_OUTEP(i)->DOEPCTL = 0U;
            }

            USBx_OUTEP(i)->DOEPTSIZ = 0U;
            USBx_OUTEP(i)->DOEPINT  = 0xFB7FU;
        }

        USBx_DEVICE->DIEPMSK &= ~(USB_OTG_DIEPMSK_TXFURM);

        /* Disable all interrupts. */
        USBx->GINTMSK = 0U;

        /* Clear any pending interrupts */
        USBx->GINTSTS = 0xBFFFFFFFU;

        /* Enable the common interrupts */
        if (cfg.dma_enable == 0U)
        {
            USBx->GINTMSK |= USB_OTG_GINTMSK_RXFLVLM;
        }

        /* Enable interrupts matching to the Device mode ONLY */
        USBx->GINTMSK |= USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_USBRST |
                         USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_IEPINT |
                         USB_OTG_GINTMSK_OEPINT   | USB_OTG_GINTMSK_IISOIXFRM |
                         USB_OTG_GINTMSK_PXFRM_IISOOXFRM | USB_OTG_GINTMSK_WUIM;

        if (cfg.Sof_enable != 0U)
        {
            USBx->GINTMSK |= USB_OTG_GINTMSK_SOFM;
        }

        if (cfg.vbus_sensing_enable == 1U)
        {
            USBx->GINTMSK |= (USB_OTG_GINTMSK_SRQIM | USB_OTG_GINTMSK_OTGINT);
        }

        return ret;
    }

    Usb::HAL_StatusTypeDef Usb::USB_SetDevSpeed(USB_OTG_GlobalTypeDef *USBx, uint8_t speed)
    {
        uint32_t USBx_BASE = (uint32_t)USBx;

        USBx_DEVICE->DCFG |= speed;
        return HAL_OK;
    }

    Usb::HAL_StatusTypeDef Usb::USB_FlushRxFifo(USB_OTG_GlobalTypeDef *USBx)
    {
        __IO uint32_t count = 0U;

        USBx->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;

        do
        {
            if (++count > 200000U)
            {
                return HAL_TIMEOUT;
            }
        } while ((USBx->GRSTCTL & USB_OTG_GRSTCTL_RXFFLSH) == USB_OTG_GRSTCTL_RXFFLSH);

        return HAL_OK;
    }

    Usb::HAL_StatusTypeDef Usb::USB_FlushTxFifo(USB_OTG_GlobalTypeDef *USBx, uint32_t num)
    {
        __IO uint32_t count = 0U;

        USBx->GRSTCTL = (USB_OTG_GRSTCTL_TXFFLSH | (num << 6));

        do
        {
            if (++count > 200000U)
            {
                return HAL_TIMEOUT;
            }
        } while ((USBx->GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH) == USB_OTG_GRSTCTL_TXFFLSH);

        return HAL_OK;
    }

    Usb::HAL_StatusTypeDef  Usb::USB_DevDisconnect(USB_OTG_GlobalTypeDef *USBx)
    {
        uint32_t USBx_BASE = (uint32_t)USBx;

        /* In case phy is stopped, ensure to ungate and restore the phy CLK */
        USBx_PCGCCTL &= ~(USB_OTG_PCGCCTL_STOPCLK | USB_OTG_PCGCCTL_GATECLK);

        USBx_DEVICE->DCTL |= USB_OTG_DCTL_SDIS;

        return HAL_OK;
    }

    Usb::HAL_StatusTypeDef Usb::HAL_PCDEx_SetRxFiFo(PCD_HandleTypeDef *hpcd, uint16_t size)
    {
        hpcd->Instance->GRXFSIZ = size;

        return HAL_OK;
    }

    Usb::HAL_StatusTypeDef Usb::HAL_PCDEx_SetTxFiFo(PCD_HandleTypeDef *hpcd, uint8_t fifo, uint16_t size)
    {
        uint8_t i;
        uint32_t Tx_Offset;

        /*  TXn min size = 16 words. (n  : Transmit FIFO index)
            When a TxFIFO is not used, the Configuration should be as follows:
                case 1 :  n > m    and Txn is not used    (n,m  : Transmit FIFO indexes)
               --> Txm can use the space allocated for Txn.
               case2  :  n < m    and Txn is not used    (n,m  : Transmit FIFO indexes)
               --> Txn should be configured with the minimum space of 16 words
           The FIFO is used optimally when used TxFIFOs are allocated in the top
               of the FIFO.Ex: use EP1 and EP2 as IN instead of EP1 and EP3 as IN ones.
           When DMA is used 3n * FIFO locations should be reserved for internal DMA registers */

        Tx_Offset = hpcd->Instance->GRXFSIZ;

        if (fifo == 0U)
        {
            hpcd->Instance->DIEPTXF0_HNPTXFSIZ = ((uint32_t)size << 16) | Tx_Offset;
        }
        else
        {
            Tx_Offset += (hpcd->Instance->DIEPTXF0_HNPTXFSIZ) >> 16;
            for (i = 0U; i < (fifo - 1U); i++)
            {
                Tx_Offset += (hpcd->Instance->DIEPTXF[i] >> 16);
            }

            /* Multiply Tx_Size by 2 to get higher performance */
            hpcd->Instance->DIEPTXF[fifo - 1U] = ((uint32_t)size << 16) | Tx_Offset;
        }

        return HAL_OK;
    }

    Usb::USBD_StatusTypeDef Usb::USBD_RegisterClass(USBD_HandleTypeDef *pdev, USBD_ClassTypeDef *pclass)
    {
        uint16_t len = 0U;

        if (pclass == NULL)
        {
            return USBD_FAIL;
        }

        /* link the class to the USB Device handle */
        pdev->pClass = pclass;

        /* Get Device Configuration Descriptor */

        if (pdev->pClass->GetFSConfigDescriptor != NULL)
        {
            pdev->pConfDesc = (void *)pdev->pClass->GetFSConfigDescriptor(&len);
        }

        return USBD_OK;
    }

    uint8_t Usb::USBD_CUSTOM_HID_RegisterInterface(USBD_HandleTypeDef *pdev,
                                              USBD_CUSTOM_HID_ItfTypeDef *fops)
    {
        if (fops == NULL)
        {
            return (uint8_t)USBD_FAIL;
        }

        pdev->pUserData = fops;

        return (uint8_t)USBD_OK;
    }

    Usb::USBD_StatusTypeDef Usb::USBD_Start(USBD_HandleTypeDef *pdev)
    {
        /* Start the low level driver  */
        return USBD_LL_Start(pdev);
    }

    Usb::USBD_StatusTypeDef Usb::USBD_LL_Start(USBD_HandleTypeDef *pdev)
    {
        HAL_StatusTypeDef hal_status = HAL_OK;
        USBD_StatusTypeDef usb_status = USBD_OK;

        hal_status = HAL_PCD_Start(static_cast<PCD_HandleTypeDef *>(pdev->pData));

        usb_status =  USBD_Get_USB_Status(hal_status);

        return usb_status;
    }

    Usb::HAL_StatusTypeDef Usb::HAL_PCD_Start(PCD_HandleTypeDef *hpcd)
    {
        USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;

        __HAL_LOCK(hpcd);

        if ((hpcd->Init.battery_charging_enable == 1U) &&
            (hpcd->Init.phy_itface != USB_OTG_ULPI_PHY))
        {
            /* Enable USB Transceiver */
            USBx->GCCFG |= USB_OTG_GCCFG_PWRDWN;
        }

        __HAL_PCD_ENABLE(hpcd);
        (void)USB_DevConnect(hpcd->Instance);
        __HAL_UNLOCK(hpcd);

        return HAL_OK;
    }

    Usb::HAL_StatusTypeDef  Usb::USB_DevConnect(USB_OTG_GlobalTypeDef *USBx)
    {
        uint32_t USBx_BASE = (uint32_t)USBx;

        /* In case phy is stopped, ensure to ungate and restore the phy CLK */
        USBx_PCGCCTL &= ~(USB_OTG_PCGCCTL_STOPCLK | USB_OTG_PCGCCTL_GATECLK);

        USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_SDIS;

        return HAL_OK;
    }

    Usb::USBD_StatusTypeDef Usb::USBD_Get_USB_Status(HAL_StatusTypeDef hal_status)
    {
        USBD_StatusTypeDef usb_status = USBD_OK;

        switch (hal_status)
        {
            case HAL_OK :
                usb_status = USBD_OK;
                break;
            case HAL_ERROR :
                usb_status = USBD_FAIL;
                break;
            case HAL_BUSY :
                usb_status = USBD_BUSY;
                break;
            case HAL_TIMEOUT :
                usb_status = USBD_FAIL;
                break;
            default :
                usb_status = USBD_FAIL;
                break;
        }
        return usb_status;
    }


//-------------------------------------------------------------------------------------------------

    void Usb::HAL_PCD_IRQHandler(PCD_HandleTypeDef *hpcd)
    {
        USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;
        uint32_t USBx_BASE = (uint32_t)USBx;
        USB_OTG_EPTypeDef *ep;
        uint32_t i;
        uint32_t ep_intr;
        uint32_t epint;
        uint32_t epnum;
        uint32_t fifoemptymsk;
        uint32_t temp;

        /* ensure that we are in device mode */
        if (USB_GetMode(hpcd->Instance) == USB_OTG_MODE_DEVICE)
        {
            /* avoid spurious interrupt */
            if (__HAL_PCD_IS_INVALID_INTERRUPT(hpcd))
            {
                return;
            }

            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_MMIS))
            {
                /* incorrect mode, acknowledge the interrupt */
                __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_MMIS);
            }

            /* Handle RxQLevel Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_RXFLVL))
            {
                USB_MASK_INTERRUPT(hpcd->Instance, USB_OTG_GINTSTS_RXFLVL);

                temp = USBx->GRXSTSP;

                ep = &hpcd->OUT_ep[temp & USB_OTG_GRXSTSP_EPNUM];

                if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  STS_DATA_UPDT)
                {
                    if ((temp & USB_OTG_GRXSTSP_BCNT) != 0U)
                    {
                        (void)USB_ReadPacket(USBx, ep->xfer_buff,
                                             (uint16_t)((temp & USB_OTG_GRXSTSP_BCNT) >> 4));

                        ep->xfer_buff += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
                        ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
                    }
                }
                else if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  STS_SETUP_UPDT)
                {
                    (void)USB_ReadPacket(USBx, (uint8_t *)hpcd->Setup, 8U);
                    ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
                }
                else
                {
                    /* ... */
                }
                USB_UNMASK_INTERRUPT(hpcd->Instance, USB_OTG_GINTSTS_RXFLVL);
            }

            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OEPINT))
            {
                epnum = 0U;

                /* Read in the device interrupt bits */
                ep_intr = USB_ReadDevAllOutEpInterrupt(hpcd->Instance);

                while (ep_intr != 0U)
                {
                    if ((ep_intr & 0x1U) != 0U)
                    {
                        epint = USB_ReadDevOutEPInterrupt(hpcd->Instance, (uint8_t)epnum);

                        if ((epint & USB_OTG_DOEPINT_XFRC) == USB_OTG_DOEPINT_XFRC)
                        {
                            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_XFRC);
                            (void)PCD_EP_OutXfrComplete_int(hpcd, epnum);
                        }

                        if ((epint & USB_OTG_DOEPINT_STUP) == USB_OTG_DOEPINT_STUP)
                        {
                            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_STUP);
                            /* Class B setup phase done for previous decoded setup */
                            (void)PCD_EP_OutSetupPacket_int(hpcd, epnum);
                        }

                        if ((epint & USB_OTG_DOEPINT_OTEPDIS) == USB_OTG_DOEPINT_OTEPDIS)
                        {
                            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPDIS);
                        }

                        /* Clear Status Phase Received interrupt */
                        if ((epint & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR)
                        {
                            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPSPR);
                        }

                        /* Clear OUT NAK interrupt */
                        if ((epint & USB_OTG_DOEPINT_NAK) == USB_OTG_DOEPINT_NAK)
                        {
                            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_NAK);
                        }
                    }
                    epnum++;
                    ep_intr >>= 1U;
                }
            }

            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IEPINT))
            {
                /* Read in the device interrupt bits */
                ep_intr = USB_ReadDevAllInEpInterrupt(hpcd->Instance);

                epnum = 0U;

                while (ep_intr != 0U)
                {
                    if ((ep_intr & 0x1U) != 0U) /* In ITR */
                    {
                        epint = USB_ReadDevInEPInterrupt(hpcd->Instance, (uint8_t)epnum);

                        if ((epint & USB_OTG_DIEPINT_XFRC) == USB_OTG_DIEPINT_XFRC)
                        {
                            fifoemptymsk = (uint32_t)(0x1UL << (epnum & EP_ADDR_MSK));
                            USBx_DEVICE->DIEPEMPMSK &= ~fifoemptymsk;

                            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_XFRC);

                            if (hpcd->Init.dma_enable == 1U)
                            {
                                hpcd->IN_ep[epnum].xfer_buff += hpcd->IN_ep[epnum].maxpacket;

                                /* this is ZLP, so prepare EP0 for next setup */
                                if ((epnum == 0U) && (hpcd->IN_ep[epnum].xfer_len == 0U))
                                {
                                    /* prepare to rx more setup packets */
                                    (void)USB_EP0_OutStart(hpcd->Instance, 1U, (uint8_t *)hpcd->Setup);
                                }
                            }

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                            hpcd->DataInStageCallback(hpcd, (uint8_t)epnum);
#else
                            HAL_PCD_DataInStageCallback(hpcd, (uint8_t)epnum);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
                        }
                        if ((epint & USB_OTG_DIEPINT_TOC) == USB_OTG_DIEPINT_TOC)
                        {
                            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_TOC);
                        }
                        if ((epint & USB_OTG_DIEPINT_ITTXFE) == USB_OTG_DIEPINT_ITTXFE)
                        {
                            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_ITTXFE);
                        }
                        if ((epint & USB_OTG_DIEPINT_INEPNE) == USB_OTG_DIEPINT_INEPNE)
                        {
                            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_INEPNE);
                        }
                        if ((epint & USB_OTG_DIEPINT_EPDISD) == USB_OTG_DIEPINT_EPDISD)
                        {
                            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_EPDISD);
                        }
                        if ((epint & USB_OTG_DIEPINT_TXFE) == USB_OTG_DIEPINT_TXFE)
                        {
                            (void)PCD_WriteEmptyTxFifo(hpcd, epnum);
                        }
                    }
                    epnum++;
                    ep_intr >>= 1U;
                }
            }

            /* Handle Resume Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT))
            {
                /* Clear the Remote Wake-up Signaling */
                USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;

                if (hpcd->LPM_State == LPM_L1)
                {
                    hpcd->LPM_State = LPM_L0;

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                    hpcd->LPMCallback(hpcd, PCD_LPM_L0_ACTIVE);
#else
                    HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L0_ACTIVE);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
                }
                else
                {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                    hpcd->ResumeCallback(hpcd);
#else
                    HAL_PCD_ResumeCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
                }

                __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT);
            }

            /* Handle Suspend Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP))
            {
                if ((USBx_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS) == USB_OTG_DSTS_SUSPSTS)
                {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                    hpcd->SuspendCallback(hpcd);
#else
                    HAL_PCD_SuspendCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
                }
                __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP);
            }
#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)
            /* Handle LPM Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_LPMINT))
    {
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_LPMINT);

      if (hpcd->LPM_State == LPM_L0)
      {
        hpcd->LPM_State = LPM_L1;
        hpcd->BESL = (hpcd->Instance->GLPMCFG & USB_OTG_GLPMCFG_BESL) >> 2U;

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        hpcd->LPMCallback(hpcd, PCD_LPM_L1_ACTIVE);
#else
        HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L1_ACTIVE);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }
      else
      {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        hpcd->SuspendCallback(hpcd);
#else
        HAL_PCD_SuspendCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }
    }
#endif /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */
            /* Handle Reset Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_USBRST))
            {
                USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;
                (void)USB_FlushTxFifo(hpcd->Instance, 0x10U);

                for (i = 0U; i < hpcd->Init.dev_endpoints; i++)
                {
                    USBx_INEP(i)->DIEPINT = 0xFB7FU;
                    USBx_INEP(i)->DIEPCTL &= ~USB_OTG_DIEPCTL_STALL;
                    USBx_INEP(i)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;
                    USBx_OUTEP(i)->DOEPINT = 0xFB7FU;
                    USBx_OUTEP(i)->DOEPCTL &= ~USB_OTG_DOEPCTL_STALL;
                    USBx_OUTEP(i)->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;
                }
                USBx_DEVICE->DAINTMSK |= 0x10001U;

                if (hpcd->Init.use_dedicated_ep1 != 0U)
                {
                    USBx_DEVICE->DOUTEP1MSK |= USB_OTG_DOEPMSK_STUPM |
                                               USB_OTG_DOEPMSK_XFRCM |
                                               USB_OTG_DOEPMSK_EPDM;

                    USBx_DEVICE->DINEP1MSK |= USB_OTG_DIEPMSK_TOM |
                                              USB_OTG_DIEPMSK_XFRCM |
                                              USB_OTG_DIEPMSK_EPDM;
                }
                else
                {
                    USBx_DEVICE->DOEPMSK |= USB_OTG_DOEPMSK_STUPM |
                                            USB_OTG_DOEPMSK_XFRCM |
                                            USB_OTG_DOEPMSK_EPDM |
                                            USB_OTG_DOEPMSK_OTEPSPRM |
                                            USB_OTG_DOEPMSK_NAKM;

                    USBx_DEVICE->DIEPMSK |= USB_OTG_DIEPMSK_TOM |
                                            USB_OTG_DIEPMSK_XFRCM |
                                            USB_OTG_DIEPMSK_EPDM;
                }

                /* Set Default Address to 0 */
                USBx_DEVICE->DCFG &= ~USB_OTG_DCFG_DAD;

                /* setup EP0 to receive SETUP packets */
                (void)USB_EP0_OutStart(hpcd->Instance, (uint8_t)hpcd->Init.dma_enable,
                                       (uint8_t *)hpcd->Setup);

                __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_USBRST);
            }

            /* Handle Enumeration done Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_ENUMDNE))
            {
                (void)USB_ActivateSetup(hpcd->Instance);
                hpcd->Init.speed = USB_GetDevSpeed(hpcd->Instance);

                /* Set USB Turnaround time */
                (void)USB_SetTurnaroundTime(hpcd->Instance,
                                            HAL_RCC_GetHCLKFreq(),
                                            (uint8_t)hpcd->Init.speed);

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                hpcd->ResetCallback(hpcd);
#else
                HAL_PCD_ResetCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

                __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_ENUMDNE);
            }

            /* Handle SOF Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SOF))
            {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                hpcd->SOFCallback(hpcd);
#else
                HAL_PCD_SOFCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

                __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SOF);
            }

            /* Handle Incomplete ISO IN Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR))
            {
                /* Keep application checking the corresponding Iso IN endpoint
                causing the incomplete Interrupt */
                epnum = 0U;

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                hpcd->ISOINIncompleteCallback(hpcd, (uint8_t)epnum);
#else
                HAL_PCD_ISOINIncompleteCallback(hpcd, (uint8_t)epnum);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

                __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR);
            }

            /* Handle Incomplete ISO OUT Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT))
            {
                /* Keep application checking the corresponding Iso OUT endpoint
                causing the incomplete Interrupt */
                epnum = 0U;

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                hpcd->ISOOUTIncompleteCallback(hpcd, (uint8_t)epnum);
#else
                HAL_PCD_ISOOUTIncompleteCallback(hpcd, (uint8_t)epnum);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

                __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);
            }

            /* Handle Connection event Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT))
            {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                hpcd->ConnectCallback(hpcd);
#else
                HAL_PCD_ConnectCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

                __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT);
            }

            /* Handle Disconnection event Interrupt */
            if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OTGINT))
            {
                temp = hpcd->Instance->GOTGINT;

                if ((temp & USB_OTG_GOTGINT_SEDET) == USB_OTG_GOTGINT_SEDET)
                {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
                    hpcd->DisconnectCallback(hpcd);
#else
                    HAL_PCD_DisconnectCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
                }
                hpcd->Instance->GOTGINT |= temp;
            }
        }
    }


    uint32_t  Usb::USB_ReadInterrupts(USB_OTG_GlobalTypeDef *USBx)
    {
        uint32_t tmpreg;

        tmpreg = USBx->GINTSTS;
        tmpreg &= USBx->GINTMSK;

        return tmpreg;
    }










}