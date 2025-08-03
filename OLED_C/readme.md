# TI MSPM0 OLED显示库

一个轻量级的C版本oled库，用于通过I2C控制TI MSPM0微控制器连接的OLED显示屏。

## 功能特性

- 支持128x32和128x64 OLED显示屏
- I2C接口(默认地址0x3C)
- 文本显示，支持两种英文字体(6x8和8x16)
- 图像显示功能
- 变量显示(带格式的数字)
- 绘图功能(像素、线条)
- 亮度控制
- 节能模式

## 硬件要求

- TI MSPM0微控制器
- 基于SSD1306的OLED显示屏(128x32或128x64)
- I2C连接(SCL, SDA, VCC, GND)

## 快速入门

1. 在项目中包含库文件:
```c
#include "oled.h"
```

2. 初始化OLED显示屏:
```c
OLED_t oled;
OLED_Init(&oled, 64, I2C_0_INST); // 高度(32或64), I2C端口
OLED_Begin(&oled);
```

3. 显示文本:
```c
uint32_t im_a_int_type = 114514;
float im_a_float_type = 1919.810;
OLED_PrintText(&oled, 0, 0, "M0 such a SH*T", 1); // x坐标(0 - 127), y坐标(页0 - 7), 文本, 字体大小(8 / 16)
OLED_PrintVar(&oled, 0, 1, im_a_float_type, "float", 8, false); // false表示不补零, true表示补零
OLED_PrintVar(&oled, 0, 2, im_a_int_type, "int", 6, false);
OLED_Clear(&oled);
```

## API参考

### 初始化
- `void OLED_Init(OLED_t* oled, uint8_t height, I2C_Regs* I2C_PORT)`
- `bool OLED_Begin(OLED_t* oled)`

### 显示控制
- `void OLED_Clear(OLED_t* oled)`
- `void OLED_ClearPart(OLED_t* oled, uint8_t x1, uint8_t page1, uint8_t x2, uint8_t page2)`
- `void OLED_SetCursor(OLED_t* oled, uint8_t x, uint8_t y)`
- `void OLED_UpdateBufferArea(OLED_t* oled)`

### 绘图功能
- `void OLED_SetPixel(OLED_t* oled, uint8_t x, uint8_t y, bool state)`
- `void OLED_PrintText(OLED_t* oled, uint8_t x, uint8_t y, const char* str, uint8_t size)`
- `void OLED_PrintImage(OLED_t* oled, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t* image)`
- `void OLED_PrintVar(OLED_t* oled, uint8_t x, uint8_t y, float num, const char* type, uint8_t length, bool zeroPad)`
- `void OLED_PrintHLine(OLED_t* oled, uint8_t x, uint8_t y, uint8_t width)`

### 电源管理
- `void OLED_LowBrightness(OLED_t* oled, bool enable)`
- `void OLED_Power(OLED_t* oled, bool state)`

