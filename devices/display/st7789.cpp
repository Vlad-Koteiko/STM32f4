//
// Created by komiak_ap on 07.04.2023.
//

#include "st7789.h"

#include <cmath>

#include "font_info.h"
#include "systick.h"

extern const std::uint8_t   consolas_9ptBitmaps[];
extern const FONT_CHAR_INFO consolas_9ptDescriptors[];
extern const FONT_INFO      consolas_9ptFontInfo;

namespace devices::st7789
{
    St7789::St7789(drivers::spi::SPI        &spiScreen,
                   drivers::port::GPIO      &port,
                   drivers::port::PIN_NUMBER dc,
                   drivers::port::PIN_NUMBER res,
                   drivers::port::PIN_NUMBER bck,
                   std::uint16_t             width,
                   std::uint16_t             height) :
        spiScr(spiScreen), port(port), DC(dc), RES(res), BCK(bck)
    {
        stWidth  = width;
        stHeight = height;

        HardReset();
        SoftReset();
        SleepModeExit();

        ColorModeSet(_65K | _16bit);
        drivers::systick::SysTick::DelayMs(10);
        MemAccessModeSet(4, 1, 1, 0);
        drivers::systick::SysTick::DelayMs(10);
        InversionMode(1);
        drivers::systick::SysTick::DelayMs(10);
        FillScreen(0);
        SetBL(100);
        DisplayPower(1);
        drivers::systick::SysTick::DelayMs(100);
    }

    void St7789::HardReset() noexcept
    {
        port.SetPin(RES, drivers::port::PIN_RESET);
        drivers::systick::SysTick::DelayMs(10);
        port.SetPin(RES, drivers::port::PIN_SET);
        drivers::systick::SysTick::DelayMs(150);
    }

    void St7789::SoftReset() noexcept
    {
        SendCmd(SWRESET);
        drivers::systick::SysTick::DelayMs(130);
    }

    void St7789::SendCmd(std::uint8_t cmd) noexcept
    {
        port.SetPin(DC, drivers::port::PIN_RESET);
        spiScr.TransmitData8(cmd);
    }

    void St7789::SendData(std::uint8_t data) noexcept
    {
        port.SetPin(DC, drivers::port::PIN_SET);
        spiScr.TransmitData8(data);
    }

    void St7789::SleepModeEnter() noexcept
    {
        SendCmd(SLPIN);
        drivers::systick::SysTick::DelayMs(500);
    }

    void St7789::SleepModeExit() noexcept
    {
        SendCmd(SLPOUT);
        drivers::systick::SysTick::DelayMs(500);
    }

    void St7789::ColorModeSet(std::uint8_t colorMode) noexcept
    {
        SendCmd(COLMOD);
        SendData(colorMode & 0x77);
    }

    void St7789::MemAccessModeSet(std::uint8_t rotation,
                                  std::uint8_t vertMirror,
                                  std::uint8_t horizMirror,
                                  std::uint8_t isBgr) noexcept
    {
        std::uint8_t value;
        rotation &= 0x7;

        SendCmd(MADCTL);
        switch(rotation)
        {
            case 0:
                value = 0;
                break;
            case 1:
                value = ST7789_MADCTL_MX;
                break;
            case 2:
                value = ST7789_MADCTL_MY;
                break;
            case 3:
                value = ST7789_MADCTL_MX | ST7789_MADCTL_MY;
                break;
            case 4:
                value = ST7789_MADCTL_MV;
                break;
            case 5:
                value = ST7789_MADCTL_MV | ST7789_MADCTL_MX;
                break;
            case 6:
                value = ST7789_MADCTL_MV | ST7789_MADCTL_MY;
                break;
            case 7:
                value = ST7789_MADCTL_MV | ST7789_MADCTL_MX | ST7789_MADCTL_MY;
                break;
        }

        if(vertMirror)
            value = ST7789_MADCTL_ML;
        if(horizMirror)
            value = ST7789_MADCTL_MH;

        if(isBgr)
            value |= ST7789_MADCTL_BGR;

        SendData(value);
    }

    void St7789::InversionMode(std::uint8_t mode) noexcept
    {
        if(mode)
            SendCmd(INVON);
        else
            SendCmd(INVOFF);
    }

    void St7789::FillScreen(std::uint16_t color) noexcept
    {
        FillRect(0, 0, stWidth, stHeight, color);
    }

    void St7789::FillRect(std::uint16_t x,
                          std::uint16_t y,
                          std::uint16_t w,
                          std::uint16_t h,
                          std::uint16_t color) noexcept
    {
        if((x >= stWidth) || (y >= stHeight))
            return;
        if((x + w) > stWidth)
            w = stWidth - x;
        if((y + h) > stHeight)
            h = stHeight - y;
        SetWindow(x, y, x + w - 1, y + h - 1);
        for(uint32_t i = 0; i < (h * w); i++) RamWrite(&color, 1);
    }

    void St7789::SetWindow(std::uint8_t x0,
                           std::uint8_t y0,
                           std::uint8_t x1,
                           std::uint8_t y1) noexcept
    {
        ColumnSet(x0, x1);
        RowSet(y0, y1);
        SendCmd(RAMWR);
    }

    void St7789::ColumnSet(std::uint16_t columnStart, std::uint16_t columnEnd) noexcept
    {
        if(columnStart > columnEnd)
            return;
        if(columnEnd > stWidth)
            return;

        columnStart += ST7789_X_Start;
        columnEnd += ST7789_X_Start;

        SendCmd(CASET);
        SendData(columnStart >> 8);
        SendData(columnStart & 0xFF);
        SendData(columnEnd >> 8);
        SendData(columnEnd & 0xFF);
    }

    void St7789::RowSet(std::uint16_t rowStart, std::uint16_t rowEnd) noexcept
    {
        if(rowStart > rowEnd)
            return;
        if(rowEnd > stHeight)
            return;

        rowStart += ST7789_Y_Start;
        rowEnd += ST7789_Y_Start;

        SendCmd(RASET);
        SendData(rowStart >> 8);
        SendData(rowStart & 0xFF);
        SendData(rowEnd >> 8);
        SendData(rowEnd & 0xFF);
    }

    void St7789::RamWrite(uint16_t *pBuff, uint16_t Len) noexcept
    {
        while(Len--)
        {
            SendData(*pBuff >> 8);
            SendData(*pBuff & 0xFF);
        }
    }

    void St7789::SetBL(std::uint8_t value) noexcept
    {
        if(value > 100)
            value = 100;

        if(value)
            port.SetPin(BCK, drivers::port::PIN_SET);
        else
            port.SetPin(BCK, drivers::port::PIN_RESET);
    }

    void St7789::DisplayPower(std::uint8_t value) noexcept
    {
        if(value)
            SendCmd(DISPON);
        else
            SendCmd(DISPOFF);
    }

    void St7789::DrawRectangle(int16_t  x1,
                               int16_t  y1,
                               int16_t  x2,
                               int16_t  y2,
                               uint16_t color) noexcept
    {
        DrawLine(x1, y1, x1, y2, color);
        DrawLine(x2, y1, x2, y2, color);
        DrawLine(x1, y1, x2, y1, color);
        DrawLine(x1, y2, x2, y2, color);
    }

    void St7789::DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) noexcept
    {
        // Вертикальная линия
        if(x1 == x2)
        {
            // Отрисовываем линию быстрым методом
            if(y1 > y2)
                FillRect(x1, y2, 1, y1 - y2 + 1, color);
            else
                FillRect(x1, y1, 1, y2 - y1 + 1, color);
            return;
        }

        // Горизонтальная линия
        if(y1 == y2)
        {
            // Отрисовываем линию быстрым методом
            if(x1 > x2)
                FillRect(x2, y1, x1 - x2 + 1, 1, color);
            else
                FillRect(x1, y1, x2 - x1 + 1, 1, color);
            return;
        }

        DrawLineSlow(x1, y1, x2, y2, color);
    }

    void St7789::DrawRectangleFilled(int16_t  x1,
                                     int16_t  y1,
                                     int16_t  x2,
                                     int16_t  y2,
                                     uint16_t fillcolor) noexcept
    {
        if(x1 > x2)
            SwapInt16Values(&x1, &x2);
        if(y1 > y2)
            SwapInt16Values(&y1, &y2);
        FillRect(x1, y1, x2 - x1, y2 - y1, fillcolor);
    }

    void St7789::SwapInt16Values(int16_t *pValue1, int16_t *pValue2) noexcept
    {
        int16_t TempValue = *pValue1;
        *pValue1          = *pValue2;
        *pValue2          = TempValue;
    }

    void St7789::DrawLineSlow(int16_t  x1,
                              int16_t  y1,
                              int16_t  x2,
                              int16_t  y2,
                              uint16_t color) noexcept
    {
        const int16_t deltaX = abs(x2 - x1);
        const int16_t deltaY = abs(y2 - y1);
        const int16_t signX  = x1 < x2 ? 1 : -1;
        const int16_t signY  = y1 < y2 ? 1 : -1;

        int16_t error = deltaX - deltaY;

        DrawPixel(x2, y2, color);

        while(x1 != x2 || y1 != y2)
        {
            DrawPixel(x1, y1, color);
            const int16_t error2 = error * 2;

            if(error2 > -deltaY)
            {
                error -= deltaY;
                x1 += signX;
            }
            if(error2 < deltaX)
            {
                error += deltaX;
                y1 += signY;
            }
        }
    }

    void St7789::DrawPixel(int16_t x, int16_t y, uint16_t color) noexcept
    {
        if((x < 0) || (x >= stWidth) || (y < 0) || (y >= stHeight))
            return;

        SetWindow(x, y, x, y);
        RamWrite(&color, 1);
    }

    void St7789::DrawCircleFilled(int16_t  x0,
                                  int16_t  y0,
                                  int16_t  radius,
                                  uint16_t fillcolor) noexcept
    {
        int x     = 0;
        int y     = radius;
        int delta = 1 - 2 * radius;
        int error = 0;

        while(y >= 0)
        {
            DrawLine(x0 + x, y0 - y, x0 + x, y0 + y, fillcolor);
            DrawLine(x0 - x, y0 - y, x0 - x, y0 + y, fillcolor);
            error = 2 * (delta + y) - 1;

            if(delta < 0 && error <= 0)
            {
                ++x;
                delta += 2 * x + 1;
                continue;
            }

            error = 2 * (delta - x) - 1;

            if(delta > 0 && error > 0)
            {
                --y;
                delta += 1 - 2 * y;
                continue;
            }

            ++x;
            delta += 2 * (x - y);
            --y;
        }
    }

    void St7789::DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) noexcept
    {
        int x     = 0;
        int y     = radius;
        int delta = 1 - 2 * radius;
        int error = 0;

        while(y >= 0)
        {
            DrawPixel(x0 + x, y0 + y, color);
            DrawPixel(x0 + x, y0 - y, color);
            DrawPixel(x0 - x, y0 + y, color);
            DrawPixel(x0 - x, y0 - y, color);
            error = 2 * (delta + y) - 1;

            if(delta < 0 && error <= 0)
            {
                ++x;
                delta += 2 * x + 1;
                continue;
            }

            error = 2 * (delta - x) - 1;

            if(delta > 0 && error > 0)
            {
                --y;
                delta += 1 - 2 * y;
                continue;
            }

            ++x;
            delta += 2 * (x - y);
            --y;
        }
    }

    void St7789::PrintChar(int16_t x, int16_t y, uint16_t color, uint8_t character) noexcept
    {
        if((consolas_9ptFontInfo.start_character <= character) &&
           (character <= consolas_9ptFontInfo.end_character))
        {
            uint16_t pozCharacter = character - consolas_9ptFontInfo.start_character;
            //            printf("Character : %d \n\r", character);
            //            printf("Poz char: %d\n\r", pozCharacter);
            //            printf("start %d \n\r", consolas_9ptFontInfo.start_character);
            //            printf("end %d \n\r", consolas_9ptFontInfo.end_character);
            uint8_t widthInBytes = consolas_9ptDescriptors[pozCharacter].width;
            //            uint8_t heightInBits = consolas_9ptDescriptors[pozCharacter].height;
            uint16_t offset     = consolas_9ptDescriptors[pozCharacter].offset;
            uint16_t offsetNext = consolas_9ptDescriptors[pozCharacter + 1].offset;

            //            printf("Offset %d\n\r", offset);
            //            printf("Offset next %d", offsetNext);

            uint8_t ySeek = y;
            uint8_t xSeek = x;
            uint8_t bytePaint;

            for(int i = offset; i < offsetNext; i++)
            {
                bytePaint = consolas_9ptBitmaps[i];
                // printf("Byte paint %d %X\n\r", i, bytePaint);

                for(int i = 7; i >= 0; i--)
                {
                    if(((bytePaint >> i) & 0x01) == 0x01)
                        DrawPixel(xSeek, ySeek, color);
                    xSeek++;
                }

                if(i % widthInBytes == 0)
                {
                    ySeek++;
                    xSeek = x;
                }
            }
        }
        else
        {
        }
    }

    void St7789::PrintString(int16_t  x,
                             int16_t  y,
                             uint16_t color,
                             uint8_t *string,
                             uint16_t count) noexcept
    {
        for(int i = 0; i < count; i++)
        {
            if(x >= stWidth || y >= stHeight)
                return;

            uint16_t pozCharacter = string[i] - consolas_9ptFontInfo.start_character;
            //  printf("Poz char %d\n\r", pozCharacter);
            std::uint16_t width = consolas_9ptDescriptors[pozCharacter].width;
            PrintChar(x, y, color, string[i]);
            x = x + width * 8;
            //  printf("X Y : %d %d\n\r", x, y );
        }
    }
}    // namespace devices::st7789