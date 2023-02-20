//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.h"

std::uint32_t counter = 0;
std::uint8_t bufferReceve = 0;

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

     void Init_NVIC(drivers::ClockControl &clockControl)
     {
         drivers::nvic::NVIC nvic(clockControl);
         nvic.NVIC_Enable(nvic.EXTI_0,nvic.syscfg.PORT_A,nvic.syscfg.EXTI_0);
     }


    int main()
    {
        using usart_1 =  drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>;

        drivers::ClockControl clockControl;
        drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_A> gpio_A(clockControl);
        usart_1 usart(clockControl,usart_1::RATE_115200,84000000);
        libs::Cout cout;

        Init_NVIC(clockControl);

        //-----------------------------------------GPIO INIT----------------------------------

        gpio_A.PIN_init(gpio_A.PIN_1,gpio_A.OUTPUT);
        gpio_A.SetPinPull(gpio_A.PIN_0,gpio_A.NO_PULL_UP_PULL_DOWN);
        gpio_A.PIN_init(gpio_A.PIN_0, gpio_A.INPUT);

        //------------------------------------------------------------------------------------

        //----------------------------------------USART INIT----------------------------------

//        usart.uartInit(usart.RATE_115200,84000000);
        usart.uartEnableInterrupt();

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

        char test[20] = "Test UART main\n\r";

        while (1)
        {
            cout << "counter = " << counter << cout.ENDL;
            cout << "USART = " << bufferReceve << cout.ENDL;

            usart.TransmitString(test,20);

            gpio_A.SetPin(gpio_A.PIN_1,gpio_A.PIN_NO);
            clockControl.mDelay(1000);
            gpio_A.SetPin(gpio_A.PIN_1,gpio_A.PIN_OFF);
            clockControl.mDelay(1000);
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



