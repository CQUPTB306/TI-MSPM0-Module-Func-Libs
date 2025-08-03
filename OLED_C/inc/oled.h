#ifndef __OLED_H__
#define __OLED_H__

#include "ti_msp_dl_config.h"

#define OLED_ADDR 0x3C

typedef struct {
    uint8_t height_;
    I2C_Regs* I2C_PORT_;
} OLED_t;

void OLED_Init(OLED_t* oled, uint8_t height, I2C_Regs* I2C_PORT);
bool OLED_Begin(OLED_t* oled);
void OLED_Clear(OLED_t* oled);
void OLED_ClearPart(OLED_t* oled, uint8_t x1, uint8_t page1, uint8_t x2, uint8_t page2);
void OLED_SetCursor(OLED_t* oled, uint8_t x, uint8_t y);
void OLED_UpdateBufferArea(OLED_t* oled);

void OLED_SetPixel(OLED_t* oled, uint8_t x, uint8_t y, bool state);
void OLED_PrintText(OLED_t* oled, uint8_t x, uint8_t y, const char* str, uint8_t size);
void OLED_PrintImage(OLED_t* oled, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t* image);
void OLED_PrintVar(OLED_t* oled, uint8_t x, uint8_t y, float num, const char* type, uint8_t length, bool zeroPad);
void OLED_PrintHLine(OLED_t* oled, uint8_t x, uint8_t y, uint8_t width);

void OLED_LowBrightness(OLED_t* oled, bool enable);
void OLED_Power(OLED_t* oled, bool state);

#endif
