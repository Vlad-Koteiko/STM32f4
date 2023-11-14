#ifndef __CAN_HPP__
#define __CAN_HPP__

#include "MWR.hpp"
#include "gpio.hpp"
#include "nvic.h"

namespace drivers::can
{
    /// @brief CAN addresses
    enum ADDRESSES_CAN : std::uintptr_t
    {
        CAN1 = 0x4000'6400,     ///< CAN1
        CAN2 = 0x4000'6800      ///< CAN2
    };

    class Can
    {
    private:

        /// @brief CAN master control register
        enum CAN_MCR
        {
            INRQ    = 0,        ///< Initialization request [rw]
            SLEEP   = 1,        ///< Sleep mode request [rw]
            TXFP    = 2,        ///< Transmit FIFO priority [rw]
            RFLM    = 3,        ///< Receive FIFO locked mode [rw]
            NART    = 4,        ///< No automatic retransmission [rw]
            AWUM    = 5,        ///< Automatic wakeup mode [rw]
            ABOM    = 6,        ///< Automatic bus-off management [rw]
            TTCM    = 7,        ///< Time triggered communication mode [rs]
            RESET   = 15,       ///< bxCAN software master reset [rw]
            DBF     = 16        ///< Debug freeze [rw]
        };

        /// @brief CAN master status register
        enum CAN_MSR
        {
            INAK    = 0,        ///< Initialization acknowledge [r]
            SLAK    = 1,        ///< Sleep acknowledge [r]
            ERRI    = 2,        ///< Error interrupt [rc_w1]
            WKUI    = 3,        ///< Wakeup interrupt [rc_w1]
            SLAKI   = 4,        ///< Sleep acknowledge interrupt [rc_w1]
            TXM     = 8,        ///< Transmit mode [r]
            RXM     = 9,        ///< Receive mode [r]
            SAMP    = 10,       ///< Last sample point [r]
            RX      = 11        ///< Can Rx signal
        };

        /// @brief CAN transmit status register
        enum CAN_TSR
        {
            RQCP0   = 0,        ///< Request completed mailbox0 [rc_w1]
            TXOK0   = 1,        ///< Transmit OK of mailbox0 [rc_w1]
            ALST0   = 2,        ///< Arbitration lost for mailbox0 [rc_w1]
            TERR0   = 3,        ///< Transmission error of mailbox0 [rc_w1]
            ABRQ0   = 7,        ///< Abort request for mailbox0 [rs]
            RQCP1   = 8,        ///< Request completed mailbox1 [rc_w1]
            TXOK1   = 9,        ///< Transmit OK of mailbox1 [rc_w1]
            ALST1   = 10,       ///< Arbitration lost for mailbox1 [rc_w1]
            TERR1   = 11,       ///< Transmission error of mailbox1 [rc_w1]
            ABRQ2   = 15,       ///< Abort request for mailbox1 [rs]
            RQCP2   = 16,       ///< Request completed mailbox2 [rc_w1]
            TXOK2   = 17,       ///< Transmit OK of mailbox2 [rc_w1]
            ALST2   = 18,       ///< Arbitration lost for mailbox2 [rc_w1]
            TERR2   = 19,       ///< Transmission error of mailbox2 [rc_w1]
            ABRQ2   = 23,       ///< Abort request for mailbox2 [rs]
            CODE    = 24,       ///< Mailbox code [1:0] [r]
            TME0    = 26,       ///< Transmit mailbox 0 empty [r]
            TME1    = 27,       ///< Transmit mailbox 1 empty [r]
            TME2    = 28,       ///< Transmit mailbox 2 empty [r]
            LOW0    = 29,       ///< Lowest priority flag for mailbox 0 [r]
            LOW1    = 30,       ///< Lowest priority flag for mailbox 1 [r]
            LOW2    = 31        ///< Lowest priority flag for mailbox 2 [r]
        };

        /// @brief CAN receive FIFO 0 register
        enum CAN_RF0R
        {
            FMP0    = 0,        ///< FIFO 0 message pending [1:0] [r]
            FULL0   = 3,        ///< FIFO 0 full [rc_w1]
            FOVR0   = 4,        ///< FIFO 0 overrun [rc_w1]
            RFOM0   = 5         ///< Release FIFO 0 output mailbox [rs]
        };

        /// @brief CAN receive FIFO 1 register
        enum CAN_RF1R
        {
            FMP1    = 0,        ///< FIFO 1 message pending [1:0] [r]
            FULL1   = 3,        ///< FIFO 1 full [rc_w1]
            FOVR1   = 4,        ///< FIFO 1 overrun [rc_w1]
            RFOM1   = 5         ///< Release FIFO 1 output mailbox [rs]
        };

        /// @brief CAN interrupt enable register
        enum CAN_IER
        {
            TMEIE   = 0,        ///< Transmit mailbox empty interrupt enable [rw]
            FMPIE0  = 1,        ///< FIFO message pending interrupt enable [rw]
            FFIE0   = 2,        ///< FIFO full interrupt enable [rw]
            FOVIE0  = 3,        ///< FIFO overrun interrupt enable [rw]
            FMPIE1  = 4,        ///< FIFO message pending interrupt enable [rw]
            FFIE1   = 5,        ///< FIFO full interrupt enable [rw]
            FOVIE1  = 6,        ///< FIFO overrun interrupt enable [rw]
            EWGIE   = 8,        ///< Error warning interrupt enable [rw]
            EPVIE   = 9,        ///< Error passive interrupt enable [rw]
            BOFIE   = 10,       ///< Bus-off interrupt enable [rw]
            LECIE   = 11,       ///< Last error code interrupt enable [rw]
            ERRIE   = 15,       ///< Error interrupt enable [rw]
            WKUIE   = 16,       ///< Wakeup interrupt enable [rw]
            SLKIE   = 17        ///< Sleep interrupt enable [rw]
        }

        /// @brief CAN error status register
        enum CAN_ESR
        {
            EWGF    = 0,        ///< Error warning flag [r]
            EPVF    = 1,        ///< Error passive flag [r]
            BOFF    = 2,        ///< Bus-off flag [r]
            LEC     = 4,        ///< Last error code [2:0] [rw]
            TEC     = 16,       ///< Least significant byte of the 9-bit transmit error counter [7:0] [r]
            REC     = 24        ///< Receive error counter [7:0] [r]
        };

        /// @brief CAN bit timing register
        enum CAN_BTR
        {
            BRP     = 0,        ///< Baud rate prescaler [9:0] [rw]
            TS1     = 16,       ///< Time segment 1 [3:0] [rw]
            TS2     = 20,       ///< Time segment 2 [2:0] [rw]
            SJW     = 24,       ///< Resynchronization jump width [1:0] [rw]
            LBKM    = 30,       ///< Loop back mode [rw]
            SILM    = 31        ///< Silent mode [rw]
        };

        enum CAN_TIxR_Offset
        {
            CAN_TI0XR   = 0x180,
            CAN_TI1XR   = 0x190,
            CAN_TI2XR   = 0x1A0
        };

        /// @brief CAN TX mailbox identifier register x=0..2
        enum CAN_TIxR
        {
            TXRQ    = 0,        ///< Tramsmit mailbox request [rw]
            RTR     = 1,        ///< Remote transmission request [rw]
            IDE     = 2,        ///< Identifier extension [rw]
            EXID    = 3,        ///< Extended [17:0] [rw]
            STID_EXID = 21      ///< Standard identifier or extended identifier [10:0]/[28:18] [rw]
        };

        enum CAN_TDTxR_Offset
        {
            CAN_TDT0R   = 0x184,
            CAN_TDT1R   = 0x194,
            CAN_TDT2R   = 0x1A4
        };

        /// @brief CAN mailbox data length control and time stamp register x=0..2
        enum CAN_TDTxR
        {
            DLC     = 0,        ///< Data length code [3:0] [rw]
            TGT     = 8,        ///< Transmit global time [rw]
            TIME    = 15        ///< Message time stamp [15:0] [rw]
        };

        enum CAN_TDLxR_Offset
        {
            CAN_TDL0R   = 0x188,
            CAN_TDL1R   = 0x198,
            CAN_TDL2R   = 0x1A8
        };

        /// @brief CAN mailbox data low register x=0..2
        enum CAN_TDLxR
        {
            DATA0       = 0,    ///< Data byte 0 [rw]
            DATA1       = 8,    ///< Data byte 1 [rw]
            DATA2       = 16,   ///< Data byte 2 [rw]
            DATA3       = 24    ///< Data byte 3 [rw]
        };

        enum CAN_TDHxR_Offset
        {
            CAN_TDH0R   = 0x18C,
            CAN_TDH1R   = 0x19C,
            CAN_TDH2R   = 0x1AC
        };

        /// @brief CAN mailbox data high register x=0..2
        enum CAN_TDLxR
        {
            DATA4       = 0,    ///< Data byte 4 [rw]
            DATA5       = 8,    ///< Data byte 5 [rw]
            DATA6       = 16,   ///< Data byte 6 [rw]
            DATA7       = 24,   ///< Data byte 7 [rw]
        };

        enum CAN_RIxR_Offset
        {
            CAN_RI0R    = 0x1B0,
            CAN_RI0R    = 0x1C0
        };

        /// @brief CAN receive FIFO mailbox identifier register x=0..1
        enum CAN_RIxR
        {
            RTR     = 1,        ///< Remote transmission request [r]
            IDE     = 2,        ///< Identifier extension [r]
            EXID    = 3,        ///< Extended identifier [17:0] [r]
            STID_EXID = 21      ///< Standard identifier or extended identifier [10:0]/[28:8] [r]
        };

        enum CAN_RDTxR_Offset
        {
            CAN_RDT0R = 0x1B4,
            CAN_RDT1R = 0x1C4
        };

        /// @brief CAN receive FIFO mailbox data length control and time stamp register x=0..1
        enum CAN_RDTxR
        {
            DLC     = 0,        ///< Data length code [3:0] [r]
            FMI     = 8,        ///< Filter match index [7:0] [r]
            TIME    = 16,       ///< Message time stamp [15:0] [r]
        };

        enum CAN_RDLxR_Offset
        {
            CAN_RDL0R   = 0x1B8,
            CAN_RDL1R   = 0x1C8
        };

        /// @brief CAN receive FIFO mailbox data low register x=0..1
        enum CAN_RDLxR
        {
            DATA0       = 0,    ///< Data byte 0 [7:0] [rw]
            DATA1       = 8,    ///< Data byte 1 [7:0] [rw]
            DATA2       = 16,   ///< Data byte 2 [7:0] [rw]
            DATA3       = 24,   ///< Data byte 3 [7:0] [rw]
        };

        enum CAN_RDHxR_Offset
        {
            CAN_RDH0R   = 0x1BC,
            CAN_RDH1R   = 0x1CC
        };

        /// @brief CAN receive FIFO mailbox data high register
        enum CAN_RDHxR
        {
            DATA4       = 0,    ///< Data byte 4 [7:0] [rw]
            DATA5       = 8,    ///< Data byte 5 [7:0] [rw]
            DATA6       = 16,   ///< Data byte 6 [7:0] [rw]
            DATA7       = 24,   ///< Data byte 7 [7:0] [rw]
        };

        /// @brief CAN filter master register
        enum CAN_FMR
        {
            FINIT       = 0,    ///< Filter init mode [rw]
            CAN2SB      = 8,    ///< CAN2 start bank [5:0] [rw]
        };

        enum RegisterCAN
        {
            CAN_MCR     = 0x000,
            CAN_MSR     = 0x004,
            CAN_TSR     = 0x008,
            CAN_RF0R    = 0x00C,
            CAN_RF1R    = 0x010,
            CAN_IER     = 0x014,
            CAN_ESR     = 0x018,
            CAN_BTR     = 0x01C,

            CAN_FMR     = 0x200,
            CAN_FM1R    = 0x204,
            CAN_FS1R    = 0x20C,
            CAN_FFA1R   = 0x214,
            CAN_FA1R    = 0x21C,

            CAN_F0R1    = 0x240,
            CAN_F0R2    = 0x244
        }

    public:
        Can(ADDRESSES_CAN address);
        bool start();
        bool stop();
    }; 
}

#endif // __CAN_HPP__