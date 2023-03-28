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

     void Init(drivers::ClockControl &clockControl)
     {
         drivers::nvic::NVIC nvic(clockControl);
         drivers::flash::Flash flash;

         flash.PrefetchBufferEnable();
         flash.DataCacheEnable();
         flash.InstructionCacheEnable();

         nvic.NVIC_Enable(nvic.EXTI_0,nvic.syscfg.PORT_A,nvic.syscfg.EXTI_0);
         nvic.NVIC_Enable(nvic.OTG_FS_IRQ);
     }


    int main()
    {
        using usart_1 =  drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>;

        drivers::ClockControl clockControl;
        drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_A> gpio_A(clockControl);
        drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_D> gpio_D(clockControl);
        usart_1 usart(clockControl,usart_1::RATE_115200,84000000);
        drivers::usb::Usb usb(clockControl);
        libs::Cout cout;


        Init(clockControl);

        //-----------------------------------------GPIO INIT----------------------------------

        gpio_A.PIN_init(gpio_A.PIN_1,gpio_A.OUTPUT);
        gpio_A.SetPinPull(gpio_A.PIN_0,gpio_A.NO_PULL_UP_PULL_DOWN);
        gpio_A.PIN_init(gpio_A.PIN_0, gpio_A.INPUT);

        gpio_D.PIN_init(gpio_D.PIN_15,gpio_D.OUTPUT);
        gpio_D.SetPinPull(gpio_D.PIN_0,gpio_D.NO_PULL_UP_PULL_DOWN);
        gpio_D.PIN_init(gpio_D.PIN_0, gpio_D.INPUT);

        //------------------------------------------------------------------------------------

        //----------------------------------------USART INIT----------------------------------

//        usart.uartInit(usart.RATE_115200,84000000);
        usart.uartEnableInterrupt();

        //------------------------------------------------------------------------------------
//        usb.Init();
//        usb.StartUSB();
        usb.MX_USB_DEVICE_Init();

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
            gpio_D.SetPin(gpio_D.PIN_15,gpio_D.PIN_NO);
            clockControl.mDelay(500);
            gpio_A.SetPin(gpio_A.PIN_1,gpio_A.PIN_OFF);
            gpio_D.SetPin(gpio_D.PIN_15,gpio_D.PIN_OFF);
            clockControl.mDelay(500);
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
