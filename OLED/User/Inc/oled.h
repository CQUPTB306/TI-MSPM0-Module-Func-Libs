#ifndef __OLED_H__
#define __OLED_H__

#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define OLED_ADDR 0x3C

class OLED {
    public:
        OLED(uint8_t height = 64, I2C_Regs* I2C_PORT = I2C_0_INST);
        bool begin();
        void clear();
        void clearPart(uint8_t x1, uint8_t page1, uint8_t x2, uint8_t page2);
        void setCursor(uint8_t x, uint8_t y);
        void updateBufferArea();

        void setPixel(uint8_t x, uint8_t y, bool state);
        void printText(uint8_t x, uint8_t y, const char* str, uint8_t size);
        void printImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t* image);
        void printVar(uint8_t x, uint8_t y, float num, const char* type, uint8_t length, bool zeroPad);
        void printHLine(uint8_t x, uint8_t y, uint8_t width);
        // void printVLine(uint8_t x, uint8_t y, uint8_t height);

        void lowBrightness(bool enable);
        void power(bool state);

    private:
        void writeCommand(uint8_t command);
        void writeData(uint8_t data);
        uint8_t height_;
        I2C_Regs* I2C_PORT_;
};

#endif
