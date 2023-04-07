//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.h"
#include "basictimer.h"
#include "spi.h"
#include "w25q16.h"
#include "systick.h"

std::uint32_t counter = 0;
std::uint8_t bufferReceve = 0;

//drivers::clock::ClockControl &clk;
drivers::port::GPIO *portdRef;
drivers::timers::BasicTimers *tim6Pointer;
drivers::usart::USART *usart2Pointer;
libs::Cout *coutP;

    int main() {
        drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
        libs::MWR::write_register(0xE000ED88, 0xF00000); //разрешение FPU
        drivers::systick::SysTick sysTick(168000000, 1000);
//        sysTick.Start();
//        drivers::port::GPIO gpioD(clockControl, drivers::port::PORTD);
//        portdRef = &gpioD;

        //Init(clockControl);
        //-----------------------------------------USART INIT---------------------------------
        drivers::usart::USART usart2(clockControl, drivers::usart::USART2,
                                     drivers::usart::USART2_Remap::U2_TX_PA2_RX_PA3);
        //       usart2.SetIDLEIE(drivers::usart::ENABLE);
        //       usart2.SetRXNEIE(drivers::usart::ENABLE);
//        usart2Pointer = &usart2;

        char test[20] = "Test UART main\n\r";
        usart2.TransmitString(test, 16);
        libs::Cout cout(usart2);
        coutP = &cout;
        cout << "Test cout" << cout.ENDL;
        std::uint32_t x = clockControl.GetFreqAPB1();
        cout << x << cout.ENDL;
        cout << clockControl.GetFreqAPB2() << cout.ENDL;
        cout << clockControl.GetFreqHCLK() << cout.ENDL;


//        gpioD.InitPin(drivers::port::PIN_12, drivers::port::OUTPUT);
//        gpioD.InitPin(drivers::port::PIN_13, drivers::port::OUTPUT);
//        gpioD.InitPin(drivers::port::PIN_14, drivers::port::OUTPUT);
//        gpioD.InitPin(drivers::port::PIN_15, drivers::port::OUTPUT);
//
//
//
//        gpioD.SetPin(drivers::port::PIN_12, drivers::port::PIN_SET);
//        gpioD.SetPin(drivers::port::PIN_13, drivers::port::PIN_SET);
//        gpioD.SetPin(drivers::port::PIN_14, drivers::port::PIN_SET);
//        gpioD.SetPin(drivers::port::PIN_15, drivers::port::PIN_SET);
//        clockControl.mDelay(500);

        //------------------------------------------------------------------------------------
//        drivers::nvic::NVIC nvic;
//        nvic.NVIC_EnableIRQ(drivers::nvic::TIM6_DAC);
//        nvic.NVIC_EnableIRQ(drivers::nvic::USART2);

//        drivers::timers::BasicTimers timer6(clockControl, drivers::timers::TIM6, std::chrono::milliseconds (1000), true);
//        drivers::timers::BasicTimers timer6(clockControl, drivers::timers::TIM6);
//        timer6.EnableUpdateEvent();
//        timer6.EnableInterrupt();
//        timer6.SetAutoReload(10000);
//        timer6.SetPrescaler(8400);
//        timer6.EnableARRPreload();
        //timer6.EnableCounter();
//        tim6Pointer = &timer6;

//        char str[8] = "Start\n\r";
//
//        usart2.TransmitString(str, 7);

        drivers::spi::SPI spi1(clockControl, drivers::spi::SPI1);
        spi1.RemapSPI1(drivers::spi::SPI1_PB5_PB4_PB3_PA15);
        spi1.SetClockPhase(drivers::spi::EDGE1);
        spi1.SetClockPolarity(drivers::spi::LOW);
        spi1.SetMode(drivers::spi::MASTER);
        spi1.SetBaudRatePrescaler(drivers::spi::DIV256);
        spi1.SetTransferBitOrder(drivers::spi::MSB_FIRST);
        spi1.SetTransferDirection(drivers::spi::FULL_DUPLEX);
        spi1.SetDataWidth(drivers::spi::BIT8);
        spi1.SetNSSMode(drivers::spi::HARD_OUTPUT);
        spi1.SetStandard(drivers::spi::MOTOROLA);
        spi1.Enable();

        drivers::port::GPIO portA(clockControl, drivers::port::PORTA);
        portA.InitPin(drivers::port::PIN_15, drivers::port::OUTPUT);
        portA.InitPin(drivers::port::PIN_10, drivers::port::OUTPUT);
        portA.InitPin(drivers::port::PIN_1, drivers::port::OUTPUT);
        portA.SetPin(drivers::port::PIN_15, drivers::port::PIN_SET);

        libs::flash::W25Q16 flashW25Q16(spi1, portA, drivers::port::PIN_15);


        std::uint32_t id = flashW25Q16.ReadID();
        cout << "ID " << id << cout.ENDL;

        cout<<flashW25Q16.ReadStatusRegister1()<<cout.ENDL;



        std::uint8_t testArray[16];
        std::uint8_t testReadArray[16];

        cout<<"Test array: ";
        for (int i = 0; i < 16; i++) {
            testArray[i] = i;
            cout<<testArray[i]<<" ";
        }
        cout<<cout.ENDL;

        cout<<"Read array 1: ";
        flashW25Q16.ReadBytes(0x00, testReadArray, 16);
        for(int i = 0; i < 16; i++) {
            cout << testReadArray[i] << " ";
        }
        cout<<cout.ENDL;

        flashW25Q16.WriteBytes(0x00, testArray, 16);


       cout<<"Read array 2: ";
        flashW25Q16.ReadBytes(0x00, testReadArray, 16);
        for(int i = 0; i < 16; i++) {
            cout << testReadArray[i] << " ";
        }
        cout<<cout.ENDL;

        flashW25Q16.EraseSector(0);

        while (1)
        {
//            gpioD.TogglePin(drivers::port::PIN_12);
//            clockControl.mDelay(500);
//            gpioD.TogglePin(drivers::port::PIN_13);
//            clockControl.mDelay(500);
//            gpioD.TogglePin(drivers::port::PIN_14);
//            clockControl.mDelay(500);
//            gpioD.TogglePin(drivers::port::PIN_15);
            //clockControl.mDelay(500);
            sysTick.DelayMs(500);
            portA.TogglePin(drivers::port::PIN_10);
            portA.TogglePin(drivers::port::PIN_1);
            sysTick.DelayMs(500);
        }

       return 0;
    }

    void EXTI0_IRQHandler()
    {
        drivers::exti::EXIT_RTSR exitRtsr;
        exitRtsr.ClearFlag(exitRtsr.LINE_0);
        counter++;
    }

    void USART2_IRQHandler()
    {
        if(usart2Pointer->ReadFlag_IDLE())
        {
            coutP->operator<<("IDLE inter\n\r")<<coutP->ENDL;
            usart2Pointer->SetIDLEIE(drivers::usart::DISABLE);
        }
        if(usart2Pointer->ReadFlag_RXNE())
        {
            coutP->operator<<("RXNE ");
            coutP->operator<<((std::uint8_t)usart2Pointer->ReadFlag_RXNE());
            coutP->operator<<(" IDLE ");
            coutP->operator<<((std::uint8_t)usart2Pointer->ReadFlag_IDLE());
            coutP->operator<<("\n\r");
            usart2Pointer->ClearFlag_RXNE();
            char str[] = "IDLE: ";
            usart2Pointer->ReceiveData();
            usart2Pointer->TransmitString(str, 6);
            coutP->operator<<((std::uint8_t)usart2Pointer->ReadFlag_IDLE());
            coutP->operator<<("\n\r");
        }
    }


void TIM6_DAC_IRQHandler()
{
    portdRef->TogglePin(drivers::port::PIN_12);
    //libs::MWR::resetBit(0x40001010, 0);
    tim6Pointer->ClearUpdateInterruptFlag();
    counter++;
}