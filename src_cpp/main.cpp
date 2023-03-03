//
// Created by koteiko_vv on 13.12.2022.
//
#include <cstdint>
#include "MWR.hpp"
#include "cout.hpp"
#include "clockControl.hpp"
#include "gpio.h"
#include "usart.h"
#include "nvic.h"
#include "syscfg.h"
#include "exti.h"
#include "array"
#include <vector>
#include <memory_resource>
#include "main.h"


    std::uint32_t counter = 0;

   /* void foo1( std::pmr::monotonic_buffer_resource &pool,libs::Cout &cout)
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
    }*/

     void EXTI0_IRQHandler(void)
     {
         drivers::exti::EXIT_RTSR exitRtsr;
         exitRtsr.ClearFlag(exitRtsr.LINE_0);
         counter++;
     }

int main()
{
    drivers::ClockControl clockControl;

    drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_A> gpio_A(clockControl);
    drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_D> gpioD(clockControl);

//    drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_2> usart(clockControl);
//    drivers::syscfg::SYSCFG syscfg(clockControl);
//    drivers::exti::EXIT_RTSR exitRtsr;
//    drivers::nvic::NVIC nvic;
//    libs::Cout cout(usart);
//
//    gpio_A.PIN_init(gpio_A.PIN_1,gpio_A.OUTPUT);
//
//    gpio_A.SetPinPull(gpio_A.PIN_0,gpio_A.NO_PULL_UP_PULL_DOWN);
//    gpio_A.PIN_init(gpio_A.PIN_0, gpio_A.INPUT);
//
//    gpio_A.USART_init(gpio_A.USART_2);
//
//    usart.uartInit(usart.RATE_115200,16000000);
//
//    syscfg.SetSourceEXTI(syscfg.PORT_A,syscfg.EXTI_0);
//    exitRtsr.SetLineTrigger(exitRtsr.LINE_0);
//
//    nvic.NVIC_SetPriority(nvic.EXTI_0, 0);
//    nvic.NVIC_Enable(nvic.EXTI_0);




    gpio_A.PIN_init(gpio_A.PIN_1, gpio_A.OUTPUT);
    gpioD.PIN_init(gpioD.PIN_12, gpioD.OUTPUT);
    gpioD.PIN_init(gpioD.PIN_13, gpioD.OUTPUT);
    gpioD.PIN_init(gpioD.PIN_14, gpioD.OUTPUT);
    gpioD.PIN_init(gpioD.PIN_15, gpioD.OUTPUT);

    gpio_A.SetPin(gpio_A.PIN_1, gpio_A.PIN_NO);
    gpioD.SetPin(gpioD.PIN_12, gpioD.PIN_NO);
    gpioD.SetPin(gpioD.PIN_13, gpioD.PIN_NO);
    gpioD.SetPin(gpioD.PIN_14, gpioD.PIN_NO);
    gpioD.SetPin(gpioD.PIN_15, gpioD.PIN_NO);


    while (true) {

        gpioD.SetPin(gpioD.PIN_12, gpioD.PIN_OFF);
        clockControl.mDelay(1000);
        gpioD.SetPin(gpioD.PIN_13, gpioD.PIN_OFF);
        clockControl.mDelay(1000);
        gpioD.SetPin(gpioD.PIN_14, gpioD.PIN_OFF);
        clockControl.mDelay(1000);
        gpioD.SetPin(gpioD.PIN_15, gpioD.PIN_OFF);
        clockControl.mDelay(1000);

        gpioD.SetPin(gpioD.PIN_12, gpioD.PIN_NO);
        clockControl.mDelay(1000);
        gpioD.SetPin(gpioD.PIN_13, gpioD.PIN_NO);
        clockControl.mDelay(1000);
        gpioD.SetPin(gpioD.PIN_14, gpioD.PIN_NO);
        clockControl.mDelay(1000);
        gpioD.SetPin(gpioD.PIN_15, gpioD.PIN_NO);
        clockControl.mDelay(1000);
    }
        /* USER CODE END WHILE */

//        volatile std::uint32_t reg_value = 0x40020000;
//
//        float resalt = (static_cast<float>(16000000)/static_cast<float>(16 * 115200));
//        std::uint16_t integet = static_cast<std::uint16_t>(resalt);
//        std::uint16_t fraction = (static_cast<float>(resalt -  static_cast<float>(integet)) * 16);
//
//        std::uint32_t reg = libs::MWR::read_register<std::uint32_t>(reg_value);
//        cout << "integet = " << integet << cout.ENDL;
//        cout << "fraction = " << fraction << cout.ENDL;
//        cout << "counter = " << counter << cout.ENDL;
//        cout << "Hello world = " << counter << cout.ENDL;
//
//        LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
//        gpio_A.SetOutputPin(gpio_A.PIN_1);
//        gpio_A.SetPin(gpio_A.PIN_1,gpio_A.PIN_NO);
//        clockControl.mDelay(1000);
//        LL_mDelay(1000);
//        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
//        gpio_A.ResetOutputPin(gpio_A.PIN_1);
//        gpio_A.SetPin(gpio_A.PIN_1,gpio_A.PIN_OFF);
//        LL_mDelay(1000);
//        clockControl.mDelay(1000);
        /* USER CODE BEGIN 3 */


   return 0;
}
