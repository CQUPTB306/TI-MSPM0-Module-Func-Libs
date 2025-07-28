# OLED Library for TI MSPM0 Microcontroller

一个用于TI MSPM0微控制器控制I2C 128 * 64/ 128 * 32 OLED的C++库。

## 功能特性

- 文本显示（内置6x8和8x16字体）
- 图像显示功能
- 变量值显示（浮点/整数）
- 基本图形绘制（线条、像素点）
- 部分显示缓冲区管理，支持下半屏（若使用128*64）自定义画点
- 亮度调节
- 节能模式

## 硬件要求

- I2C接口OLED显示屏（128x64或128x32）
- TI MSPM0系列微控制器
- I2C接口连接

## 快速开始

```cpp
#include "ti_msp_dl_config.h"
#include "oled.h"

OLED oled(64, I2C_0_INST); // 此构造函数需传入屏幕高度和使用到的硬件I2C实例

int main(void) {
    SYSCFG_DL_init();

    oled.begin();

    oled.printText(0, 0, "Init OK!", 16);
    oled.printText(0, 4, "Buffer Area Below", 8);

    // ...

    while (1) {
        // ...

        // Print variables
        static float i_am_a_float = 0.0;
        static int16_t i_am_a_int = 0;
        oled.printVar(10, 2, i_am_a_float, "float", 5, false);
        oled.printVar(10, 3, i_am_a_int, "int", 5, false);
        i_am_a_float += 0.1;
        i_am_a_int++;
        
        // Draw points (请注意，此缓冲区我只设置了下半部分屏幕，即y=[32,64]区域有效。若你想在上半屏也实现自定义画点，请更改源文件中的缓冲区大小和区域)
        static uint8_t x = 0;
        static uint8_t y = 32;
        oled.setPixel(x, y, true);
        x++;
        if (x >= 128) {
            x = 0;
            y++;
            if (y >= 64) {
                y = 32;
            }
        }
        oled.updateBufferArea();
        
        // ...
    }
}

```

## API

### 构造函数
```cpp
OLED(uint8_t height = 64, I2C_Regs* I2C_PORT = I2C_0_INST)
```
- `height`: 显示屏高度（32或64）
- `I2C_PORT`: I2C外设实例

### 方法

#### 显示控制
```cpp
bool begin(); // 初始化显示屏
void clear(); // 清空整个显示屏
void clearPart(uint8_t x1, uint8_t page1, uint8_t x2, uint8_t page2); // 清除指定区域
void updateBufferArea(); // 从缓冲区更新显示
```

#### 绘图功能
```cpp
void setPixel(uint8_t x, uint8_t y, bool state); // 设置像素点
void printText(uint8_t x, uint8_t y, const char* str, uint8_t size); // 显示文本
void printImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t* image); // 显示图像
void printVar(uint8_t x, uint8_t y, float num, const char* type, uint8_t length, bool zeroPad); // 显示变量
void printHLine(uint8_t x, uint8_t y, uint8_t width); // 绘制水平线
```

#### 电源管理
```cpp
void lowBrightness(bool enable); // 启用/禁用低亮度模式
void power(bool state); // 打开/关闭显示屏
```
