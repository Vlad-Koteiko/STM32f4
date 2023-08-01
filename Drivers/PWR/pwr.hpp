/*! \file pwr.hpp
    \brief class PWR

    Header file with a description of the power controller class
*/

#ifndef PWR_H
#define PWR_H

#include "MWR.hpp"
#include "clockControl.hpp"

/// @brief Namespace for work with PWR
namespace drivers::pwr
{
    /// @brief  Power control register
    enum CR_poz
    {
        LPDS = 0,    ///< Low-power deepsleep
        PDDS = 1,    ///< Power-down deepsleep
        CWUF = 2,    ///< Clear wakeup flag
        CSBF = 3,    ///< Clear standby flag
        PVDE = 4,    ///< Power voltage detector enable
        PLS  = 5,    ///< PVD level selection [5:7]
        DBP  = 8,    ///< Disable backup domain write protection
        FPDS = 9,    ///< Flash power-down in Stop mode
        VOS  = 14    ///< Regulator voltage scaling output selection
    };

    /// @brief Power control/status register
    enum CSR_poz
    {
        WUF    = 0,    ///< Wakeup flag
        SBF    = 1,    ///< Standby flag
        PVDO   = 2,    ///< PVD output
        BRR    = 3,    ///< Backup regulator ready
        EWUP   = 8,    ///< Enable WKUP pin
        BRE    = 9,    ///< Backup regulator enable
        VOSRDY = 14    ///< Regulator voltage scaling output selection ready bit
    };

    /// @brief Voltage level
    enum VOLTAGE : std::uint8_t
    {
        V20,    ///< 2.0 V
        V21,    ///< 2.1 V
        V23,    ///< 2.3 V
        V25,    ///< 2.5 V
        V26,    ///< 2.6 V
        V27,    ///< 2.7 V
        V28,    ///< 2.8 V
        V29     ///< 2.9 V
    };

    /// @brief Class for working with PWR
    class PWR
    {
        static constexpr std::uintptr_t baseAddress = 0x40007000;    ///< base address
        drivers::clock::ClockControl   &clockControl;                ///< Reference clock control

        /// @brief Registers PWR
        enum RegisterPWR : std::uintptr_t
        {
            CR  = baseAddress,       ///< Control register
            CSR = baseAddress + 4    ///< Control/status register
        };

    public:
        /// @brief Constructor PWR
        /// @param clockControl Clock control
        PWR(drivers::clock::ClockControl &clockControl);

        /// @brief Set low-power deepsleep
        void SetLPDS();

        /// @brief Reset low-power deepslee
        void ResetLPDS();

        /// @brief Set power-down deepsleep
        void SetPDDS();

        /// @brief Reset power-down deepsleep
        void ResetPDDS();

        /// @brief Clear wakeup flag
        void ClearWakeUp();

        /// @brief  Clear standby flag
        void ClearStandbyFlag();

        /// @brief  Power voltage detector enable
        void PvdEnabled();

        /// @brief  Power voltage detector disable
        void PvdDisable();

        /// @brief  Get status power voltage detector
        /// @return true -  enable
        bool GetStatusPvd();

        /// @brief Set power voltage detector level
        /// @param v enum VOLTAGE
        void SetPvdLevel(VOLTAGE v);

        /// @brief Get power voltage detector level
        /// @return value
        std::uint8_t GetPdvLevel();

        /// @brief Enable backup domain write protection
        void BackupDomainEnable();

        /// @brief Disable backup domain write protection
        void BackupDomainDisable();

        /// @brief Get status backup domain write protection
        /// @return true - enable
        bool GetStatusBackupDomain();

        /// @brief Wakeup flag
        /// @return true - set
        bool GetWakeUpFlag();

        /// @brief Standby flag
        /// @return true - set
        bool GetStandbyFlag();

        /// @brief PVD output
        /// @return true - set
        bool GetPvdOutput();

        /// @brief Backup regulator ready
        /// @return true - ready
        bool BackupRegulatorReady();

        /// @brief Wakeup pin enable
        void WakeupPinEnable();

        /// @brief Wakeup pin disable
        void WakeupPinDisable();

        /// @brief Get status wakeup pin
        /// @return true - enable
        bool GetStatusWakeupPin();

        /// @brief Backup regulator enable
        void BackupRegulatorEnable();

        /// @brief Backup regulator disable
        void BackupRegulatorDisable();

        /// @brief Get status backup regulator
        /// @return true - enable
        bool GetStatusBackupRegulator();

        /// @brief Regulator voltage scaling output selection ready bit
        /// @return true - ready
        bool GetRegulatorVoltageScallingReady();
    };
}    // namespace drivers::pwr

#endif    // PWR_H