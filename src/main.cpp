//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.hpp"


std::uint32_t counter = 0;
std::uint8_t bufferReceve = 0;


drivers::timers::BasicTimers *tim6Pointer;
drivers::usart::USART *uart_p_2;
libs::Cout *globalCout;

std::uint8_t recvDma[5];

void Bass::main() noexcept {

    drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
    libs::MWR::write_register(0xE000ED88, 0xF00000);
    drivers::port::GPIO gpioA(clockControl, drivers::port::PORTA);
    auto portdRef = &gpioA;
    //Init(clockControl);
    //-----------------------------------------USART INIT---------------------------------
    drivers::usart::USART usart2(clockControl, drivers::usart::USART2);
    uart_p_2 = &usart2;
    usart2.EnableInterrupt(drivers::usart::INTERRUPT::RXNEIE);

    gpioA.InitPin(drivers::port::PIN_1, drivers::port::OUTPUT);
    //        gpioD.InitPin(drivers::port::PIN_13, drivers::port::OUTPUT);
    //        gpioD.InitPin(drivers::port::PIN_14, drivers::port::OUTPUT);
    //        gpioD.InitPin(drivers::port::PIN_15, drivers::port::OUTPUT);

    gpioA.SetPin(drivers::port::PIN_1, drivers::port::PIN_SET);
    //        gpioD.SetPin(drivers::port::PIN_13, drivers::port::PIN_SET);
    //        gpioD.SetPin(drivers::port::PIN_14, drivers::port::PIN_SET);
    //        gpioD.SetPin(drivers::port::PIN_15, drivers::port::PIN_SET);
    //        clockControl.mDelay(500);

    //------------------------------------------------------------------------------------
    drivers::nvic::NVIC nvic;
    nvic.NVIC_EnableIRQ(drivers::nvic::TIM6_DAC);
    nvic.NVIC_EnableIRQ(drivers::nvic::USART2);
    nvic.NVIC_EnableIRQ(drivers::nvic::DMA1_Stream5);
    nvic.NVIC_EnableIRQ(drivers::nvic::DMA1_Stream6);

    //        drivers::timers::BasicTimers timer6(clockControl, drivers::timers::TIM6, std::chrono::milliseconds(1000), true);
    //        drivers::timers::BasicTimers timer6(clockControl, drivers::timers::TIM6);
    //        timer6.EnableUpdateEvent();
    //        timer6.EnableInterrupt();
    //        timer6.SetUpdateSource(drivers::timers::COUNTER);
    //        timer6.SetAutoReload(10000);
    //        timer6.SetPrescaler(8400);
    //        timer6.EnableARRPreload();
    //        timer6.EnableCounter();

    drivers::dma::DMA dma1(clockControl, drivers::dma::ADDRESSES_DMA::DMA_1);
    auto globalDma1 = &dma1;

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

    //usart2.InitUsartDma(dma1, true, true);
    //usart2.InitUsartDma(dma1, dmaConf, true, true);

//    std::uint8_t  buf[] = "Transmit dma\n\r\0";
//    usart2.TransmitDataDma(dma1, buf, 14);
//    usart2.ReceiveDataDma(dma1, recvDma, 5);

    while (1)
    {
      //printf("hello\n\r");
        //            gpioD.TogglePin(drivers::port::PIN_12);
        //            clockControl.mDelay(500);
        //            gpioD.TogglePin(drivers::port::PIN_13);
        //            clockControl.mDelay(500);
        //            gpioD.TogglePin(drivers::port::PIN_14);
        //            clockControl.mDelay(500);
        //            gpioA.TogglePin(drivers::port::PIN_1);
        //            cout << "bufferReceve = " << bufferReceve << cout.ENDL;
        //
                    clockControl.mDelay(500);
    }

}

//void EXTI0_IRQHandler()
//{
//    drivers::exti::EXIT_RTSR exitRtsr;
//    exitRtsr.ClearFlag(exitRtsr.LINE_0);
//    counter++;
//}
//
//void USART1_IRQHandler()
//{
//    //        bufferReceve = drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>::ReceiveData();
//    //        uart_p_2->ClearFlag_RXNE();
//    //        bufferReceve = uart_p_2->ReceiveData();
//}
//
//void USART2_IRQHandler()
//{
//    //        bufferReceve = drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>::ReceiveData();
//    uart_p_2->ClearFlag(drivers::usart::SR_clear_flag::RXNE);
//    bufferReceve = uart_p_2->ReceiveData();
//}
//
//void OTG_FS_IRQHandler()
//{
//
//}
//
//void TIM6_DAC_IRQHandler()
//{
//    portdRef->TogglePin(drivers::port::PIN_12);
//    //libs::MWR::resetBit(0x40001010, 0);
//    tim6Pointer->ClearUpdateInterruptFlag();
//    counter++;
//}
//
//void DMA1_Stream5_IRQHandler()
//{
//    globalCout->operator<<("stream5\n\r");
//    globalDma1->clearFlagInterrupt(drivers::dma::constants::Stream_5, drivers::dma::constants::TRANSFER_COMPLETE_INTERRUPT);
//}
//
//void DMA1_Stream6_IRQHandler()
//{
//    globalCout->operator<<("stream6\n\r");
//    //libs::MWR::write_register(0x4002600C, 0xFFFFFFFF);
//    //        libs::MWR::setBit(0x4002600C, 21);
//    //        globalDma1->disable(drivers::dma::constants::Stream_6);
//    globalDma1->clearFlagInterrupt(drivers::dma::constants::Stream_6, drivers::dma::constants::TRANSFER_COMPLETE_INTERRUPT);
//    //        globalDma1->clearFlagInterrupt(drivers::dma::constants::Stream_6, drivers::dma::constants::HALF_TRANSFER_INTERRUPT);
//}
