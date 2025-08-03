# TI MSPM0 编码器库

适用于TI MSPM0微控制器的轻量级编码器接口库，专为霍尔编码器应用设计。

## 功能特性

- 实时速度和距离测量
- 方向检测(前进/后退)
- 可配置参数适应不同机械结构
- 简单直观的API接口
- 中断驱动实现确保精确计时

## 硬件要求

- TI MSPM0系列微控制器
- 霍尔效应编码器传感器，已知PPG(每秒脉冲数)
- 已知减速比的电机
- 已知周长(直径)的轮子

## 安装配置

1. 将`inc/encoder.h`和`src/encoder.c`复制到您的项目
2. 在主程序中包含`encoder.h`头文件
3. 根据需要配置`encoder.h`中的参数：
   ```c
   #define HALL_ENC_PULSE_PER_TURN 26.0f  // 每转脉冲数
   #define MOTOR_REDUCTION_RATIO 28.0f    // 齿轮减速比
   #define WHEEL_PERIMETER 21.6f          // 轮子周长(厘米)
   #define SAMPLE_INTERVAL_SEC 0.02f      // 更新间隔(秒),请根据你设置的定时器周期调整
   ```

## APIs

### `ENCODER_Init(ENCODER_t* enc, IRQn_Type EXTI_IRQn, IRQn_Type TIMER_IRQn)`
使用中断号初始化编码器结构体。

### `ENCODER_Begin(ENCODER_t* enc)`
通过启用中断开始编码器测量。

### `ENCODER_Update(ENCODER_t* enc)`
更新编码器计算(需定期调用)。

### `int32_t ENCODER_GetCount(const ENCODER_t* enc)`
获取自上次更新后的原始脉冲计数。

### `float ENCODER_GetSpeed(const ENCODER_t* enc)`
获取当前速度(厘米/秒)。

### `float ENCODER_GetDistance(const ENCODER_t* enc)`
获取总移动距离(厘米)。

### `void ENCODER_ClearDistance(ENCODER_t* enc)`
将距离计数器重置为零。

### `ENCODER_Dir_t ENCODER_GetDirection(const ENCODER_t* enc)`
获取当前运动方向(前进/后退)。

## 使用示例

此图展示如何配置编码器两相的IO模式和中断，注意要在两个IO中都同样配置上升沿/下降沿EXTI

<img width="625" height="923" alt="屏幕截图 2025-08-03 162735" src="https://github.com/user-attachments/assets/e2ab5369-4923-4576-be17-6b1de0590a3a" />


此图展示如何配置定时器中断，用于速度检测

<img width="648" height="1233" alt="屏幕截图 2025-08-03 162811" src="https://github.com/user-attachments/assets/9c46ff10-b389-4963-92cf-e06042ced5a2" />


```c

/*
 * 编码器完整使用示例代码
 * 使用GROUP1外部中断和TIMER_ENC定时器中断
 */

// 请注意配置编码器A/B相的IO模式以及外部中断
#include "ti_msp_dl_config.h"
#include "encoder.h"

// 定义编码器实例
ENCODER_t my_encoder;

int main(void) {
    // 系统初始化
    SYSCFG_DL_init();
    
    // 初始化编码器
    ENCODER_Init(&my_encoder, GROUP1_IRQn, TIMER_ENC_INST_INT_IRQN);
    ENCODER_Begin(&my_encoder);
    
    // 配置定时器中断
    NVIC_ClearPendingIRQ(TIMER_ENC_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_ENC_INST_INT_IRQN);
    DL_TimerG_startCounter(TIMER_ENC_INST);
    
    // 启用GROUP1中断
    NVIC_ClearPendingIRQ(GROUP1_IRQn);
    NVIC_EnableIRQ(GROUP1_IRQn);
    
    while (1) {
        // 读取并显示编码器数据
        float speed = ENCODER_GetSpeed(&my_encoder);
        float distance = ENCODER_GetDistance(&my_encoder);
        ENCODER_Dir_t dir = ENCODER_GetDirection(&my_encoder);
        
        // ...
        
        // 每秒清除一次距离计数（根据你实际的用法来写，你也可以不清）
        static uint32_t last_clear = 0;
        if (DL_TimerG_getCounterValue(TIMER_General_INST) - last_clear > 1000) {
            ENCODER_ClearDistance(&my_encoder);
            last_clear = DL_TimerG_getCounterValue(TIMER_General_INST);
        }
    }
}

// GROUP1中断处理函数
void GROUP1_IRQHandler(void) {
    uint32_t gpio_status = DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT, 
                            ENCODER_A_PIN | ENCODER_B_PIN);
                            
    if ((gpio_status & ENCODER_A_PIN) == ENCODER_A_PIN) {
        if (DL_GPIO_readPins(ENCODER_PORT, ENCODER_B_PIN) & ENCODER_B_PIN) {
            my_encoder._cnt_IRQ--;
        } else {
            my_encoder._cnt_IRQ++;
        }
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_A_PIN);
    }
    
    if ((gpio_status & ENCODER_B_PIN) == ENCODER_B_PIN) {
        if (DL_GPIO_readPins(ENCODER_PORT, ENCODER_A_PIN) & ENCODER_A_PIN) {
            my_encoder._cnt_IRQ++;
        } else {
            my_encoder._cnt_IRQ--;
        }
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_B_PIN);
    }
    
    NVIC_ClearPendingIRQ(GROUP1_IRQn);
}

// TIMER_ENC中断处理函数
void TIMER_ENC_INST_IRQHandler(void) {
    if (DL_TimerG_getPendingInterrupt(TIMER_ENC_INST) == DL_TIMER_IIDX_ZERO) {
        ENCODER_Update(&my_encoder);
    }
    DL_TimerG_clearInterruptStatus(TIMER_ENC_INST, DL_TIMER_IIDX_ZERO);
    NVIC_ClearPendingIRQ(TIMER_ENC_INST_INT_IRQN);
}

```
