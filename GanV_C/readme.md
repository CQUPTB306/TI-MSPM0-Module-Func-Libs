# TI MSPM0 感为八路循迹库

本库是适用于MSPM0的感为八路循迹库 ~~虽然大概率不允许使用~~，可直接获取八个传感器的高低电平。

## 功能特点

- 直接读取八路高低电平

## 使用方法

1. 将本库添加到您的工程中
2. 包含头文件：`#include "gjwz.h"`
3. 初始化I2C
4. 调用`GJWZ_GetDigital(GJWZ_INST, &data);`读取传感器电平并传到变量`data`中


## 使用示例
```c
#include "ti_msp_dl_config.h"
#include "gjwz.h"

int main(void)
{
    SYSCFG_DL_init();
    
    GJWZ_Init(GJWZ_INST);
    
    uint8_t gjwz_data = 0;          // 接收数据
    int32_t line_pos = 0;           // 有效线位置，失线时保持
    
    /* 循迹控制参数（8路灰度传感器，bit0=最左侧） */
    static const int32_t WEIGHT[8] = { 0, 500, 250, 50, -50, -250, -500, 0 };

    while (1) {
        GJWZ_GetDigital(GJWZ_INST, &gjwz_data);

        int32_t sum = 0, count = 0;
        /* ── 加权平均求线位置 ── */

        for (int i = 0; i < 8; i++) {
            if ((gjwz_data & (1 << i)) == 0) {
                sum += WEIGHT[i];
                count++;
            }
        }
        if (count >= 2) {             // 防止失线
            line_pos = sum / count;   // 负=偏左，正=偏右
        }
        ...
    }
}
```