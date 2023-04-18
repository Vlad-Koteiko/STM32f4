//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.h"
#include "basictimer.h"
#include "spi.h"
#include "w25q16.h"
#include "systick.h"
#include "st7789.h"
#include "printf.h"
#include "ad7705.h"

std::uint32_t counter = 0;
std::uint8_t bufferReceve = 0;

//drivers::clock::ClockControl &clk;
drivers::port::GPIO *portdRef;
drivers::usart::USART *usart2Pointer;
libs::Cout *coutP;
drivers::timers::BasicTimers *tim6Pointer;
drivers::usart::USART *uart_p_2;

int my_putchar(char c)
{
    while (!usart2Pointer->IsBusyFlag())
    {}
    usart2Pointer->TransmitData(c);
    return 0;
}


    int main()
    {

        drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
        libs::MWR::write_register(0xE000ED88, 0xF00000); //разрешение FPU
        drivers::systick::SysTick sysTick(168000000, 1000);
        sysTick.Start();
        drivers::port::GPIO gpioD(clockControl, drivers::port::PORTD);
        portdRef = &gpioD;
        //Init(clockControl);
        //-----------------------------------------USART INIT---------------------------------
        drivers::usart::USART usart2(clockControl, drivers::usart::USART2,
                                     drivers::usart::USART2_Remap::U2_TX_PA2_RX_PA3);
        //       usart2.SetIDLEIE(drivers::usart::ENABLE);
        //       usart2.SetRXNEIE(drivers::usart::ENABLE);
        usart2Pointer = &usart2;


        uart_p_2 = &usart2;
        usart2.SetRXNEIE(drivers::usart::ENABLE);
        char test[20] = "Test UART main\n\r";
        usart2.TransmitString(test, 16);
        libs::Cout cout(usart2);
        coutP = &cout;
        cout << "Test cout" << cout.ENDL;

        gpioD.InitPin(drivers::port::PIN_12, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_13, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_14, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_15, drivers::port::OUTPUT);

        gpioD.SetPin(drivers::port::PIN_12, drivers::port::PIN_SET);
        gpioD.SetPin(drivers::port::PIN_13, drivers::port::PIN_SET);
        gpioD.SetPin(drivers::port::PIN_14, drivers::port::PIN_SET);
        gpioD.SetPin(drivers::port::PIN_15, drivers::port::PIN_SET);
      //  drivers::systick::SysTick::DelayMs(500);

        gpioD.InitPin(drivers::port::PIN_5, drivers::port::INPUT);
        gpioD.InitPin(drivers::port::PIN_6, drivers::port::OUTPUT);
        gpioD.InitPin(drivers::port::PIN_7, drivers::port::OUTPUT);

        //------------------------------------------------------------------------------------

        drivers::spi::SPI spi1(clockControl, drivers::spi::SPI1);
        spi1.RemapSPI1(drivers::spi::SPI1_PB5_PB4_PB3_PA15);
        //spi1.SetClockPhase(drivers::spi::EDGE1);
        spi1.SetClockPhase(drivers::spi::EDGE2);
        //spi1.SetClockPolarity(drivers::spi::LOW);
        spi1.SetClockPolarity(drivers::spi::HIGH);
        spi1.SetMode(drivers::spi::MASTER);
        spi1.SetBaudRatePrescaler(drivers::spi::DIV8);
        spi1.SetTransferBitOrder(drivers::spi::MSB_FIRST);
        spi1.SetTransferDirection(drivers::spi::FULL_DUPLEX);
        spi1.SetDataWidth(drivers::spi::BIT8);
        spi1.SetNSSMode(drivers::spi::HARD_OUTPUT);
        spi1.SetStandard(drivers::spi::MOTOROLA);
        spi1.Enable();

        devices::ad7705::Ad7705 adc(spi1, gpioD, drivers::port::PIN_6,
                                    gpioD, drivers::port::PIN_7,
                                    gpioD, drivers::port::PIN_5);


//        adc.Init(devices::ad7705::NORMAL, devices::ad7705::G2, devices::ad7705::FIN_49152_50Hz);
//        adc.WriteClock(0, 0, devices::ad7705::FIN_49152_50Hz);


        std::uint16_t reg;
        reg = adc.ReadTestReg();
        printf("TestReg : %x\n\r", reg);
        std::uint32_t buf;

        buf = adc.ReadZeroScaleCalibrReg(devices::ad7705::AIN1_PLUS_AIN1_MINUS);
        printf("Zero : %x\n\r", buf);

        drivers::timers::BasicTimers timer6(clockControl, drivers::timers::TIM6, std::chrono::milliseconds (1000), true);
//        drivers::timers::BasicTimers timer6(clockControl, drivers::timers::TIM6);
//        timer6.EnableUpdateEvent();
//        timer6.EnableInterrupt();
        //timer6.SetUpdateSource(drivers::timers::COUNTER);
//        timer6.SetAutoReload(10000);
//        timer6.SetPrescaler(8400);
//        timer6.EnableARRPreload();
//        timer6.EnableCounter();
        char str[8] = "Start\n\r";
        tim6Pointer = &timer6;
        usart2.TransmitString(str, 7);

        while (1)
        {
            gpioD.TogglePin(drivers::port::PIN_12);
            drivers::systick::SysTick::DelayMs(500);
            gpioD.TogglePin(drivers::port::PIN_13);
            drivers::systick::SysTick::DelayMs(500);
            gpioD.TogglePin(drivers::port::PIN_14);
            drivers::systick::SysTick::DelayMs(500);
            gpioD.TogglePin(drivers::port::PIN_15);
            drivers::systick::SysTick::DelayMs(500);
            sysTick.DelayMs(500);
            sysTick.DelayMs(500);

//            reg = adc.ReadData(devices::ad7705::AIN1_PLUS_AIN1_MINUS);
//            printf("Ch1 : %x\n\r", reg);
//            reg = adc.ReadData(devices::ad7705::AIN2_PLUS_AIN2_MINUS);
//            printf("Ch2 : %x\n\r", reg);
//            cout << "bufferReceve = " << bufferReceve << cout.ENDL;

            sysTick.DelayMs(500);
            cout<<counter<<cout.ENDL;
        }

       return 0;
    }

