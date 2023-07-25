//
// Created by komiak_ap on 07.04.2023.
//

#ifndef STM32F4_ST7789_H
#define STM32F4_ST7789_H

#include "printf.h"
#include "spi.h"

// Пример инициализации SPI
/*
drivers::spi::SPI spi1(clockControl, drivers::spi::SPI1);
spi1.RemapSPI1(drivers::spi::SPI1_PB5_PB4_PB3_PA15);
spi1.SetClockPhase(drivers::spi::EDGE2);
spi1.SetClockPolarity(drivers::spi::HIGH);
spi1.SetMode(drivers::spi::MASTER);
spi1.SetBaudRatePrescaler(drivers::spi::DIV2);
spi1.SetTransferBitOrder(drivers::spi::MSB_FIRST);
spi1.SetTransferDirection(drivers::spi::FULL_DUPLEX);
spi1.SetDataWidth(drivers::spi::BIT8);
spi1.SetNSSMode(drivers::spi::HARD_OUTPUT);
spi1.SetStandard(drivers::spi::MOTOROLA);
spi1.Enable();
 */

namespace devices::st7789
{

#define ST7789_X_Start 0
#define ST7789_Y_Start 0

    enum COMMANDS : std::uint8_t
    {
        SWRESET    = 0x01,
        SLPIN      = 0x10,
        SLPOUT     = 0x11,
        COLMOD     = 0x3A,
        PTLON      = 0x12,
        NORON      = 0x13,
        INVOFF     = 0x20,
        INVON      = 0x21,
        GAMSET     = 0x26,
        DISPOFF    = 0x28,
        DISPON     = 0x29,
        CASET      = 0x2A,
        RASET      = 0x2B,
        RAMWR      = 0x2C,
        PTLAR      = 0x30,
        MADCTL     = 0x36,
        MADCTL_MY  = 0x80,
        MADCTL_MX  = 0x40,
        MADCTL_MV  = 0x20,
        MADCTL_ML  = 0x10,
        MADCTL_RGB = 0x00,
        RDID1      = 0xDA,
        RDID2      = 0xDB,
        RDID3      = 0xDC,
        RDID4      = 0xDD
    };

    enum COLORS : std::uint16_t
    {
        BLACK   = 0x0000,
        BLUE    = 0x001F,
        RED     = 0xF800,
        GREEN   = 0x07E0,
        CYAN    = 0x07FF,
        MAGENTA = 0xF81F,
        YELLOW  = 0xFFE0,
        WHITE   = 0xFFFF
    };

    enum COLOR_MODE : std::uint8_t
    {
        _65K   = 0x50,
        _262K  = 0x60,
        _12bit = 0x03,
        _16bit = 0x05,
        _18bit = 0x06,
        _16M   = 0x07
    };

    enum MADCTL : std::uint8_t
    {
        ST7789_MADCTL_MY  = 0x80,
        ST7789_MADCTL_MX  = 0x40,
        ST7789_MADCTL_MV  = 0x20,
        ST7789_MADCTL_ML  = 0x10,
        ST7789_MADCTL_BGR = 0x08,
        ST7789_MADCTL_MH  = 0x04
    };

    class St7789
    {
        drivers::spi::SPI        &spiScr;
        drivers::port::Gpio      &port;
        drivers::port::PIN_NUMBER DC;
        drivers::port::PIN_NUMBER RES;
        drivers::port::PIN_NUMBER BCK;
        std::uint16_t             stWidth;
        std::int16_t              stHeight;

    public:
        St7789(drivers::spi::SPI        &spiScreen,
               drivers::port::Gpio      &port,
               drivers::port::PIN_NUMBER dc,
               drivers::port::PIN_NUMBER res,
               drivers::port::PIN_NUMBER bck,
               std::uint16_t             width,
               std::uint16_t             height);

        void HardReset() noexcept;
        void SoftReset() noexcept;
        void SendCmd(std::uint8_t cmd) noexcept;
        void SendData(std::uint8_t data) noexcept;
        void SleepModeEnter() noexcept;
        void SleepModeExit() noexcept;
        void ColorModeSet(std::uint8_t colorMode) noexcept;
        void MemAccessModeSet(std::uint8_t rotation,
                              std::uint8_t vertMirror,
                              std::uint8_t horizMirror,
                              std::uint8_t isBgr) noexcept;
        void InversionMode(std::uint8_t mode) noexcept;
        void FillScreen(std::uint16_t color) noexcept;
        void SetBL(std::uint8_t value) noexcept;
        void DisplayPower(std::uint8_t value) noexcept;

        void FillRect(std::uint16_t x,
                      std::uint16_t y,
                      std::uint16_t w,
                      std::uint16_t h,
                      std::uint16_t color) noexcept;
        void SetWindow(std::uint8_t x0, std::uint8_t y0, std::uint8_t x1, std::uint8_t y1) noexcept;
        void RamWrite(std::uint16_t *pBuff, std::uint16_t Len) noexcept;
        void ColumnSet(std::uint16_t columnStart, std::uint16_t columnEnd) noexcept;
        void RowSet(std::uint16_t rowStart, std::uint16_t rowEnd) noexcept;

        void DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) noexcept;
        void DrawRectangleFilled(int16_t  x1,
                                 int16_t  y1,
                                 int16_t  x2,
                                 int16_t  y2,
                                 uint16_t fillcolor) noexcept;
        void DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) noexcept;
        void SwapInt16Values(int16_t *pValue1, int16_t *pValue2) noexcept;
        void DrawLineSlow(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) noexcept;
        void DrawPixel(int16_t x, int16_t y, uint16_t color) noexcept;
        void DrawCircleFilled(int16_t x0, int16_t y0, int16_t radius, uint16_t fillcolor) noexcept;
        void DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) noexcept;
        void PrintChar(int16_t x, int16_t y, uint16_t color, uint8_t character) noexcept;
        void PrintString(int16_t  x,
                         int16_t  y,
                         uint16_t color,
                         uint8_t *string,
                         uint16_t count) noexcept;
    };
}    // namespace devices::st7789

#endif    // STM32F4_ST7789_H
