//
// Created by koteiko_vv on 13.12.2022.
//

#include "main.hpp"

#include "etl/scheduler.h"
#include "etl/string.h"
#include "etl/task.h"
#include "etl/to_string.h"
#include "etl/vector.h"
#include "etl_profile.h"
#include "string.h"

drivers::usart::Usart* usartGlobal;

class Task1 : public etl::task
{
private:
    drivers::usart::Usart* usartPtr;

public:
    Task1(uint8_t prior, drivers::usart::Usart* usart) : task(prior)
    {
        usartPtr = usart;
    }

    std::uint32_t task_request_work() const
    {
        return 1;
    }

    void task_process_work()
    {
        char strSend[] = "task 1 work\n\r";
        usartPtr->TransmitString(strSend, 13);
    }
};

class Task2 : public etl::task
{
private:
    drivers::usart::Usart* usartPtr;

public:
    Task2(uint8_t prior, drivers::usart::Usart* usart) : task(prior)
    {
        usartPtr = usart;
    }

    std::uint32_t task_request_work() const
    {
        return 1;
    }

    void task_process_work()
    {
        char strSend[] = "task 2 work\n\r";
        usartPtr->TransmitString(strSend, 13);
    }
};

class Idle
{
public:
    //*************************************
    Idle(etl::ischeduler& scheduler_) : scheduler(scheduler_)
    {}

    //*************************************
    void IdleCallback()
    {
        char str1[] = "Idle callback\n\r";
        usartGlobal->TransmitString(str1, 15), scheduler.exit_scheduler();
        char str2[] = "Exiting the scheduler\n\r";
        usartGlobal->TransmitString(str1, 23);
    }

private:
    etl::ischeduler& scheduler;
};

void WatchdogCallback()
{
    char str1[] = "Watchdog callback\n\r";
    usartGlobal->TransmitString(str1, 19);
}

class Scheduler : public etl::scheduler<etl::scheduler_policy_sequential_single, 2>
{};

int main() noexcept
{
    constexpr drivers::clock::ClockControl clockControl(drivers::clock::FREQ_168000000);
    clockControl.init();
    drivers::usart::Usart usart2(clockControl, drivers::usart::USART2);
    usart2.init();
    usartGlobal = &usart2;

    std::uint8_t string[] = "Start STM32F407\n\r";

    // etl::vector data { 0, 1, 2, 70, 70, 5, 6, 7, 8, 9 };

    // etl::string<100> str;
    // // etl::to_string<etl::string>(data, str);
    // // usart2.TransmitString(&str[0], 10);

    // //    usart2.sendByte(static_cast<std::byte>(data[4]));

    // Task1 task1(1, &usart2);
    // Task2 task2(1, &usart2);

    // Scheduler scheduler;

    // Idle idleHandler(scheduler);

    // etl::function_mv<Idle, &Idle::IdleCallback> idleCallback(
    //     idleHandler);        // Member function, no parameters, returning void.
    // etl::function_fv<WatchdogCallback>
    //     watchdogCallback;    // Global function, no parameters, returning void.

    // scheduler.add_task(task1);
    // scheduler.add_task(task2);
    // scheduler.set_idle_callback(idleCallback);
    // scheduler.set_watchdog_callback(watchdogCallback);

    // scheduler.start();

    while(1)
    {
        usart2.TransmitString(string, sizeof(string));

        clockControl.mDelay(1000);
    }
}
