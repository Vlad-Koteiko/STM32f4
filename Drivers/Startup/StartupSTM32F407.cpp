//
// Created by koteiko_vv on 01.02.2023.
//
#include <cstdint>
#include "main.hpp"
#include "usart.hpp"

// Constants from linker
extern std::uint32_t _sidata;    // Start of init data
extern std::uint32_t _sdata;     // Start of data
extern std::uint32_t _edata;     // End of data
extern std::uint32_t _sbss;      // Start of bss
extern std::uint32_t _ebss;      // End of bss
extern std::uint32_t _estack;    // Top of stack

using vector = std::add_pointer<void()>::type; // void (*)(void)

//Static constructor initializator from libc
//extern void __libc_init_array();

// Main program endtry point
//extern int main();

//extern void SystemInit();

// Reset Handler
void Reset_Handler() {

    // Initialize data segment
    uint32_t *dataInit = &_sidata;
    uint32_t *data = &_sdata;
    while(data < &_edata) {
        *data++ = *dataInit++;
    }

    // Initialize bss segment
    uint32_t *bss = &_sbss;
    while(bss < &_ebss) {
        *bss++ = 0;
    }

    // Run  ST system init
    // SystemInit();

    // Initilize static constructors
    //__libc_init_array();

    // Enter main program
//    main();
    Bass bass;
    bass.main();

    // Handle the case where main function returns
    while(1);
}

    void Default_Handler(void) {
        while(1);
    }

 // Weak definitions of interupt handlers
     void __attribute__((weak)) NMI_Handler (void)                          {Default_Handler();}
     void __attribute__((weak)) HardFault_Handler (void)                    {Default_Handler();}
     void __attribute__((weak)) MemManage_Handler (void)                    {Default_Handler();}
     void __attribute__((weak)) BusFault_Handler (void)                     {Default_Handler();}
     void __attribute__((weak)) UsageFault_Handler (void)                   {Default_Handler();}
     void __attribute__((weak)) SVC_Handler (void)                          {Default_Handler();}
     void __attribute__((weak)) DebugMon_Handler (void)                     {Default_Handler();}
     void __attribute__((weak)) PendSV_Handler (void)                       {Default_Handler();}
     void __attribute__((weak)) SysTick_Handler (void)                      {Default_Handler();}
     void __attribute__((weak)) WWDG_IRQHandler(void)                       {Default_Handler();}  // Window WatchDog
     void __attribute__((weak)) PVD_IRQHandler(void)                        {Default_Handler();}  // PVD through EXTI Line detection
     void __attribute__((weak)) TAMP_STAMP_IRQHandler(void)                 {Default_Handler();}  // Tamper and TimeStamps through the EXTI line
     void __attribute__((weak)) RTC_WKUP_IRQHandler(void)                   {Default_Handler();}  // RTC Wakeup through the EXTI line
     void __attribute__((weak)) FLASH_IRQHandler(void)                      {Default_Handler();}  // FLASH
     void __attribute__((weak)) RCC_IRQHandler(void)                        {Default_Handler();}  // RCC
     void __attribute__((weak)) EXTI0_IRQHandler(void)                      {Default_Handler();}  // EXTI Line0
     void __attribute__((weak)) EXTI1_IRQHandler(void)                      {Default_Handler();}  // EXTI Line1
     void __attribute__((weak)) EXTI2_IRQHandler(void)                      {Default_Handler();}  // EXTI Line2
     void __attribute__((weak)) EXTI3_IRQHandler(void)                      {Default_Handler();}  // EXTI Line3
     void __attribute__((weak)) EXTI4_IRQHandler(void)                      {Default_Handler();}  // EXTI Line4
     void __attribute__((weak)) DMA1_Stream0_IRQHandler(void)               {Default_Handler();}  // DMA1 Stream 0
     void __attribute__((weak)) DMA1_Stream1_IRQHandler(void)               {Default_Handler();}  // DMA1 Stream 1
     void __attribute__((weak)) DMA1_Stream2_IRQHandler(void)               {Default_Handler();}  // DMA1 Stream 2
     void __attribute__((weak)) DMA1_Stream3_IRQHandler(void)               {Default_Handler();}  // DMA1 Stream 3
     void __attribute__((weak)) DMA1_Stream4_IRQHandler(void)               {Default_Handler();}  // DMA1 Stream 4
     void __attribute__((weak)) DMA1_Stream5_IRQHandler(void)               {Default_Handler();}  // DMA1 Stream 5
     void __attribute__((weak)) DMA1_Stream6_IRQHandler(void)               {Default_Handler();}  // DMA1 Stream 6
     void __attribute__((weak)) ADC_IRQHandler(void)                        {Default_Handler();}  // ADC1, ADC2 and ADC3s
     void __attribute__((weak)) CAN1_TX_IRQHandler(void)                    {Default_Handler();}  // CAN1 TX
     void __attribute__((weak)) CAN1_RX0_IRQHandler(void)                   {Default_Handler();}  // CAN1 RX0
     void __attribute__((weak)) CAN1_RX1_IRQHandler(void)                   {Default_Handler();}  // CAN1 RX1
     void __attribute__((weak)) CAN1_SCE_IRQHandler(void)                   {Default_Handler();}  // CAN1 SCE
     void __attribute__((weak)) EXTI9_5_IRQHandler(void)                    {Default_Handler();}  // External Line[9:5]s
     void __attribute__((weak)) TIM1_BRK_TIM9_IRQHandler(void)              {Default_Handler();}  // TIM1 Break and TIM9
     void __attribute__((weak)) TIM1_UP_TIM10_IRQHandler(void)              {Default_Handler();}  // TIM1 Update and TIM10
     void __attribute__((weak)) TIM1_TRG_COM_TIM11_IRQHandler(void)         {Default_Handler();}  // TIM1 Trigger and Commutation and TIM11
     void __attribute__((weak)) TIM1_CC_IRQHandler(void)                    {Default_Handler();}  // TIM1 Capture Compare
     void __attribute__((weak)) TIM2_IRQHandler(void)                       {Default_Handler();}  // TIM2
     void __attribute__((weak)) TIM3_IRQHandler(void)                       {Default_Handler();}  // TIM3
     void __attribute__((weak)) TIM4_IRQHandler(void)                       {Default_Handler();}  // TIM4
     void __attribute__((weak)) I2C1_EV_IRQHandler(void)                    {Default_Handler();}  // I2C1 Event
     void __attribute__((weak)) I2C1_ER_IRQHandler(void)                    {Default_Handler();}  // I2C1 Error
     void __attribute__((weak)) I2C2_EV_IRQHandler(void)                    {Default_Handler();}  // I2C2 Event
     void __attribute__((weak)) I2C2_ER_IRQHandler(void)                    {Default_Handler();}  // I2C2 Error
     void __attribute__((weak)) SPI1_IRQHandler(void)                       {Default_Handler();}  // SPI1
     void __attribute__((weak)) SPI2_IRQHandler(void)                       {Default_Handler();}  // SPI2
     void __attribute__((weak)) USART1_IRQHandler(void)                     {Default_Handler();}  // USART1
     void __attribute__((weak)) USART2_IRQHandler(void)                     {Default_Handler();}  // USART2
     void __attribute__((weak)) USART3_IRQHandler(void)                     {Default_Handler();}  // USART3
     void __attribute__((weak)) EXTI15_10_IRQHandler(void)                  {Default_Handler();}  // External Line[15:10]s
     void __attribute__((weak)) RTC_Alarm_IRQHandler(void)                  {Default_Handler();}  // RTC Alarm (A and B) through EXTI Line
     void __attribute__((weak)) OTG_FS_WKUP_IRQHandler(void)                {Default_Handler();}  // USB OTG FS Wakeup through EXTI line
     void __attribute__((weak)) TIM8_BRK_TIM12_IRQHandler(void)             {Default_Handler();}  // TIM8 Break and TIM12
     void __attribute__((weak)) TIM8_UP_TIM13_IRQHandler(void)              {Default_Handler();}  // TIM8 Update and TIM13
     void __attribute__((weak)) TIM8_TRG_COM_TIM14_IRQHandler(void)         {Default_Handler();}  // TIM8 Trigger and Commutation and TIM14
     void __attribute__((weak)) TIM8_CC_IRQHandler(void)                    {Default_Handler();}  // TIM8 Capture Compare
     void __attribute__((weak)) DMA1_Stream7_IRQHandler(void)               {Default_Handler();}  // DMA1 Stream7
     void __attribute__((weak)) FSMC_IRQHandler(void)                       {Default_Handler();}  // FSMC
     void __attribute__((weak)) SDIO_IRQHandler(void)                       {Default_Handler();}  // SDIO
     void __attribute__((weak)) TIM5_IRQHandler(void)                       {Default_Handler();}  // TIM5
     void __attribute__((weak)) SPI3_IRQHandler(void)                       {Default_Handler();}  // SPI3
     void __attribute__((weak)) UART4_IRQHandler(void)                      {Default_Handler();}  // UART4
     void __attribute__((weak)) UART5_IRQHandler(void)                      {Default_Handler();}  // UART5
     void __attribute__((weak)) TIM6_DAC_IRQHandler(void)                   {Default_Handler();}  // TIM6 and DAC1&2 underrun errors
     void __attribute__((weak)) TIM7_IRQHandler(void)                       {Default_Handler();}  // TIM7
     void __attribute__((weak)) DMA2_Stream0_IRQHandler(void)               {Default_Handler();}  // DMA2 Stream 0
     void __attribute__((weak)) DMA2_Stream1_IRQHandler(void)               {Default_Handler();}  // DMA2 Stream 1
     void __attribute__((weak)) DMA2_Stream2_IRQHandler(void)               {Default_Handler();}  // DMA2 Stream 2
     void __attribute__((weak)) DMA2_Stream3_IRQHandler(void)               {Default_Handler();}  // DMA2 Stream 3
     void __attribute__((weak)) DMA2_Stream4_IRQHandler(void)               {Default_Handler();}  // DMA2 Stream 4
     void __attribute__((weak)) ETH_IRQHandler(void)                        {Default_Handler();}  // Ethernet
     void __attribute__((weak)) ETH_WKUP_IRQHandler(void)                   {Default_Handler();}  // Ethernet Wakeup through EXTI line
     void __attribute__((weak)) CAN2_TX_IRQHandler(void)                    {Default_Handler();}  // CAN2 TX
     void __attribute__((weak)) CAN2_RX0_IRQHandler(void)                   {Default_Handler();}  // CAN2 RX0
     void __attribute__((weak)) CAN2_RX1_IRQHandler(void)                   {Default_Handler();}  // CAN2 RX1
     void __attribute__((weak)) CAN2_SCE_IRQHandler(void)                   {Default_Handler();}  // CAN2 SCE
     void __attribute__((weak)) OTG_FS_IRQHandler(void)                     {Default_Handler();}  // USB OTG FS
     void __attribute__((weak)) DMA2_Stream5_IRQHandler(void)               {Default_Handler();}  // DMA2 Stream 5
     void __attribute__((weak)) DMA2_Stream6_IRQHandler(void)               {Default_Handler();}  // DMA2 Stream 6
     void __attribute__((weak)) DMA2_Stream7_IRQHandler(void)               {Default_Handler();}  // DMA2 Stream 7
     void __attribute__((weak)) USART6_IRQHandler(void)                     {Default_Handler();}  // USART6
     void __attribute__((weak)) I2C3_EV_IRQHandler(void)                    {Default_Handler();}  // I2C3 event
     void __attribute__((weak)) I2C3_ER_IRQHandler(void)                    {Default_Handler();}  // I2C3 error
     void __attribute__((weak)) OTG_HS_EP1_OUT_IRQHandler(void)             {Default_Handler();}  // USB OTG HS End Point 1 Out
     void __attribute__((weak)) OTG_HS_EP1_IN_IRQHandler(void)              {Default_Handler();}  // USB OTG HS End Point 1 In
     void __attribute__((weak)) OTG_HS_WKUP_IRQHandler(void)                {Default_Handler();}  // USB OTG HS Wakeup through EXTI
     void __attribute__((weak)) OTG_HS_IRQHandler(void)                     {Default_Handler();}  // USB OTG HS
     void __attribute__((weak)) DCMI_IRQHandler(void)                       {Default_Handler();}  // DCMI
     void __attribute__((weak)) HASH_RNG_IRQHandler(void)                   {Default_Handler();}  // Hash and Rng
     void __attribute__((weak)) FPU_IRQHandler(void)                        {Default_Handler();}  // FPU

    // Interrupt vector table
    __attribute__((section(".isr_vector")))
     vector VectorTable[] = {                       // void (*VectorTable[])(void)
            (vector) &_estack,
              Reset_Handler,
              NMI_Handler,
              HardFault_Handler,
              MemManage_Handler,
              BusFault_Handler,
              UsageFault_Handler,
              0,
              0,
              0,
              0,
              SVC_Handler,
              DebugMon_Handler,
              0,
              PendSV_Handler,
              SysTick_Handler,
              WWDG_IRQHandler,                   // Window WatchDog
              PVD_IRQHandler,                    // PVD through EXTI Line detection
              TAMP_STAMP_IRQHandler,             // Tamper and TimeStamps through the EXTI line
              RTC_WKUP_IRQHandler,               // RTC Wakeup through the EXTI line
              FLASH_IRQHandler,                  // FLASH
              RCC_IRQHandler,                    // RCC
              EXTI0_IRQHandler,                  // EXTI Line0
              EXTI1_IRQHandler,                  // EXTI Line1
              EXTI2_IRQHandler,                  // EXTI Line2
              EXTI3_IRQHandler,                  // EXTI Line3
              EXTI4_IRQHandler,                  // EXTI Line4
              DMA1_Stream0_IRQHandler,           // DMA1 Stream 0
              DMA1_Stream1_IRQHandler,           // DMA1 Stream 1
              DMA1_Stream2_IRQHandler,           // DMA1 Stream 2
              DMA1_Stream3_IRQHandler,           // DMA1 Stream 3
              DMA1_Stream4_IRQHandler,           // DMA1 Stream 4
              DMA1_Stream5_IRQHandler,           // DMA1 Stream 5
              DMA1_Stream6_IRQHandler,           // DMA1 Stream 6
              ADC_IRQHandler,                    // ADC1, ADC2 and ADC3s
              CAN1_TX_IRQHandler,                // CAN1 TX
              CAN1_RX0_IRQHandler,               // CAN1 RX0
              CAN1_RX1_IRQHandler,               // CAN1 RX1
              CAN1_SCE_IRQHandler,               // CAN1 SCE
              EXTI9_5_IRQHandler,                // External Line[9:5]s
              TIM1_BRK_TIM9_IRQHandler,          // TIM1 Break and TIM9
              TIM1_UP_TIM10_IRQHandler,          // TIM1 Update and TIM10
              TIM1_TRG_COM_TIM11_IRQHandler,     // TIM1 Trigger and Commutation and TIM11
              TIM1_CC_IRQHandler,                // TIM1 Capture Compare
              TIM2_IRQHandler,                   // TIM2
              TIM3_IRQHandler,                   // TIM3
              TIM4_IRQHandler,                   // TIM4
              I2C1_EV_IRQHandler,                // I2C1 Event
              I2C1_ER_IRQHandler,                // I2C1 Error
              I2C2_EV_IRQHandler,                // I2C2 Event
              I2C2_ER_IRQHandler,                // I2C2 Error
              SPI1_IRQHandler,                   // SPI1
              SPI2_IRQHandler,                   // SPI2
              USART1_IRQHandler,                 // USART1
              USART2_IRQHandler,                 // USART2
              USART3_IRQHandler,                 // USART3
              EXTI15_10_IRQHandler,              // External Line[15:10]s
              RTC_Alarm_IRQHandler,              // RTC Alarm (A and B) through EXTI Line
              OTG_FS_WKUP_IRQHandler,            // USB OTG FS Wakeup through EXTI line
              TIM8_BRK_TIM12_IRQHandler,         // TIM8 Break and TIM12
              TIM8_UP_TIM13_IRQHandler,          // TIM8 Update and TIM13
              TIM8_TRG_COM_TIM14_IRQHandler,     // TIM8 Trigger and Commutation and TIM14
              TIM8_CC_IRQHandler,                // TIM8 Capture Compare
              DMA1_Stream7_IRQHandler,           // DMA1 Stream7
              FSMC_IRQHandler,                   // FSMC
              SDIO_IRQHandler,                   // SDIO
              TIM5_IRQHandler,                   // TIM5
              SPI3_IRQHandler,                   // SPI3
              UART4_IRQHandler,                  // UART4
              UART5_IRQHandler,                  // UART5
              TIM6_DAC_IRQHandler,               // TIM6 and DAC1&2 underrun errors
              TIM7_IRQHandler,                   // TIM7
              DMA2_Stream0_IRQHandler,           // DMA2 Stream 0
              DMA2_Stream1_IRQHandler,           // DMA2 Stream 1
              DMA2_Stream2_IRQHandler,           // DMA2 Stream 2
              DMA2_Stream3_IRQHandler,           // DMA2 Stream 3
              DMA2_Stream4_IRQHandler,           // DMA2 Stream 4
              ETH_IRQHandler,                    // Ethernet
              ETH_WKUP_IRQHandler,               // Ethernet Wakeup through EXTI line
              CAN2_TX_IRQHandler,                // CAN2 TX
              CAN2_RX0_IRQHandler,               // CAN2 RX0
              CAN2_RX1_IRQHandler,               // CAN2 RX1
              CAN2_SCE_IRQHandler,               // CAN2 SCE
              OTG_FS_IRQHandler,                 // USB OTG FS
              DMA2_Stream5_IRQHandler,           // DMA2 Stream 5
              DMA2_Stream6_IRQHandler,           // DMA2 Stream 6
              DMA2_Stream7_IRQHandler,           // DMA2 Stream 7
              USART6_IRQHandler,                 // USART6
              I2C3_EV_IRQHandler,                // I2C3 event
              I2C3_ER_IRQHandler,                // I2C3 error
              OTG_HS_EP1_OUT_IRQHandler,         // USB OTG HS End Point 1 Out
              OTG_HS_EP1_IN_IRQHandler,          // USB OTG HS End Point 1 In
              OTG_HS_WKUP_IRQHandler,            // USB OTG HS Wakeup through EXTI
              OTG_HS_IRQHandler,                 // USB OTG HS
              DCMI_IRQHandler,                   // DCMI
              0,                                 // CRYP crypto
              HASH_RNG_IRQHandler,               // Hash and Rng
              FPU_IRQHandler,                    // FPU
    };
