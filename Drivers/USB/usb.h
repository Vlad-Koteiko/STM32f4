//
// Created by koteiko_vv on 21.02.2023.
//

#ifndef STM32F4_USB_H
#define STM32F4_USB_H

#include "MWR.hpp"
#include "gpio.h"
#include "clockControl.hpp"
#include "nvic.h"
#include <cstdint>
#include <cstddef>

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

            PCGCCTL            = baseAddress + 0xE00, // OTG_FS power and clock gating control register

        };

        //-------------------------------------------------------------------------------------------

        #define     __O     volatile             /*!< Defines 'write only' permissions */
        #define     __IO    volatile             /*!< Defines 'read / write' permissions */
        #define USBD_STATE_DEFAULT                              0x01U
        #define DEVICE_FS 		0
        #define DEVICE_HS 		1


        typedef enum
        {
            USBD_OK = 0U,
            USBD_BUSY,
            USBD_EMEM,
            USBD_FAIL,
        } USBD_StatusTypeDef;

        typedef enum
        {
            USBD_SPEED_HIGH  = 0U,
            USBD_SPEED_FULL  = 1U,
            USBD_SPEED_LOW   = 2U,
        } USBD_SpeedTypeDef;

        typedef struct
        {
            uint32_t status;
            uint32_t total_length;
            uint32_t rem_length;
            uint32_t maxpacket;
            uint16_t is_used;
            uint16_t bInterval;
        } USBD_EndpointTypeDef;

        typedef  struct  usb_setup_req
        {
            uint8_t   bmRequest;
            uint8_t   bRequest;
            uint16_t  wValue;
            uint16_t  wIndex;
            uint16_t  wLength;
        } USBD_SetupReqTypedef;

        /* USB Device descriptors structure */
        typedef struct
        {
            uint8_t *(*GetDeviceDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
            uint8_t *(*GetLangIDStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
            uint8_t *(*GetManufacturerStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
            uint8_t *(*GetProductStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
            uint8_t *(*GetSerialStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
            uint8_t *(*GetConfigurationStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
            uint8_t *(*GetInterfaceStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
        } USBD_DescriptorsTypeDef;

        typedef struct _Device_cb
        {
            uint8_t (*Init)(struct _USBD_HandleTypeDef *pdev, uint8_t cfgidx);
            uint8_t (*DeInit)(struct _USBD_HandleTypeDef *pdev, uint8_t cfgidx);
            /* Control Endpoints*/
            uint8_t (*Setup)(struct _USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef  *req);
            uint8_t (*EP0_TxSent)(struct _USBD_HandleTypeDef *pdev);
            uint8_t (*EP0_RxReady)(struct _USBD_HandleTypeDef *pdev);
            /* Class Specific Endpoints*/
            uint8_t (*DataIn)(struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
            uint8_t (*DataOut)(struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
            uint8_t (*SOF)(struct _USBD_HandleTypeDef *pdev);
            uint8_t (*IsoINIncomplete)(struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
            uint8_t (*IsoOUTIncomplete)(struct _USBD_HandleTypeDef *pdev, uint8_t epnum);

            uint8_t  *(*GetHSConfigDescriptor)(uint16_t *length);
            uint8_t  *(*GetFSConfigDescriptor)(uint16_t *length);
            uint8_t  *(*GetOtherSpeedConfigDescriptor)(uint16_t *length);
            uint8_t  *(*GetDeviceQualifierDescriptor)(uint16_t *length);
        } USBD_ClassTypeDef;


        typedef struct _USBD_HandleTypeDef
        {
            uint8_t                 id;
            uint32_t                dev_config;
            uint32_t                dev_default_config;
            uint32_t                dev_config_status;
            USBD_SpeedTypeDef       dev_speed;
            USBD_EndpointTypeDef    ep_in[16];
            USBD_EndpointTypeDef    ep_out[16];
            __IO uint32_t           ep0_state;
            uint32_t                ep0_data_len;
            __IO uint8_t            dev_state;
            __IO uint8_t            dev_old_state;
            uint8_t                 dev_address;
            uint8_t                 dev_connection_status;
            uint8_t                 dev_test_mode;
            uint32_t                dev_remote_wakeup;
            uint8_t                 ConfIdx;

            USBD_SetupReqTypedef    request;
            USBD_DescriptorsTypeDef *pDesc;
            USBD_ClassTypeDef       *pClass;
            void                    *pClassData;
            void                    *pUserData;
            void                    *pData;
            void                    *pBosDesc;
            void                    *pConfDesc;
        } USBD_HandleTypeDef;

        typedef struct
        {
            __IO uint32_t GOTGCTL;              /*!< USB_OTG Control and Status Register          000h */
            __IO uint32_t GOTGINT;              /*!< USB_OTG Interrupt Register                   004h */
            __IO uint32_t GAHBCFG;              /*!< Core AHB Configuration Register              008h */
            __IO uint32_t GUSBCFG;              /*!< Core USB Configuration Register              00Ch */
            __IO uint32_t GRSTCTL;              /*!< Core Reset Register                          010h */
            __IO uint32_t GINTSTS;              /*!< Core Interrupt Register                      014h */
            __IO uint32_t GINTMSK;              /*!< Core Interrupt Mask Register                 018h */
            __IO uint32_t GRXSTSR;              /*!< Receive Sts Q Read Register                  01Ch */
            __IO uint32_t GRXSTSP;              /*!< Receive Sts Q Read & POP Register            020h */
            __IO uint32_t GRXFSIZ;              /*!< Receive FIFO Size Register                   024h */
            __IO uint32_t DIEPTXF0_HNPTXFSIZ;   /*!< EP0 / Non Periodic Tx FIFO Size Register     028h */
            __IO uint32_t HNPTXSTS;             /*!< Non Periodic Tx FIFO/Queue Sts reg           02Ch */
            uint32_t Reserved30[2];             /*!< Reserved                                     030h */
            __IO uint32_t GCCFG;                /*!< General Purpose IO Register                  038h */
            __IO uint32_t CID;                  /*!< User ID Register                             03Ch */
            uint32_t  Reserved40[48];           /*!< Reserved                                0x40-0xFF */
            __IO uint32_t HPTXFSIZ;             /*!< Host Periodic Tx FIFO Size Reg               100h */
            __IO uint32_t DIEPTXF[0x0F];        /*!< dev Periodic Transmit FIFO                        */
        } USB_OTG_GlobalTypeDef;

        typedef struct
        {
            uint32_t dev_endpoints;           /*!< Device Endpoints number.
                                         This parameter depends on the used USB core.
                                         This parameter must be a number between Min_Data = 1 and Max_Data = 15 */

            uint32_t Host_channels;           /*!< Host Channels number.
                                         This parameter Depends on the used USB core.
                                         This parameter must be a number between Min_Data = 1 and Max_Data = 15 */

            uint32_t speed;                   /*!< USB Core speed.
                                         This parameter can be any value of @ref PCD_Speed/HCD_Speed
                                                                                 (HCD_SPEED_xxx, HCD_SPEED_xxx) */

            uint32_t dma_enable;              /*!< Enable or disable of the USB embedded DMA used only for OTG HS.        */

            uint32_t ep0_mps;                 /*!< Set the Endpoint 0 Max Packet size.                                    */

            uint32_t phy_itface;              /*!< Select the used PHY interface.
                                         This parameter can be any value of @ref PCD_PHY_Module/HCD_PHY_Module  */

            uint32_t Sof_enable;              /*!< Enable or disable the output of the SOF signal.                        */

            uint32_t low_power_enable;        /*!< Enable or disable the low power mode.                                  */

            uint32_t lpm_enable;              /*!< Enable or disable Link Power Management.                               */

            uint32_t battery_charging_enable; /*!< Enable or disable Battery charging.                                    */

            uint32_t vbus_sensing_enable;     /*!< Enable or disable the VBUS Sensing feature.                            */

            uint32_t use_dedicated_ep1;       /*!< Enable or disable the use of the dedicated EP1 interrupt.              */

            uint32_t use_external_vbus;       /*!< Enable or disable the use of the external VBUS.                        */

        } USB_OTG_CfgTypeDef;

        typedef struct
        {
            uint8_t   num;                  /*!< Endpoint number
                                       This parameter must be a number between Min_Data = 1 and Max_Data = 15   */

            uint8_t   is_in;                /*!< Endpoint direction
                                       This parameter must be a number between Min_Data = 0 and Max_Data = 1    */

            uint8_t   is_stall;             /*!< Endpoint stall condition
                                       This parameter must be a number between Min_Data = 0 and Max_Data = 1    */

            uint8_t   type;                 /*!< Endpoint type
                                       This parameter can be any value of @ref USB_LL_EP_Type                   */

            uint8_t   data_pid_start;       /*!< Initial data PID
                                       This parameter must be a number between Min_Data = 0 and Max_Data = 1    */

            uint8_t   even_odd_frame;       /*!< IFrame parity
                                       This parameter must be a number between Min_Data = 0 and Max_Data = 1    */

            uint16_t  tx_fifo_num;          /*!< Transmission FIFO number
                                       This parameter must be a number between Min_Data = 1 and Max_Data = 15   */

            uint32_t  maxpacket;            /*!< Endpoint Max packet size
                                       This parameter must be a number between Min_Data = 0 and Max_Data = 64KB */

            uint8_t   *xfer_buff;           /*!< Pointer to transfer buffer                                               */

            uint32_t  dma_addr;             /*!< 32 bits aligned transfer buffer address                                  */

            uint32_t  xfer_len;             /*!< Current transfer length                                                  */

            uint32_t  xfer_count;           /*!< Partial transfer length in case of multi packet transfer                 */
        } USB_OTG_EPTypeDef;

        typedef enum
        {
            HAL_UNLOCKED = 0x00U,
            HAL_LOCKED   = 0x01U
        } HAL_LockTypeDef;

        typedef enum
        {
            HAL_PCD_STATE_RESET   = 0x00,
            HAL_PCD_STATE_READY   = 0x01,
            HAL_PCD_STATE_ERROR   = 0x02,
            HAL_PCD_STATE_BUSY    = 0x03,
            HAL_PCD_STATE_TIMEOUT = 0x04
        } PCD_StateTypeDef;

        typedef enum
        {
            LPM_L0 = 0x00, /* on */
            LPM_L1 = 0x01, /* LPM L1 sleep */
            LPM_L2 = 0x02, /* suspend */
            LPM_L3 = 0x03, /* off */
        } PCD_LPM_StateTypeDef;

        typedef USB_OTG_GlobalTypeDef  PCD_TypeDef;
        typedef USB_OTG_CfgTypeDef     PCD_InitTypeDef;
        typedef USB_OTG_EPTypeDef      PCD_EPTypeDef;

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        typedef struct __PCD_HandleTypeDef
#else
        typedef struct
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
        {
            PCD_TypeDef             *Instance;   /*!< Register base address             */
            PCD_InitTypeDef         Init;        /*!< PCD required parameters           */
            __IO uint8_t            USB_Address; /*!< USB Address                       */
            PCD_EPTypeDef           IN_ep[16];   /*!< IN endpoint parameters            */
            PCD_EPTypeDef           OUT_ep[16];  /*!< OUT endpoint parameters           */
            HAL_LockTypeDef         Lock;        /*!< PCD peripheral status             */
            __IO PCD_StateTypeDef   State;       /*!< PCD communication state           */
            __IO  uint32_t          ErrorCode;   /*!< PCD Error code                    */
            uint32_t                Setup[12];   /*!< Setup packet buffer               */
            PCD_LPM_StateTypeDef    LPM_State;   /*!< LPM State                         */
            uint32_t                BESL;


            uint32_t lpm_active;                 /*!< Enable or disable the Link Power Management .
                                       This parameter can be set to ENABLE or DISABLE        */

            uint32_t battery_charging_active;    /*!< Enable or disable Battery charging.
                                       This parameter can be set to ENABLE or DISABLE        */
            void                    *pData;      /*!< Pointer to upper stack Handler */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
            void (* SOFCallback)(struct __PCD_HandleTypeDef *hpcd);                              /*!< USB OTG PCD SOF callback                */
  void (* SetupStageCallback)(struct __PCD_HandleTypeDef *hpcd);                       /*!< USB OTG PCD Setup Stage callback        */
  void (* ResetCallback)(struct __PCD_HandleTypeDef *hpcd);                            /*!< USB OTG PCD Reset callback              */
  void (* SuspendCallback)(struct __PCD_HandleTypeDef *hpcd);                          /*!< USB OTG PCD Suspend callback            */
  void (* ResumeCallback)(struct __PCD_HandleTypeDef *hpcd);                           /*!< USB OTG PCD Resume callback             */
  void (* ConnectCallback)(struct __PCD_HandleTypeDef *hpcd);                          /*!< USB OTG PCD Connect callback            */
  void (* DisconnectCallback)(struct __PCD_HandleTypeDef *hpcd);                       /*!< USB OTG PCD Disconnect callback         */

  void (* DataOutStageCallback)(struct __PCD_HandleTypeDef *hpcd, uint8_t epnum);      /*!< USB OTG PCD Data OUT Stage callback     */
  void (* DataInStageCallback)(struct __PCD_HandleTypeDef *hpcd, uint8_t epnum);       /*!< USB OTG PCD Data IN Stage callback      */
  void (* ISOOUTIncompleteCallback)(struct __PCD_HandleTypeDef *hpcd, uint8_t epnum);  /*!< USB OTG PCD ISO OUT Incomplete callback */
  void (* ISOINIncompleteCallback)(struct __PCD_HandleTypeDef *hpcd, uint8_t epnum);   /*!< USB OTG PCD ISO IN Incomplete callback  */
  void (* BCDCallback)(struct __PCD_HandleTypeDef *hpcd, PCD_BCD_MsgTypeDef msg);      /*!< USB OTG PCD BCD callback                */
  void (* LPMCallback)(struct __PCD_HandleTypeDef *hpcd, PCD_LPM_MsgTypeDef msg);      /*!< USB OTG PCD LPM callback                */

  void (* MspInitCallback)(struct __PCD_HandleTypeDef *hpcd);                          /*!< USB OTG PCD Msp Init callback           */
  void (* MspDeInitCallback)(struct __PCD_HandleTypeDef *hpcd);                        /*!< USB OTG PCD Msp DeInit callback         */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
        } PCD_HandleTypeDef;

        typedef enum
        {
            DISABLE = 0U,
            ENABLE = !DISABLE
        } FunctionalState;

        typedef enum
        {
            HAL_OK       = 0x00U,
            HAL_ERROR    = 0x01U,
            HAL_BUSY     = 0x02U,
            HAL_TIMEOUT  = 0x03U
        } HAL_StatusTypeDef;

        typedef enum
        {
            USB_DEVICE_MODE  = 0,
            USB_HOST_MODE    = 1,
            USB_DRD_MODE     = 2
        } USB_OTG_ModeTypeDef;

        #define EP_TYPE_CTRL                           0U


        #define USB_OTG_HS_PERIPH_BASE               0x40040000UL
        #define USB_OTG_FS_PERIPH_BASE               0x50000000UL

        #define USBD_FS_SPEED                          2U
        #define USBD_HSINFS_SPEED                      1U
        #define USBD_HS_SPEED                          0U

        #define PCD_SPEED_HIGH               USBD_HS_SPEED
        #define PCD_SPEED_HIGH_IN_FULL       USBD_HSINFS_SPEED
        #define PCD_SPEED_FULL               USBD_FS_SPEED

        #define USB_OTG_FS          ((USB_OTG_GlobalTypeDef *) USB_OTG_FS_PERIPH_BASE)
        #define USB_OTG_HS          ((USB_OTG_GlobalTypeDef *) USB_OTG_HS_PERIPH_BASE)

        #define PCD_PHY_EMBEDDED             2U

        #define IS_PCD_ALL_INSTANCE(INSTANCE) (((INSTANCE) == USB_OTG_FS) || \
                                               ((INSTANCE) == USB_OTG_HS))

        #define assert_param(expr) ((void)0U)
        #define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */

        #define __HAL_PCD_ENABLE(__HANDLE__)                       (void)USB_EnableGlobalInt ((__HANDLE__)->Instance)
        #define __HAL_PCD_DISABLE(__HANDLE__)                      (void)USB_DisableGlobalInt ((__HANDLE__)->Instance)

        #define USB_OTG_GAHBCFG_GINT_Pos                 (0U)
        #define USB_OTG_GAHBCFG_GINT_Msk                 (0x1UL << USB_OTG_GAHBCFG_GINT_Pos) /*!< 0x00000001 */
        #define USB_OTG_GAHBCFG_GINT                     USB_OTG_GAHBCFG_GINT_Msk      /*!< Global interrupt mask */

        #define USB_OTG_ULPI_PHY                       1U
        #define USB_OTG_EMBEDDED_PHY                   2U

        #define USB_OTG_GCCFG_PWRDWN_Pos                 (16U)
        #define USB_OTG_GCCFG_PWRDWN_Msk                 (0x1UL << USB_OTG_GCCFG_PWRDWN_Pos) /*!< 0x00010000 */
        #define USB_OTG_GCCFG_PWRDWN                     USB_OTG_GCCFG_PWRDWN_Msk      /*!< Power down */

        #define USB_OTG_GUSBCFG_TSDPS_Pos                (22U)
        #define USB_OTG_GUSBCFG_TSDPS_Msk                (0x1UL << USB_OTG_GUSBCFG_TSDPS_Pos) /*!< 0x00400000 */
        #define USB_OTG_GUSBCFG_TSDPS                    USB_OTG_GUSBCFG_TSDPS_Msk     /*!< TermSel DLine pulsing selection */

        #define USB_OTG_GUSBCFG_ULPIFSLS_Pos             (17U)
        #define USB_OTG_GUSBCFG_ULPIFSLS_Msk             (0x1UL << USB_OTG_GUSBCFG_ULPIFSLS_Pos) /*!< 0x00020000 */
        #define USB_OTG_GUSBCFG_ULPIFSLS                 USB_OTG_GUSBCFG_ULPIFSLS_Msk  /*!< ULPI FS/LS select               */

        #define USB_OTG_GUSBCFG_PHYSEL_Pos               (6U)
        #define USB_OTG_GUSBCFG_PHYSEL_Msk               (0x1UL << USB_OTG_GUSBCFG_PHYSEL_Pos) /*!< 0x00000040 */
        #define USB_OTG_GUSBCFG_PHYSEL                   USB_OTG_GUSBCFG_PHYSEL_Msk    /*!< USB 2.0 high-speed ULPI PHY or USB 1.1 full-speed serial transceiver select */

        #define USB_OTG_GUSBCFG_ULPIEVBUSD_Pos           (20U)
        #define USB_OTG_GUSBCFG_ULPIEVBUSD_Msk           (0x1UL << USB_OTG_GUSBCFG_ULPIEVBUSD_Pos) /*!< 0x00100000 */
        #define USB_OTG_GUSBCFG_ULPIEVBUSD               USB_OTG_GUSBCFG_ULPIEVBUSD_Msk /*!< ULPI External VBUS Drive        */

        #define USB_OTG_GUSBCFG_ULPIEVBUSI_Pos           (21U)
        #define USB_OTG_GUSBCFG_ULPIEVBUSI_Msk           (0x1UL << USB_OTG_GUSBCFG_ULPIEVBUSI_Pos) /*!< 0x00200000 */
        #define USB_OTG_GUSBCFG_ULPIEVBUSI               USB_OTG_GUSBCFG_ULPIEVBUSI_Msk /*!< ULPI external VBUS indicator    */

        #define USB_OTG_GRSTCTL_AHBIDL_Pos               (31U)
        #define USB_OTG_GRSTCTL_AHBIDL_Msk               (0x1UL << USB_OTG_GRSTCTL_AHBIDL_Pos) /*!< 0x80000000 */
        #define USB_OTG_GRSTCTL_AHBIDL                   USB_OTG_GRSTCTL_AHBIDL_Msk    /*!< AHB master idle */

        #define USB_OTG_GRSTCTL_CSRST_Pos                (0U)
        #define USB_OTG_GRSTCTL_CSRST_Msk                (0x1UL << USB_OTG_GRSTCTL_CSRST_Pos) /*!< 0x00000001 */
        #define USB_OTG_GRSTCTL_CSRST                    USB_OTG_GRSTCTL_CSRST_Msk     /*!< Core soft reset          */

        #define USB_OTG_GAHBCFG_DMAEN_Pos                (5U)
        #define USB_OTG_GAHBCFG_DMAEN_Msk                (0x1UL << USB_OTG_GAHBCFG_DMAEN_Pos) /*!< 0x00000020 */
        #define USB_OTG_GAHBCFG_DMAEN                    USB_OTG_GAHBCFG_DMAEN_Msk     /*!< DMA enable */

        #define USB_OTG_GAHBCFG_HBSTLEN_2                (0x3UL << USB_OTG_GAHBCFG_HBSTLEN_Pos) /*!< INCR4 */
        #define USB_OTG_GAHBCFG_HBSTLEN_Pos              (1U)

        #define USB_OTG_GUSBCFG_FHMOD_Pos                (29U)
        #define USB_OTG_GUSBCFG_FHMOD_Msk                (0x1UL << USB_OTG_GUSBCFG_FHMOD_Pos) /*!< 0x20000000 */
        #define USB_OTG_GUSBCFG_FHMOD                    USB_OTG_GUSBCFG_FHMOD_Msk     /*!< Forced host mode                */

        #define USB_OTG_GUSBCFG_FDMOD_Pos                (30U)
        #define USB_OTG_GUSBCFG_FDMOD_Msk                (0x1UL << USB_OTG_GUSBCFG_FDMOD_Pos) /*!< 0x40000000 */
        #define USB_OTG_GUSBCFG_FDMOD                    USB_OTG_GUSBCFG_FDMOD_Msk     /*!< Forced peripheral mode          */

        #define USB_OTG_DEVICE_BASE                  0x800UL

        typedef struct
        {
            __IO uint32_t DCFG;            /*!< dev Configuration Register   800h */
            __IO uint32_t DCTL;            /*!< dev Control Register         804h */
            __IO uint32_t DSTS;            /*!< dev Status Register (RO)     808h */
            uint32_t Reserved0C;           /*!< Reserved                     80Ch */
            __IO uint32_t DIEPMSK;         /*!< dev IN Endpoint Mask         810h */
            __IO uint32_t DOEPMSK;         /*!< dev OUT Endpoint Mask        814h */
            __IO uint32_t DAINT;           /*!< dev All Endpoints Itr Reg    818h */
            __IO uint32_t DAINTMSK;        /*!< dev All Endpoints Itr Mask   81Ch */
            uint32_t  Reserved20;          /*!< Reserved                     820h */
            uint32_t Reserved9;            /*!< Reserved                     824h */
            __IO uint32_t DVBUSDIS;        /*!< dev VBUS discharge Register  828h */
            __IO uint32_t DVBUSPULSE;      /*!< dev VBUS Pulse Register      82Ch */
            __IO uint32_t DTHRCTL;         /*!< dev threshold                830h */
            __IO uint32_t DIEPEMPMSK;      /*!< dev empty msk                834h */
            __IO uint32_t DEACHINT;        /*!< dedicated EP interrupt       838h */
            __IO uint32_t DEACHMSK;        /*!< dedicated EP msk             83Ch */
            uint32_t Reserved40;           /*!< dedicated EP mask            840h */
            __IO uint32_t DINEP1MSK;       /*!< dedicated EP mask            844h */
            uint32_t  Reserved44[15];      /*!< Reserved                 844-87Ch */
            __IO uint32_t DOUTEP1MSK;      /*!< dedicated EP msk             884h */
        } USB_OTG_DeviceTypeDef;

        #define USBx_DEVICE     ((USB_OTG_DeviceTypeDef *)(USBx_BASE + USB_OTG_DEVICE_BASE))

        #define USB_OTG_DCTL_SDIS_Pos                    (1U)
        #define USB_OTG_DCTL_SDIS_Msk                    (0x1UL << USB_OTG_DCTL_SDIS_Pos) /*!< 0x00000002 */
        #define USB_OTG_DCTL_SDIS                        USB_OTG_DCTL_SDIS_Msk         /*!< Soft disconnect         */

        #define USB_OTG_GCCFG_NOVBUSSENS_Pos             (21U)
        #define USB_OTG_GCCFG_NOVBUSSENS_Msk             (0x1UL << USB_OTG_GCCFG_NOVBUSSENS_Pos) /*!< 0x00200000 */
        #define USB_OTG_GCCFG_NOVBUSSENS                 USB_OTG_GCCFG_NOVBUSSENS_Msk  /*!< VBUS sensing disable option*/

        #define USB_OTG_GCCFG_VBUSBSEN_Pos               (19U)
        #define USB_OTG_GCCFG_VBUSBSEN_Msk               (0x1UL << USB_OTG_GCCFG_VBUSBSEN_Pos) /*!< 0x00080000 */
        #define USB_OTG_GCCFG_VBUSBSEN                   USB_OTG_GCCFG_VBUSBSEN_Msk    /*!< Enable the VBUS sensing device */

        #define USB_OTG_GCCFG_VBUSASEN_Pos               (18U)
        #define USB_OTG_GCCFG_VBUSASEN_Msk               (0x1UL << USB_OTG_GCCFG_VBUSASEN_Pos) /*!< 0x00040000 */
        #define USB_OTG_GCCFG_VBUSASEN                   USB_OTG_GCCFG_VBUSASEN_Msk    /*!< Enable the VBUS sensing device */

        #define USB_OTG_PCGCCTL_BASE                 0xE00UL

        #define USBx_PCGCCTL    *(__IO uint32_t *)((uint32_t)USBx_BASE + USB_OTG_PCGCCTL_BASE)

        #define DCFG_FRAME_INTERVAL_80                 0U
        #define USB_OTG_SPEED_HIGH                     0U
        #define USB_OTG_SPEED_HIGH_IN_FULL             1U
        #define USB_OTG_SPEED_FULL                     3U

        #define USB_OTG_GRSTCTL_RXFFLSH_Pos              (4U)
        #define USB_OTG_GRSTCTL_RXFFLSH_Msk              (0x1UL << USB_OTG_GRSTCTL_RXFFLSH_Pos) /*!< 0x00000010 */
        #define USB_OTG_GRSTCTL_RXFFLSH                  USB_OTG_GRSTCTL_RXFFLSH_Msk   /*!< RxFIFO flush             */

        #define USB_OTG_GRSTCTL_TXFFLSH_Pos              (5U)
        #define USB_OTG_GRSTCTL_TXFFLSH_Msk              (0x1UL << USB_OTG_GRSTCTL_TXFFLSH_Pos) /*!< 0x00000020 */
        #define USB_OTG_GRSTCTL_TXFFLSH                  USB_OTG_GRSTCTL_TXFFLSH_Msk   /*!< TxFIFO flush             */

        typedef struct
        {
            __IO uint32_t DIEPCTL;           /*!< dev IN Endpoint Control Reg    900h + (ep_num * 20h) + 00h */
            uint32_t Reserved04;             /*!< Reserved                       900h + (ep_num * 20h) + 04h */
            __IO uint32_t DIEPINT;           /*!< dev IN Endpoint Itr Reg        900h + (ep_num * 20h) + 08h */
            uint32_t Reserved0C;             /*!< Reserved                       900h + (ep_num * 20h) + 0Ch */
            __IO uint32_t DIEPTSIZ;          /*!< IN Endpoint Txfer Size         900h + (ep_num * 20h) + 10h */
            __IO uint32_t DIEPDMA;           /*!< IN Endpoint DMA Address Reg    900h + (ep_num * 20h) + 14h */
            __IO uint32_t DTXFSTS;           /*!< IN Endpoint Tx FIFO Status Reg 900h + (ep_num * 20h) + 18h */
            uint32_t Reserved18;             /*!< Reserved  900h+(ep_num*20h)+1Ch-900h+ (ep_num * 20h) + 1Ch */
        } USB_OTG_INEndpointTypeDef;

        #define USB_OTG_IN_ENDPOINT_BASE             0x900UL
        #define USB_OTG_EP_REG_SIZE                  0x20UL

        #define USBx_INEP(i)    ((USB_OTG_INEndpointTypeDef *)(USBx_BASE\
                                                               + USB_OTG_IN_ENDPOINT_BASE + ((i) * USB_OTG_EP_REG_SIZE)))

#define USB_OTG_DIEPCTL_EPENA_Pos                (31U)
#define USB_OTG_DIEPCTL_EPENA_Msk                (0x1UL << USB_OTG_DIEPCTL_EPENA_Pos) /*!< 0x80000000 */
#define USB_OTG_DIEPCTL_EPENA                    USB_OTG_DIEPCTL_EPENA_Msk     /*!< Endpoint enable                  */

#define USB_OTG_DIEPCTL_SNAK_Pos                 (27U)
#define USB_OTG_DIEPCTL_SNAK_Msk                 (0x1UL << USB_OTG_DIEPCTL_SNAK_Pos) /*!< 0x08000000 */
#define USB_OTG_DIEPCTL_SNAK                     USB_OTG_DIEPCTL_SNAK_Msk      /*!< Set NAK */

#define USB_OTG_DIEPCTL_EPDIS_Pos                (30U)
#define USB_OTG_DIEPCTL_EPDIS_Msk                (0x1UL << USB_OTG_DIEPCTL_EPDIS_Pos) /*!< 0x40000000 */
#define USB_OTG_DIEPCTL_EPDIS                    USB_OTG_DIEPCTL_EPDIS_Msk     /*!< Endpoint disable                 */

        typedef struct
        {
            __IO uint32_t DOEPCTL;       /*!< dev OUT Endpoint Control Reg           B00h + (ep_num * 20h) + 00h */
            uint32_t Reserved04;         /*!< Reserved                               B00h + (ep_num * 20h) + 04h */
            __IO uint32_t DOEPINT;       /*!< dev OUT Endpoint Itr Reg               B00h + (ep_num * 20h) + 08h */
            uint32_t Reserved0C;         /*!< Reserved                               B00h + (ep_num * 20h) + 0Ch */
            __IO uint32_t DOEPTSIZ;      /*!< dev OUT Endpoint Txfer Size            B00h + (ep_num * 20h) + 10h */
            __IO uint32_t DOEPDMA;       /*!< dev OUT Endpoint DMA Address           B00h + (ep_num * 20h) + 14h */
            uint32_t Reserved18[2];      /*!< Reserved B00h + (ep_num * 20h) + 18h - B00h + (ep_num * 20h) + 1Ch */
        } USB_OTG_OUTEndpointTypeDef;

#define USB_OTG_OUT_ENDPOINT_BASE            0xB00UL

#define USBx_OUTEP(i)   ((USB_OTG_OUTEndpointTypeDef *)(USBx_BASE\
                                                        + USB_OTG_OUT_ENDPOINT_BASE + ((i) * USB_OTG_EP_REG_SIZE)))

#define USB_OTG_DOEPCTL_EPENA_Pos                (31U)
#define USB_OTG_DOEPCTL_EPENA_Msk                (0x1UL << USB_OTG_DOEPCTL_EPENA_Pos) /*!< 0x80000000 */
#define USB_OTG_DOEPCTL_EPENA                    USB_OTG_DOEPCTL_EPENA_Msk     /*!< Endpoint enable */

#define USB_OTG_DOEPCTL_SNAK_Pos                 (27U)
#define USB_OTG_DOEPCTL_SNAK_Msk                 (0x1UL << USB_OTG_DOEPCTL_SNAK_Pos) /*!< 0x08000000 */
#define USB_OTG_DOEPCTL_SNAK                     USB_OTG_DOEPCTL_SNAK_Msk      /*!< Set NAK */

#define USB_OTG_DOEPCTL_EPDIS_Pos                (30U)
#define USB_OTG_DOEPCTL_EPDIS_Msk                (0x1UL << USB_OTG_DOEPCTL_EPDIS_Pos) /*!< 0x40000000 */
#define USB_OTG_DOEPCTL_EPDIS                    USB_OTG_DOEPCTL_EPDIS_Msk     /*!< Endpoint disable */

#define USB_OTG_DIEPMSK_TXFURM_Pos               (8U)
#define USB_OTG_DIEPMSK_TXFURM_Msk               (0x1UL << USB_OTG_DIEPMSK_TXFURM_Pos) /*!< 0x00000100 */
#define USB_OTG_DIEPMSK_TXFURM                   USB_OTG_DIEPMSK_TXFURM_Msk    /*!< FIFO underrun mask                                */

#define USB_OTG_GINTMSK_RXFLVLM_Pos              (4U)
#define USB_OTG_GINTMSK_RXFLVLM_Msk              (0x1UL << USB_OTG_GINTMSK_RXFLVLM_Pos) /*!< 0x00000010 */
#define USB_OTG_GINTMSK_RXFLVLM                  USB_OTG_GINTMSK_RXFLVLM_Msk   /*!< Receive FIFO nonempty mask                          */

#define USB_OTG_GINTMSK_USBSUSPM_Pos             (11U)
#define USB_OTG_GINTMSK_USBSUSPM_Msk             (0x1UL << USB_OTG_GINTMSK_USBSUSPM_Pos) /*!< 0x00000800 */
#define USB_OTG_GINTMSK_USBSUSPM                 USB_OTG_GINTMSK_USBSUSPM_Msk  /*!< USB suspend mask                                    */

#define USB_OTG_GINTMSK_USBRST_Pos               (12U)
#define USB_OTG_GINTMSK_USBRST_Msk               (0x1UL << USB_OTG_GINTMSK_USBRST_Pos) /*!< 0x00001000 */
#define USB_OTG_GINTMSK_USBRST                   USB_OTG_GINTMSK_USBRST_Msk    /*!< USB reset mask                                      */

#define USB_OTG_GINTMSK_ENUMDNEM_Pos             (13U)
#define USB_OTG_GINTMSK_ENUMDNEM_Msk             (0x1UL << USB_OTG_GINTMSK_ENUMDNEM_Pos) /*!< 0x00002000 */
#define USB_OTG_GINTMSK_ENUMDNEM                 USB_OTG_GINTMSK_ENUMDNEM_Msk  /*!< Enumeration done mask                               */

#define USB_OTG_GINTMSK_IEPINT_Pos               (18U)
#define USB_OTG_GINTMSK_IEPINT_Msk               (0x1UL << USB_OTG_GINTMSK_IEPINT_Pos) /*!< 0x00040000 */
#define USB_OTG_GINTMSK_IEPINT                   USB_OTG_GINTMSK_IEPINT_Msk    /*!< IN endpoints interrupt mask                         */

#define USB_OTG_GINTMSK_OEPINT_Pos               (19U)
#define USB_OTG_GINTMSK_OEPINT_Msk               (0x1UL << USB_OTG_GINTMSK_OEPINT_Pos) /*!< 0x00080000 */
#define USB_OTG_GINTMSK_OEPINT                   USB_OTG_GINTMSK_OEPINT_Msk    /*!< OUT endpoints interrupt mask                        */

#define USB_OTG_GINTMSK_IISOIXFRM_Pos            (20U)
#define USB_OTG_GINTMSK_IISOIXFRM_Msk            (0x1UL << USB_OTG_GINTMSK_IISOIXFRM_Pos) /*!< 0x00100000 */
#define USB_OTG_GINTMSK_IISOIXFRM                USB_OTG_GINTMSK_IISOIXFRM_Msk /*!< Incomplete isochronous IN transfer mask             */

#define USB_OTG_GINTMSK_PXFRM_IISOOXFRM_Pos      (21U)
#define USB_OTG_GINTMSK_PXFRM_IISOOXFRM_Msk      (0x1UL << USB_OTG_GINTMSK_PXFRM_IISOOXFRM_Pos) /*!< 0x00200000 */
#define USB_OTG_GINTMSK_PXFRM_IISOOXFRM          USB_OTG_GINTMSK_PXFRM_IISOOXFRM_Msk /*!< Incomplete periodic transfer mask                   */

#define USB_OTG_GINTMSK_WUIM_Pos                 (31U)
#define USB_OTG_GINTMSK_WUIM_Msk                 (0x1UL << USB_OTG_GINTMSK_WUIM_Pos) /*!< 0x80000000 */
#define USB_OTG_GINTMSK_WUIM                     USB_OTG_GINTMSK_WUIM_Msk      /*!< Resume/remote wakeup detected interrupt mask        */

#define USB_OTG_GINTMSK_SOFM_Pos                 (3U)
#define USB_OTG_GINTMSK_SOFM_Msk                 (0x1UL << USB_OTG_GINTMSK_SOFM_Pos) /*!< 0x00000008 */
#define USB_OTG_GINTMSK_SOFM                     USB_OTG_GINTMSK_SOFM_Msk      /*!< Start of frame mask                                 */

#define USB_OTG_GINTMSK_SRQIM_Pos                (30U)
#define USB_OTG_GINTMSK_SRQIM_Msk                (0x1UL << USB_OTG_GINTMSK_SRQIM_Pos) /*!< 0x40000000 */
#define USB_OTG_GINTMSK_SRQIM                    USB_OTG_GINTMSK_SRQIM_Msk     /*!< Session request/new session detected interrupt mask */

#define USB_OTG_GINTMSK_OTGINT_Pos               (2U)
#define USB_OTG_GINTMSK_OTGINT_Msk               (0x1UL << USB_OTG_GINTMSK_OTGINT_Pos) /*!< 0x00000004 */
#define USB_OTG_GINTMSK_OTGINT                   USB_OTG_GINTMSK_OTGINT_Msk    /*!< OTG interrupt mask                                  */

#define USB_OTG_PCGCCTL_STOPCLK_Pos              (0U)
#define USB_OTG_PCGCCTL_STOPCLK_Msk              (0x1UL << USB_OTG_PCGCCTL_STOPCLK_Pos) /*!< 0x00000001 */
#define USB_OTG_PCGCCTL_STOPCLK                  USB_OTG_PCGCCTL_STOPCLK_Msk   /*!< SETUP packet count */

#define USB_OTG_PCGCCTL_GATECLK_Pos              (1U)
#define USB_OTG_PCGCCTL_GATECLK_Msk              (0x1UL << USB_OTG_PCGCCTL_GATECLK_Pos) /*!< 0x00000002 */
#define USB_OTG_PCGCCTL_GATECLK                  USB_OTG_PCGCCTL_GATECLK_Msk   /*!<Bit 0 */

        typedef struct _USBD_CUSTOM_HID_Itf
        {
            uint8_t *pReport;
            int8_t (* Init)(void);
            int8_t (* DeInit)(void);
            int8_t (* OutEvent)(uint8_t event_idx, uint8_t state);

        } USBD_CUSTOM_HID_ItfTypeDef;

        /* Macro to get variable aligned on 4-bytes, for __ICCARM__ the directive "#pragma data_alignment=4" must be used instead */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
        #ifndef __ALIGN_BEGIN
    #define __ALIGN_BEGIN
  #endif
  #ifndef __ALIGN_END
    #define __ALIGN_END      __attribute__ ((aligned (4)))
  #endif
#elif defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
#ifndef __ALIGN_END
#define __ALIGN_END    __attribute__ ((aligned (4)))
#endif /* __ALIGN_END */
#ifndef __ALIGN_BEGIN
#define __ALIGN_BEGIN
#endif /* __ALIGN_BEGIN */
#else
        #ifndef __ALIGN_END
    #define __ALIGN_END
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN
    #if defined   (__CC_ARM)      /* ARM Compiler V5*/
#define __ALIGN_BEGIN    __align(4)
    #elif defined (__ICCARM__)    /* IAR Compiler */
      #define __ALIGN_BEGIN
    #endif /* __CC_ARM */
  #endif /* __ALIGN_BEGIN */
#endif /* __GNUC__ */

#define USBD_CUSTOM_HID_REPORT_DESC_SIZE     108U

#define __HAL_LOCK(__HANDLE__)                                           \
                                do{                                        \
                                    if((__HANDLE__)->Lock == HAL_LOCKED)   \
                                    {                                      \
                                       return HAL_BUSY;                    \
                                    }                                      \
                                    else                                   \
                                    {                                      \
                                       (__HANDLE__)->Lock = HAL_LOCKED;    \
                                    }                                      \
                                  }while (0U)

#define __HAL_UNLOCK(__HANDLE__)                                          \
                                  do{                                       \
                                      (__HANDLE__)->Lock = HAL_UNLOCKED;    \
                                    }while (0U)

#define USB_OTG_MODE_DEVICE                    0U
#define __HAL_PCD_IS_INVALID_INTERRUPT(__HANDLE__)         (USB_ReadInterrupts((__HANDLE__)->Instance) == 0U)

#define USB_OTG_GINTSTS_MMIS_Pos                 (1U)
#define USB_OTG_GINTSTS_MMIS_Msk                 (0x1UL << USB_OTG_GINTSTS_MMIS_Pos) /*!< 0x00000002 */
#define USB_OTG_GINTSTS_MMIS                     USB_OTG_GINTSTS_MMIS_Msk      /*!< Mode mismatch interrupt                        */

#define __HAL_PCD_GET_FLAG(__HANDLE__, __INTERRUPT__) \
  ((USB_ReadInterrupts((__HANDLE__)->Instance) & (__INTERRUPT__)) == (__INTERRUPT__))

#define USB_OTG_GINTSTS_MMIS_Pos                 (1U)
#define USB_OTG_GINTSTS_MMIS_Msk                 (0x1UL << USB_OTG_GINTSTS_MMIS_Pos) /*!< 0x00000002 */
#define USB_OTG_GINTSTS_MMIS                     USB_OTG_GINTSTS_MMIS_Msk      /*!< Mode mismatch interrupt                        */

    public:
        Usb(ClockControl &clockControl1);

        void MspInit();

        void Init() noexcept;

        void CoreReset() noexcept;

        void CoreInit();

        void SetCurremtMode();

        void DevInit();

        void SetRxFiFo(std::uint16_t size);

        void SetTxFiFo(std::uint8_t fifo, std::uint16_t size);

        void RegisterClass(std::uint8_t& add);

        void RegisterInterface();

        void USBD_Start();

        void DevDisconnect();

        void StartUSB();

        void DevConnect();

        void PCD_Start();

        //---------------------------------------------------------------------

        PCD_HandleTypeDef hpcd_USB_OTG_FS;
        USBD_HandleTypeDef hUsbDeviceFS;
        USBD_DescriptorsTypeDef FS_Desc;
        USBD_ClassTypeDef USBD_CUSTOM_HID;

//        __ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
//                {
//                        /* USER CODE BEGIN 0 */
//                        0x06, 0x00, 0xFF,   //USAGE_PAGE (GENERIC DESCTOP)
//                        0x09, 0x01,                    // USAGE (Vendor Usage 1)
//                        0xa1, 0x01,                    // COLLECTION (Application)
//                        0x85, 0x01,                    //   REPORT_ID (1)
//                        0x09, 0x01,                    //   USAGE (Vendor Usage 1)
//                        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//                        0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
//                        0x75, 0x08,                    //   REPORT_SIZE (8)
//                        0x95, 0x01,                    //   REPORT_COUNT (1)
//                        0xB1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
//                        0x85, 0x01,                    //   REPORT_ID (1)
//                        0x09, 0x01,                    //   USAGE (Vendor Usage 1)
//                        0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
//
//                        0x85, 0x02,                    //   REPORT_ID (2)
//                        0x09, 0x02,                    //   USAGE (Vendor Usage 2)
//                        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//                        0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
//                        0x75, 0x08,                    //   REPORT_SIZE (8)
//                        0x95, 0x01,                    //   REPORT_COUNT (1)
//                        0xB1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
//                        0x85, 0x02,                    //   REPORT_ID (2)
//                        0x09, 0x02,                    //   USAGE (Vendor Usage 2)
//                        0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
//
//                        0x85, 0x03,                    //   REPORT_ID (3)
//                        0x09, 0x03,                    //   USAGE (Vendor Usage 3)
//                        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//                        0x25, 0x01,                          //   LOGICAL_MAXIMUM (255)
//                        0x75, 0x08,                    //   REPORT_SIZE (8)
//                        0x95, 0x01,                    //   REPORT_COUNT (1)
//                        0xB1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
//                        0x85, 0x03,                    //   REPORT_ID (3)
//                        0x09, 0x03,                    //   USAGE (Vendor Usage 3)
//                        0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
//
//                        0x85, 0x04,                    //   REPORT_ID (4)
//                        0x09, 0x04,                    //   USAGE (Vendor Usage 4)
//                        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//                        0x25, 0x01,                          //   LOGICAL_MAXIMUM (255)
//                        0x75, 0x08,                    //   REPORT_SIZE (8)
//                        0x95, 0x01,                    //   REPORT_COUNT (1)
//                        0xB1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
//                        0x85, 0x04,                    //   REPORT_ID (4)
//                        0x09, 0x04,                    //   USAGE (Vendor Usage 4)
//                        0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
//
//                        0x85, 0x05,                    //   REPORT_ID (5)
//                        0x09, 0x05,                    //   USAGE (Vendor Usage 5)
//                        0x75, 0x08,                    //   REPORT_SIZE (8)
//                        0x95, 0x04,                    //   REPORT_COUNT (1)
//                        0x81, 0x82,                                         //		INPUT (Data,Var,Abs,Vol)
//
//                        0x85, 0x06,                    //   REPORT_ID (6)
//                        0x09, 0x06,                    //   USAGE (Vendor Usage 6)
//                        0x75, 0x08,                    //   REPORT_SIZE (8)
//                        0x95, 0x04,                    //   REPORT_COUNT (1)
//                        0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
//                        /* USER CODE END 0 */
//                        0xC0    /*     END_COLLECTION	             */
//                };


        static int8_t CUSTOM_HID_Init_FS(void);
        static int8_t CUSTOM_HID_DeInit_FS(void);
        static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

//        USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
//                {
//                        CUSTOM_HID_ReportDesc_FS,
//                        CUSTOM_HID_Init_FS,
//                        CUSTOM_HID_DeInit_FS,
//                        CUSTOM_HID_OutEvent_FS
//                };


        void MX_USB_DEVICE_Init(void);
        USBD_StatusTypeDef USBD_Init(USBD_HandleTypeDef *pdev, USBD_DescriptorsTypeDef *pdesc, uint8_t id);
        USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev);
        HAL_StatusTypeDef HAL_PCD_Init(PCD_HandleTypeDef *hpcd);
        void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd);
        HAL_StatusTypeDef USB_EnableGlobalInt(USB_OTG_GlobalTypeDef *USBx);
        HAL_StatusTypeDef USB_DisableGlobalInt(USB_OTG_GlobalTypeDef *USBx);
        HAL_StatusTypeDef USB_CoreInit(USB_OTG_GlobalTypeDef *USBx, USB_OTG_CfgTypeDef cfg);
        static HAL_StatusTypeDef USB_CoreReset(USB_OTG_GlobalTypeDef *USBx);
        HAL_StatusTypeDef USB_SetCurrentMode(USB_OTG_GlobalTypeDef *USBx, USB_OTG_ModeTypeDef mode);
        uint32_t USB_GetMode(USB_OTG_GlobalTypeDef *USBx);
        HAL_StatusTypeDef USB_DevInit(USB_OTG_GlobalTypeDef *USBx, USB_OTG_CfgTypeDef cfg);
        HAL_StatusTypeDef USB_SetDevSpeed(USB_OTG_GlobalTypeDef *USBx, uint8_t speed);
        HAL_StatusTypeDef USB_FlushRxFifo(USB_OTG_GlobalTypeDef *USBx);
        HAL_StatusTypeDef USB_FlushTxFifo(USB_OTG_GlobalTypeDef *USBx, uint32_t num);
        HAL_StatusTypeDef  USB_DevDisconnect(USB_OTG_GlobalTypeDef *USBx);
        HAL_StatusTypeDef HAL_PCDEx_SetRxFiFo(PCD_HandleTypeDef *hpcd, uint16_t size);
        HAL_StatusTypeDef HAL_PCDEx_SetTxFiFo(PCD_HandleTypeDef *hpcd, uint8_t fifo, uint16_t size);
        USBD_StatusTypeDef USBD_RegisterClass(USBD_HandleTypeDef *pdev, USBD_ClassTypeDef *pclass);
        uint8_t USBD_CUSTOM_HID_RegisterInterface(USBD_HandleTypeDef *pdev,
                                                  USBD_CUSTOM_HID_ItfTypeDef *fops);
        USBD_StatusTypeDef USBD_Start(USBD_HandleTypeDef *pdev);
        USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev);
        HAL_StatusTypeDef HAL_PCD_Start(PCD_HandleTypeDef *hpcd);
        HAL_StatusTypeDef  USB_DevConnect(USB_OTG_GlobalTypeDef *USBx);
        USBD_StatusTypeDef USBD_Get_USB_Status(HAL_StatusTypeDef hal_status);

        //------------------------------------------------------------------------------------

        void HAL_PCD_IRQHandler(PCD_HandleTypeDef *hpcd);
        uint32_t  USB_ReadInterrupts(USB_OTG_GlobalTypeDef *USBx);


    };
}

#endif //STM32F4_USB_H
