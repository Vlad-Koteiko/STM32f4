//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.h"
#include "../Drivers/IWDG/iwdg.h"

std::uint32_t counter = 0;
std::uint8_t bufferReceve = 0;

/*    void foo1( std::pmr::monotonic_buffer_resource &pool,libs::Cout &cout)
    {
        std::pmr::vector<char> vec{ &pool};

        for(char ch = 'a'; ch <= 'z'; ++ch)
        {
            vec.push_back(ch);
        }
    }

    void foo2( std::pmr::monotonic_buffer_resource &pool,libs::Cout &cout)
    {
        std::pmr::vector<std::uint8_t> vec{ &pool};

        for(std::size_t i = 0; i <= 20; i++)
        {
            vec.push_back(i);
        }
    }

     void Init(drivers::clock::ClockControl &clockControl)
     {
         drivers::nvic::NVIC nvic(clockControl);
         drivers::flash::Flash flash;

         flash.PrefetchBufferEnable();
         flash.DataCacheEnable();
         flash.InstructionCacheEnable();

         nvic.NVIC_Enable(nvic.EXTI_0,nvic.syscfg.PORT_A,nvic.syscfg.EXTI_0);
         nvic.NVIC_Enable(nvic.OTG_FS_IRQ);
     }*/


    int main()
    {
       // using usart_1 =  drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>;

        drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
        //drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_A> gpio_A(clockControl);
        drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_D> gpioD(clockControl);
        //usart_1 usart(clockControl,usart_1::RATE_115200,84000000);
        //libs::Cout cout;

        //Init(clockControl);

        //-----------------------------------------GPIO INIT----------------------------------

//        gpio_A.PIN_init(gpio_A.PIN_1,gpio_A.OUTPUT);
//        gpio_A.SetPinPull(gpio_A.PIN_0,gpio_A.NO_PULL_UP_PULL_DOWN);
//        gpio_A.PIN_init(gpio_A.PIN_0, gpio_A.INPUT);

        gpioD.PIN_init(gpioD.PIN_12, gpioD.OUTPUT);
        gpioD.PIN_init(gpioD.PIN_13, gpioD.OUTPUT);
        gpioD.PIN_init(gpioD.PIN_14, gpioD.OUTPUT);
        gpioD.PIN_init(gpioD.PIN_15, gpioD.OUTPUT);

        gpioD.SetPin(gpioD.PIN_12, gpioD.PIN_NO);
        gpioD.SetPin(gpioD.PIN_13, gpioD.PIN_NO);
        gpioD.SetPin(gpioD.PIN_14, gpioD.PIN_NO);
        gpioD.SetPin(gpioD.PIN_15, gpioD.PIN_NO);
        //------------------------------------------------------------------------------------


        while (1)
        {
            gpioD.SetPin(gpioD.PIN_12, gpioD.PIN_NO);
            clockControl.mDelay(500);
            gpioD.SetPin(gpioD.PIN_13, gpioD.PIN_NO);
            clockControl.mDelay(500);
            gpioD.SetPin(gpioD.PIN_14, gpioD.PIN_NO);
            clockControl.mDelay(500);
            gpioD.SetPin(gpioD.PIN_15, gpioD.PIN_NO);
            clockControl.mDelay(500);

            gpioD.SetPin(gpioD.PIN_12, gpioD.PIN_OFF);
            clockControl.mDelay(500);
            gpioD.SetPin(gpioD.PIN_13, gpioD.PIN_OFF);
            clockControl.mDelay(500);
            gpioD.SetPin(gpioD.PIN_14, gpioD.PIN_OFF);
            clockControl.mDelay(500);
            gpioD.SetPin(gpioD.PIN_15, gpioD.PIN_OFF);
            clockControl.mDelay(500);

            //wathdog.Refresh();
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
        bufferReceve = drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>::ReceiveData();
    }

    void OTG_FS_IRQHandler()
    {

    }
