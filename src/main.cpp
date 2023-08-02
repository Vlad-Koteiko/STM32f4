//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.hpp"

std::uint32_t counter      = 0;
std::uint8_t  bufferReceve = 0;

drivers::timers::BasicTimer *tim6Pointer;
drivers::usart::Usart       *uart_p_2;
libs::Cout                  *globalCout;

std::uint8_t recvDma[5];

void Bass::main() noexcept
{
    drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
    clockControl.init();
    drivers::port::Gpio gpioD(clockControl, drivers::port::PORTD);
    auto                portdRef = &gpioD;
    // Init(clockControl);
    //-----------------------------------------USART INIT---------------------------------
    drivers::usart::Usart usart2(clockControl, drivers::usart::USART2);
    usart2.init();
    //    uart_p_2 = &usart2;
    //    usart2.EnableInterrupt(drivers::usart::INTERRUPT::RXNEIE);

    gpioD.InitPin(drivers::port::PIN_12, drivers::port::OUTPUT);
    gpioD.InitPin(drivers::port::PIN_13, drivers::port::OUTPUT);
    gpioD.InitPin(drivers::port::PIN_14, drivers::port::OUTPUT);
    gpioD.InitPin(drivers::port::PIN_15, drivers::port::OUTPUT);

    gpioD.SetPin(drivers::port::PIN_12, drivers::port::PIN_SET);
    gpioD.SetPin(drivers::port::PIN_13, drivers::port::PIN_SET);
    gpioD.SetPin(drivers::port::PIN_14, drivers::port::PIN_SET);
    gpioD.SetPin(drivers::port::PIN_15, drivers::port::PIN_SET);
    //        clockControl.mDelay(500);

    //------------------------------------------------------------------------------------
    drivers::nvic::NVIC nvic;
    nvic.NVIC_EnableIRQ(drivers::nvic::TIM6_DAC);
    nvic.NVIC_EnableIRQ(drivers::nvic::USART2);
    nvic.NVIC_EnableIRQ(drivers::nvic::DMA1_Stream5);
    nvic.NVIC_EnableIRQ(drivers::nvic::DMA1_Stream6);

    //        drivers::timers::BasicTimers timer6(clockControl, drivers::timers::TIM6,
    //        std::chrono::milliseconds(1000), true); drivers::timers::BasicTimers
    //        timer6(clockControl, drivers::timers::TIM6); timer6.EnableUpdateEvent();
    //        timer6.EnableInterrupt();
    //        timer6.SetUpdateSource(drivers::timers::COUNTER);
    //        timer6.SetAutoReload(10000);
    //        timer6.SetPrescaler(8400);
    //        timer6.EnableARRPreload();
    //        timer6.EnableCounter();

    //    drivers::dma::DMA dma1(clockControl, drivers::dma::ADDRESSES_DMA::DMA_1);
    //    auto globalDma1 = &dma1;

    //
    //    drivers::dma::DMA_Config dmaConf;
    //
    //    dmaConf.directModeErrorInterrupt = drivers::dma::constants::DISABLE;
    //    dmaConf.transferErrorInterrupt = drivers::dma::constants::DISABLE;
    //    dmaConf.halfTransferInterrupt = drivers::dma::constants::DISABLE;
    //    dmaConf.transferCompleteInterrupt = drivers::dma::constants::ENABLE;
    //    dmaConf.peripheralFlowController = drivers::dma::constants::DISABLE;
    //    dmaConf.circularMode = drivers::dma::constants::DISABLE;
    //    dmaConf.priorityLevel = drivers::dma::constants::LOW;
    //    dmaConf.doubleBufferMode = drivers::dma::constants::DISABLE;
    //    dmaConf.currentTarget = drivers::dma::constants::MEMORY0;
    //    dmaConf.peripheralBurstTransferConfiguration = drivers::dma::constants::SINGLE_TRANSFER;
    //    dmaConf.memoryBurstTransferConfiguration = drivers::dma::constants::SINGLE_TRANSFER;
    //    dmaConf.FIFOThresholdSelection = drivers::dma::constants::FIFO_0_25;
    //    dmaConf.directModeDisableInvers = drivers::dma::constants::ENABLE;
    //    dmaConf.fifoErrorInterrupt = drivers::dma::constants::DISABLE;

    // usart2.InitUsartDma(dma1, true, true);
    // usart2.InitUsartDma(dma1, dmaConf, true, true);

    //    std::uint8_t  buf[] = "Transmit dma\n\r\0";
    //    usart2.TransmitDataDma(dma1, buf, 14);
    //    usart2.ReceiveDataDma(dma1, recvDma, 5);

    drivers::spi::SPI spi1(clockControl, drivers::spi::SPI1);

    spi1.RemapSPI1(drivers::spi::SPI1_PA7_PA6_PA5_PA15);
    spi1.SetClockPhase(drivers::spi::EDGE2);
    spi1.SetClockPolarity(drivers::spi::HIGH);
    spi1.SetMode(drivers::spi::MASTER);
    spi1.SetBaudRatePrescaler(drivers::spi::DIV256);
    spi1.SetTransferBitOrder(drivers::spi::MSB_FIRST);
    spi1.SetTransferDirection(drivers::spi::FULL_DUPLEX);
    spi1.SetDataWidth(drivers::spi::BIT8);
    spi1.SetNSSMode(drivers::spi::HARD_OUTPUT);
    spi1.SetStandard(drivers::spi::MOTOROLA);
    spi1.Enable();

    spi1.TransmitData8(0xAA);

    while(1)
    {
        gpioD.TogglePin(drivers::port::PIN_12);
        clockControl.mDelay(500);
        gpioD.TogglePin(drivers::port::PIN_13);
        clockControl.mDelay(500);
        gpioD.TogglePin(drivers::port::PIN_14);
        clockControl.mDelay(500);
        gpioD.TogglePin(drivers::port::PIN_15);
        clockControl.mDelay(500);
        spi1.TransmitData8(0xAA);
    }
}
