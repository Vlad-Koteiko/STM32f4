//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.h"
#include "../Drivers/IWDG/iwdg.h"

std::uint32_t counter = 0;
std::uint8_t bufferReceve = 0;

void SetBaudRate1(std::uint32_t baudRate, std::uint32_t FPCLK, std::uint16_t &frac, std::uint16_t &integ)
{
    float resalt = (static_cast<float>(FPCLK)/static_cast<float>(16 * baudRate));
    std::uint16_t integet = static_cast<std::uint16_t>(resalt);
    std::uint16_t fraction = (static_cast<float>(resalt -  static_cast<float>(integet)) * 16);
    frac = fraction;
    integ = integet;
    //libs::MWR::write_register(baseAddress + BRR,(integet << 4) | (fraction & 0x000F));
}

    int main()
    {
       // using usart_1 =  drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>;

        drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
        libs::MWR::write_register(0xE000ED88, 0xF00000);
        //drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_A> gpio_A(clockControl);
        drivers::port::GPIO gpioD(clockControl, drivers::port::PORTD);
        //using GPIO_D = drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_D>;
        //using GPIO_A = drivers::port::GPIO<drivers::port::ADDRESSES_PORT::PORT_A>;
        //usart_1 usart(clockControl,usart_1::RATE_115200,84000000);


        //Init(clockControl);
        //-----------------------------------------USART INIT---------------------------------
        drivers::usart::USART usart2(clockControl, drivers::usart::USART2, drivers::usart::USART2_Remap::U2_TX_PA2_RX_PA3);
        char test[20] = "Test UART main\n\r";
        usart2.TransmitString(test, 16);
        libs::Cout cout(usart2);
        //libs::MWR::write_register(0x40004404, 0x61);
        cout<<"Test cout"<<cout.ENDL;
        std::uint32_t  x = clockControl.GetFreqAPB1();
        cout<<x<<cout.ENDL;
        cout<<clockControl.GetFreqAPB2()<<cout.ENDL;
        cout<<clockControl.GetFreqHCLK()<<cout.ENDL;

        float a = static_cast<float>(clockControl.GetFreqAPB2());
        float b = 1000.0;
        float c = a / b;

        std::uint32_t t = static_cast<std::uint32_t >(c);
        cout<<t<<cout.ENDL;

//        std::uint16_t f, i;
//        SetBaudRate1(115200, clockControl.GetFreqAPB1(),f, i);
//        cout<<f<<cout.ENDL;
//        cout<<i<<cout.ENDL;

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

        std::uint64_t b64 = 0xCCCCBBBBDDDDAAAA;
        std::uint32_t b32 = 0xDDDDAAAA;
        std::uint16_t b16 = 0xAAAA;
        std::uint8_t b8 = 0xBB;
        drivers::flash::Flash flash;
        flash.Unlock();
//        flash.ProgrammByte(0x800A000, b8);
//        clockControl.mDelay(500);
//        //flash.Lock();
//        flash.ProgrammHalfWord(0x800A010, b16);
//        clockControl.mDelay(500);
//        flash.ProgrammWord(0x800A020, b32);
//        clockControl.mDelay(500);
//        flash.ProgrammDoubleWord(0x800A030, b64);
//        clockControl.mDelay(500);
//
//        std::uint32_t buf = flash.ReadByte(0x800A000);
//        cout<<buf<<cout.ENDL;
//        buf = flash.ReadHalfWord(0x800A010);
//        cout<<buf<<cout.ENDL;
//        buf = flash.ReadWord(0x800A020);
//        cout<<buf<<cout.ENDL;

        flash.EraseSector(2, drivers::flash::RANGE2);
        //------------------------------------------------------------------------------------


        while (1)
        {
            gpioD.TogglePin(drivers::port::PIN_12);
            clockControl.mDelay(500);
            gpioD.TogglePin(drivers::port::PIN_13);
            clockControl.mDelay(500);

//            drivers::nvic::NVIC::NVIC_SystemReset();

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
       // bufferReceve = drivers::usart::USART<drivers::usart::ADDRESSES_USART::USART_1>::ReceiveData();
    }

    void OTG_FS_IRQHandler()
    {

    }
