//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.h"
#include "../Drivers/IWDG/iwdg.h"

std::uint32_t counter = 0;
std::uint8_t bufferReceve = 0;

    int main()
    {
       // using usart_1 =  drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>;

        const drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
        //drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_A> gpio_A(clockControl);
        drivers::port::GPIO gpioD(clockControl, drivers::port::PORT_D);
        //using GPIO_D = drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_D>;
        //using GPIO_A = drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_A>;
        //usart_1 usart(clockControl,usart_1::RATE_115200,84000000);
        //libs::Cout cout;

        //Init(clockControl);

        //-----------------------------------------GPIO INIT----------------------------------

//        gpio_A.PIN_init(gpio_A.PIN_1,gpio_A.OUTPUT);
//        gpio_A.SetPinPull(gpio_A.PIN_0,gpio_A.NO_PULL_UP_PULL_DOWN);
//        gpio_A.PIN_init(gpio_A.PIN_0, gpio_A.INPUT);

        gpioD.InitPin(drivers::port::PIN_12, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_13, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_14, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_15, drivers::port::OUTPUT);

//        gpio_A.InitPin(GPIO_A::PIN_12, GPIO_A::OUTPUT);
//        gpio_A.InitPin(GPIO_A::PIN_13, GPIO_A::OUTPUT);
//        gpio_A.InitPin(GPIO_A::PIN_14, GPIO_A::OUTPUT);
//        gpio_A.InitPin(GPIO_A::PIN_15, GPIO_A::OUTPUT);

        gpioD.SetPin(drivers::port::PIN_12, drivers::port::PIN_SET);
        gpioD.SetPin(drivers::port::PIN_13, drivers::port::PIN_SET);
        gpioD.SetPin(drivers::port::PIN_14, drivers::port::PIN_SET);
        gpioD.SetPin(drivers::port::PIN_15, drivers::port::PIN_SET);
        clockControl.mDelay(500);
        //------------------------------------------------------------------------------------


        while (1)
        {
//            gpioD.SetPin(GPIO_D::PIN_12, GPIO_D::PIN_SET);
//            clockControl.mDelay(500);
//            gpioD.SetPin(GPIO_D::PIN_13, GPIO_D::PIN_SET);
//            clockControl.mDelay(500);
//            gpioD.SetPin(GPIO_D::PIN_14, GPIO_D::PIN_SET);
//            clockControl.mDelay(500);
//            gpioD.SetPin(GPIO_D::PIN_15, GPIO_D::PIN_SET);
//            clockControl.mDelay(500);
//
//            gpioD.SetPin(GPIO_D::PIN_12, GPIO_D::PIN_RESET);
//            clockControl.mDelay(500);
//            gpioD.SetPin(GPIO_D::PIN_13, GPIO_D::PIN_RESET);
//            clockControl.mDelay(500);
//            gpioD.SetPin(GPIO_D::PIN_14, GPIO_D::PIN_RESET);
//            clockControl.mDelay(500);
//            gpioD.SetPin(GPIO_D::PIN_15, GPIO_D::PIN_RESET);
//            clockControl.mDelay(500);

            gpioD.TogglePin(drivers::port::PIN_12);
            clockControl.mDelay(500);
            gpioD.TogglePin(drivers::port::PIN_13);
            clockControl.mDelay(500);
            gpioD.TogglePin(drivers::port::PIN_14);
            clockControl.mDelay(500);
            gpioD.TogglePin(drivers::port::PIN_15);
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
