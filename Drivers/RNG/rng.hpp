/*! \file rng.hpp
    \brief class RNG

    Header file with a description of the random number generator class
*/

#ifndef RNG_HPP
#define RNG_HPP

#include "MWR.hpp"
#include "clockControl.hpp"

/// @brief Namespace for work with RNG
namespace drivers::rng
{
    /// @brief RNG control register
    enum CR_poz : std::uint8_t
    {
        RNGREN = 2,    ///< Random number generator enable
        IE     = 3     ///< Interrupt enable
    };

    /// @brief RNG status register
    enum SR_poz : std::uint8_t
    {
        DRDY = 0,    ///< Data ready
        CECS = 1,    ///< Clock error current status
        SECS = 2,    ///< Seed error current status
        CEIS = 5,    ///< Clock error interrupt status
        SEIS = 6,    ///< Seed error interrupt status
    };

    /// @brief Class for working with RNG
    class RNG
    {
        drivers::clock::ClockControl   &clockControl;                   ///< Link to class ClockControl
        static constexpr std::uintptr_t baseAddress = 0x50060800;       ///< Base addres RNG

        enum RegisterRNG : std::uintptr_t
        {
            CR = baseAddress,        ///< RNG control register
            SR = baseAddress + 4,    ///< RNG status register
            DR = baseAddress + 8,    ///< RNG data register
        };

        /// @brief Constructor RNG
        /// @param clockControl Reference ClockControl
        RNG(drivers::clock::ClockControl &clockControl);

        /// @brief Enable RNG
        void          Enable() noexcept;

        /// @brief Disable RNG
        void          Disable() noexcept;

        /// @brief Is enabled RNG
        /// @return true - enable, false - disable
        bool          IsEnabled() noexcept;

        /// @brief Data ready
        /// @return true - ready, false - not ready
        bool          IsActiveFlag_DRDY() noexcept;

        /// @brief Clock error current status
        /// @return true - not correctly, false - correctly
        bool          IsActiveFlag_CECS() noexcept;

        /// @brief Seed error current status
        /// @return false - not detected, true - detected
        bool          IsActiveFlag_SECS() noexcept;

        /// @brief Clock error interrupt status
        /// @return false - correctly detected, true - not correctly detected
        bool          IsActiveFlag_CEIS() noexcept;

        /// @brief Seed error interrupt status
        /// @return false - no faulty sequence detected
        bool          IsActiveFlag_SEIS() noexcept;

        /// @brief Clear flag CEIS
        void          ClearFlag_CEIS() noexcept;

        /// @brief Clear flag SEIS
        void          ClearFlag_SEIS() noexcept;

        /// @brief Enable interrupt
        void          EnableIT() noexcept;

        /// @brief Disable interrupt
        void          DisableIT() noexcept;

        /// @brief Is enabled interrupt
        /// @return true - enabled
        bool          IsEnabledIT() noexcept;

        /// @brief Get random data
        /// @return random data
        std::uint32_t ReadRandData32() noexcept;
    };
}    // namespace drivers::rng

#endif    // RNG_HPP