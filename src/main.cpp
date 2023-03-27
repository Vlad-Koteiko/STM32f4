//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.h"
#include "basictimer.h"

std::uint32_t counter = 0;
std::uint8_t bufferReceve = 0;

//drivers::clock::ClockControl &clk;
drivers::port::GPIO *portdRef;
drivers::timers::BasicTimers *tim6Pointer;


    int main()
    {
        drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
        libs::MWR::write_register(0xE000ED88, 0xF00000);
        drivers::port::GPIO gpioD(clockControl, drivers::port::PORTD);
        portdRef = &gpioD;

        //Init(clockControl);
        //-----------------------------------------USART INIT---------------------------------
        drivers::usart::USART usart2(clockControl, drivers::usart::USART2, drivers::usart::USART2_Remap::U2_TX_PA2_RX_PA3);
        char test[20] = "Test UART main\n\r";
        usart2.TransmitString(test, 16);
        libs::Cout cout(usart2);
        cout<<"Test cout"<<cout.ENDL;
        std::uint32_t  x = clockControl.GetFreqAPB1();
        cout<<x<<cout.ENDL;
        cout<<clockControl.GetFreqAPB2()<<cout.ENDL;
        cout<<clockControl.GetFreqHCLK()<<cout.ENDL;


        gpioD.InitPin(drivers::port::PIN_12, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_13, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_14, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_15, drivers::port::OUTPUT);



        gpioD.SetPin(drivers::port::PIN_12, drivers::port::PIN_SET);
        gpioD.SetPin(drivers::port::PIN_13, drivers::port::PIN_SET);
        gpioD.SetPin(drivers::port::PIN_14, drivers::port::PIN_SET);
        gpioD.SetPin(drivers::port::PIN_15, drivers::port::PIN_SET);
        clockControl.mDelay(500);

        //------------------------------------------------------------------------------------
        drivers::nvic::NVIC nvic;
        nvic.NVIC_EnableIRQ(drivers::nvic::TIM6_DAC);

        drivers::timers::BasicTimers timer6(clockControl, drivers::timers::TIM6);
        timer6.EnableUpdateEvent();
        timer6.EnableInterrupt();
        //timer6.SetUpdateSource(drivers::timers::COUNTER);
        timer6.SetAutoReload(10000);
        timer6.SetPrescaler(8400);
        timer6.EnableARRPreload();
        timer6.EnableCounter();
        char str[8] = "Start\n\r";
        tim6Pointer = &timer6;
        usart2.TransmitString(str, 7);

        while (1)
        {
//            gpioD.TogglePin(drivers::port::PIN_12);
//            clockControl.mDelay(500);
//            gpioD.TogglePin(drivers::port::PIN_13);
//            clockControl.mDelay(500);
//            gpioD.TogglePin(drivers::port::PIN_14);
//            clockControl.mDelay(500);
            gpioD.TogglePin(drivers::port::PIN_15);
            clockControl.mDelay(500);
            cout<<counter<<cout.ENDL;
        }

       return 0;
    }

    void EXTI0_IRQHandler()
    {
        drivers::exti::EXIT_RTSR exitRtsr;
        exitRtsr.ClearFlag(exitRtsr.LINE_0);
        counter++;
    }

    void USART1_IRQHandler()
    {
       // bufferReceve = drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>::ReceiveData();
    }

    void OTG_FS_IRQHandler()
    {

    }

void TIM6_DAC_IRQHandler()
{
    portdRef->TogglePin(drivers::port::PIN_12);
    //libs::MWR::resetBit(0x40001010, 0);
    tim6Pointer->ClearUpdateInterruptFlag();
    counter++;
}