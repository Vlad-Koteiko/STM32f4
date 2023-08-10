#ifndef __ABSTRACTTIMER_H__
#define __ABSTRACTTIMER_H__

#include <cstdint>

#include "MWR.hpp"
#include "timersconst.hpp"

namespace drivers::timers::abstracttimer
{
    class AbstractTimer
    {
    protected:
        std::uintptr_t baseAddress;    ///< Base address

        /// @brief Registers BasicTimer
        enum Registers : std::uintptr_t
        {
            CR1   = 0x00,    ///< Control register 1
            CR2   = 0x04,    ///< Control register 2
            DIER  = 0x0C,    ///< DMA/Interrupt enable register
            SR    = 0x10,    ///< Status register
            EGR   = 0x14,    ///< Event generation register
            CCMR1 = 0x18,    ///< Capture/compare mode register 1
            CCER  = 0x20,    ///< Capture/compare enable register
            CNT   = 0x24,    ///< counter
            PSC   = 0x28,    ///< Prescaller
            ARR   = 0x2C,    ///< auto-reload register
            CCR1  = 0x34,    ///< Capture/compare register 1
            CCR2  = 0x38,    ///< Capture/compare register 2
            OR    = 0x50     ///< Option register
        };

    public:
        AbstractTimer(std::uintptr_t base_address);

        /// @brief Enable counter
        void enable() noexcept;

        /// @brief Disable counter
        void disable() noexcept;

        /// @brief Is enable counter
        /// @return true - enable
        bool isEnabledCounter() noexcept;

        /// @brief Update disable
        /// @param s ENABLE/DISABLE
        void updateDisable(STATUS s);

        /// @brief Is update disable
        /// @return true - Enable
        bool isUpdateDisable();

        /// @brief Update request source
        /// @param s ENABLE/DISABLE
        void updateRequestSource(STATUS s);

        /// @brief Is update request source
        /// @return true - Enable
        bool isUpdateRequestSource();

        /// @brief One pulse mode
        /// @param s ENABLE/DISABLE
        void onePulseMode(STATUS s);

        /// @brief Is one pulse mode
        /// @return true - Enable
        bool isOnePulseMode();

        /// @brief Auto-reload preload enable
        /// @param s ENABLE/DISABLE
        void autoReloadPreloadEnable(STATUS s);

        /// @brief Is auto-reload preload enable
        /// @return true - Enable
        bool isAutoReloadPreloadEnable();

        /// @brief Master mode selection
        /// @param m Mode
        void masterModeSelection(MASTER_MODE_SELECTION m);

        /// @brief Get master mode selection
        /// @return Mode
        MASTER_MODE_SELECTION getMasterModeSelection();

        /// @brief Enable interrupt
        /// @param f Source interrupt
        void enableIT(SOURCE_INTERRUPR si);

        /// @brief Disable interrupt
        /// @param f Source interrupt
        void disabelIT(SOURCE_INTERRUPR si);

        /// @brief Is enabled interrupt
        /// @param f Source interrupt
        bool isEnabledIT(SOURCE_INTERRUPR si);

        /// @brief Enable request DMA
        /// @param rd Request DMA
        void enableRequestDMA(REQUESTS_DMA rd);

        /// @brief Disable request DMA
        /// @param rd Request DMA
        void disableRequestDMA(REQUESTS_DMA rd);

        /// @brief Is enabled request DMA
        /// @return true - Enable
        bool isEnabledRequestDMA(REQUESTS_DMA rd);

        /// @brief Get the value of the status register flag
        /// @param flag Name flag
        /// @return Value
        bool getFlagStatusRegister(SR_poz flag);

        /// @brief Clear the value of the status register flag
        /// @param flag Name flag
        void clearFlagStatusRegister(SR_poz flag);

        /// @brief Enable event generation
        /// @param flag Name flag
        void enableEventGeneration(EGR_poz flag);

        /// @brief Disable event generation
        /// @param flag Name flag
        void diasbelEventGeneration(EGR_poz flag);

        /// @brief Is enabled event generation
        /// @param flag Name flag
        /// @return true - Enable
        bool isEnabledEventGeneration(EGR_poz flag);

        /// @brief Set counter
        /// @param val value
        void setCounter(std::uint16_t val) noexcept;

        /// @brief Get counter
        /// @return value
        std::uint16_t getCounter() noexcept;

        /// @brief Set prescaler
        /// @param val value
        void setPrescaler(std::uint16_t val) noexcept;

        /// @brief Get prescaler
        /// @return value
        std::uint16_t getPrescaler() noexcept;

        /// @brief Set autoreload
        /// @param val value
        void setAutoReload(std::uint16_t val) noexcept;

        /// @brief Get autoreload
        /// @return value
        std::uint16_t getAutoReload() noexcept;
    };
}    // namespace drivers::timers::abstracttimer

#endif    // __ABSTRACTTIMER_H__