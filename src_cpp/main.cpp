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

    void foo1( std::pmr::monotonic_buffer_resource &pool,libs::Cout &cout)
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

     void EXTI0_IRQHandler(void)
     {
         drivers::exti::EXIT_RTSR exitRtsr;
         exitRtsr.ClearFlag(exitRtsr.LINE_0);
         counter++;
     }

     void Init_NVIC(drivers::ClockControl &clockControl)
     {
         drivers::nvic::NVIC nvic(clockControl);
         nvic.NVIC_Enable(nvic.EXTI_0,nvic.syscfg.PORT_A,nvic.syscfg.EXTI_0);
     }

int main()
{
    drivers::ClockControl clockControl;
    drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_A> gpio_A(clockControl);
    drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_2> usart(clockControl);
    libs::Cout cout(usart);

    Init_NVIC(clockControl);

    //-----------------------------------------GPIO INIT----------------------------------

    gpio_A.PIN_init(gpio_A.PIN_1,gpio_A.OUTPUT);
    gpio_A.SetPinPull(gpio_A.PIN_0,gpio_A.NO_PULL_UP_PULL_DOWN);
    gpio_A.PIN_init(gpio_A.PIN_0, gpio_A.INPUT);
    gpio_A.USART_init(gpio_A.USART_2);

    //------------------------------------------------------------------------------------

    //----------------------------------------USART INIT----------------------------------

    usart.uartInit(usart.RATE_115200,16000000);

    //------------------------------------------------------------------------------------


//    std::array<std::uint8_t,64> buffer = {};
//    std::fill_n(std::begin(buffer),buffer.size() - 1, '_');
//
//    std::pmr::monotonic_buffer_resource pool{buffer.begin(),buffer.size()};

//    for (std::size_t i = 0; i  < buffer.size(); i++)
//    {
//        cout <<  static_cast<char>(buffer[i]);
//    }
//
//    cout << cout.ENDL;
//
//    foo1(pool,cout);
//    for (std::size_t i = 0; i  < buffer.size(); i++)
//    {
//        cout <<  static_cast<char>(buffer[i]);
//    }
//
//    cout << cout.ENDL;
//
//    foo2(pool,cout);
//
//    for (std::size_t i = 0; i  < buffer.size(); i++)
//    {
//        cout <<  static_cast<char>(buffer[i]);
//    }
//
//    cout << cout.ENDL;
//
//    foo1(pool,cout);
//
//    for (std::size_t i = 0; i  < buffer.size(); i++)
//    {
//        cout <<  static_cast<char>(buffer[i]);
//    }
//
//    cout << cout.ENDL;
//
//    foo2(pool,cout);
//
//    for (std::size_t i = 0; i  < buffer.size(); i++)
//    {
//        cout <<  static_cast<char>(buffer[i]);
//    }

//    LL_APB2_GRP1_EnableClock(1 << 14);
//    LL_APB1_GRP1_EnableClock(1 << 28);

//    SystemClock_Config();
//    SystemClock_Conf();
//    MX_GPIO_Init();
//    GPIO_INIT();

//    MX_USART2_UART_Init();

//    usart_init();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    while (1)
    {

        volatile std::uint32_t reg_value = 0x40020000;

        float resalt = (static_cast<float>(16000000)/static_cast<float>(16 * 115200));
        std::uint16_t integet = static_cast<std::uint16_t>(resalt);
        std::uint16_t fraction = (static_cast<float>(resalt -  static_cast<float>(integet)) * 16);

        std::uint32_t reg = libs::MWR::read_register<std::uint32_t>(reg_value);
        cout << "integet = " << integet << cout.ENDL;
        cout << "fraction = " << fraction << cout.ENDL;
        cout << "counter = " << counter << cout.ENDL;

        gpio_A.SetPin(gpio_A.PIN_1,gpio_A.PIN_NO);
        clockControl.mDelay(1000);
        gpio_A.SetPin(gpio_A.PIN_1,gpio_A.PIN_OFF);
        clockControl.mDelay(1000);
    }

   return 0;
}
