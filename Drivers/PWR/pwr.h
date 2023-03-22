#ifndef PWR_H
#define PWR_H

#include "MWR.hpp"
#include "clockControl.hpp"

namespace drivers::pwr
{
    enum CR_poz
    {
        LPDS = 0,   // Low-power deepsleep
        PDDS = 1,   // Power-down deepsleep
        CWUF = 2,   // Clear wakeup flag
        CSBF = 3,   // Clear standby flag
        PVDE = 4,   // Power voltage detector enable
        PLS  = 5,   // PVD level selection [5:7]
        DBP  = 8,   // Disable backup domain write protection
        FPDS = 9,   // Flash power-down in Stop mode
        VOS  = 14   // Regulator voltage scaling output selection
    };

    enum CSR_poz
    {
        WUF    = 0, // Wakeup flag
        SBF    = 1, // Standby flag
        PVDO   = 2, // PVD output
        BRR    = 3, // Backup regulator ready
        EWUP   = 8, // Enable WKUP pin
        BRE    = 9, // Backup regulator enable
        VOSRDY = 14 // Regulator voltage scaling output selection ready bit
    };

    enum VOLTAGE : std::uint8_t
    {
        V20,
        V21,
        V23,
        V25,
        V26,
        V27,
        V28,
        V29
    };

    class PWR
    {
        static constexpr std::uintptr_t baseAddress = 0x40007000;
        drivers::clock::ClockControl &clockControl;

        enum RegisterPWR : std::uintptr_t
        {
            CR = baseAddress,
            CSR = baseAddress + 4
        };

    public:

        PWR(drivers::clock::ClockControl &clockControl);
        void SetLPDS();
        void ResetLPDS();
        void SetPDDS();
        void ResetPDDS();
        void ClearWakeUp();
        void ClearStandbyFlag();
        void PvdEnabled();
        void PvdDisable();
        bool GetStatusPvd();
        void SetPvdLevel(VOLTAGE v);
        std::uint8_t GetPdvLevel();
        void BackupDomainEnable();
        void BackupDomainDisable();
        bool GetStatusBackupDomain();

    };
}

#endif //PWR_H