# Encoder Lib for TI MSPM0

一个轻量级的编码器C++库，用于TI MSPM0微控制器，通过硬件中断支持提供速度和距离测量功能。
C++库相较于仓库中C库的优势在于可以方便的控制多个编码器对象。

## 功能特性

- 基于硬件中断的脉冲计数
- 速度计算(单位：cm/s)
- 距离测量(单位：cm)
- 方向检测(前进/后退)
- 可配置参数，适用于不同类型的编码器
- 使用定时器中断，CPU占用低

## 硬件要求

- TI MSPM0系列微控制器
- 霍尔效应编码器传感器，已知PPG(每秒脉冲数)
- 已知减速比的电机
- 已知周长(直径)的轮子

## 安装方法

1. 将`inc/encoder.h`和`src/encoder.cpp`文件复制到您的项目
2. 在您的主程序中包含`encoder.h`
3. 根据需要配置`encoder.h`中的参数：
   ```c
   #define HALL_ENC_PULSE_PER_TURN 26.0f  // 每转脉冲数
   #define MOTOR_REDUCTION_RATIO 28.0f    // 齿轮减速比
   #define WHEEL_PERIMETER 21.6f          // 轮子周长(厘米)
   #define SAMPLE_INTERVAL_SEC 0.02f      // 更新间隔(秒),请根据你设置的定时器周期调整
   ```


## 常见问题

1. **读数不准确**：
   - 检查`HALL_ENC_PULSE_PER_TURN`及其他轮周长/减速比参数是否正确
   - 确保中断优先级设置正确

2. **速度计算波动大**：
   - 尝试调整`SAMPLE_INTERVAL_SEC`和定时器触发间隔参数

3. **中断不触发或进入DefaultIRQ**：
   - 验证GPIO中断配置
   - 检查NVIC中断使能设置
   - 检查中断处理函数是否有extern "C"修饰

## APIs

### `Encoder(IRQn_Type EXTI_IRQn, IRQn_Type TIMER_IRQn)`
编码器类的构造函数。

参数:
- `EXTI_IRQn`: 编码器脉冲的外部中断IRQ
- `TIMER_IRQn`: 速度计算的定时器中断IRQ

### `void begin()`
初始化编码器硬件中断和定时器。

### `int32_t getCount() const`
返回自上次更新以来的原始脉冲计数。

### `uint16_t getSpeed() const`
返回当前速度(cm/s)。

### `uint32_t getDistance() const`
返回总移动距离(cm)。

### `void clearDistance()`
将距离计数器重置为零。

### `Direction getDirection() const`
返回当前运动方向(前进或后退)。

### `void update()`
更新速度和距离计算。由定时器中断自动调用。

## 配置说明

在`encoder.h`中修改以下常量以适应您的硬件:

```cpp
#define HALL_ENC_PULSE_PER_TURN 52.0f    // 每转脉冲数
#define MOTOR_REDUCTION_RATIO 34.014f    // 齿轮减速比
#define WHEEL_PERIMETER 18.84955592f     // 轮子周长(cm)
#define SAMPLE_INTERVAL_SEC 0.02f        // 速度计算间隔
```

## 使用示例

此图展示如何配置编码器两相的IO模式和中断，注意要在两个IO中都同样配置上升沿/下降沿EXTI
<img width="625" height="923" alt="屏幕截图 2025-08-03 162735" src="https://github.com/user-attachments/assets/b5ac6c33-0bdd-41f3-92cf-f7f6ae49c0b2" />


此图展示如何配置定时器中断，用于速度检测
<img width="648" height="1233" alt="屏幕截图 2025-08-03 162811" src="https://github.com/user-attachments/assets/1d5d17a3-d8f4-43bf-a419-e45d9229f884" />


```cpp
#include "ti_msp_dl_config.h"
#include "encoder.h"

Encoder encoder(GPIOA_INT_IRQn, TIMER_ENC_INST_INT_IRQN);

int main() {
    SYSCFG_DL_init();
    encoder.begin();
    
    while (1) {
        float speed = encoder.getSpeed();
        float distance = encoder.getDistance();
        
        // 示例的距离清空，根据你自己的需求修改
        if (distance >= 100.0) {
            encoder.clearDistance();
        }
    }
}

// 中断处理函数
extern "C" {

void GROUP1_IRQHandler() {
    // 处理编码器脉冲
    uint32_t gpio_status = DL_GPIO_getEnabledInterruptStatus(GPIO_MOTOR_ENC_PORT, 
                          GPIO_MOTOR_ENC_A_PIN | GPIO_MOTOR_ENC_B_PIN);

    if (gpio_status & GPIO_MOTOR_ENC_A_PIN) {
        encoder.cnt_IRQ += (DL_GPIO_readPins(GPIO_MOTOR_ENC_PORT, GPIO_MOTOR_ENC_B_PIN)) ? -1 : 1;
        DL_GPIO_clearInterruptStatus(GPIO_MOTOR_ENC_PORT, GPIO_MOTOR_ENC_A_PIN);
    }
    if (gpio_status & GPIO_MOTOR_ENC_B_PIN) {
        encoder.cnt_IRQ += (DL_GPIO_readPins(GPIO_MOTOR_ENC_PORT, GPIO_MOTOR_ENC_A_PIN)) ? 1 : -1;
        DL_GPIO_clearInterruptStatus(GPIO_MOTOR_ENC_PORT, GPIO_MOTOR_ENC_B_PIN);
    }
}

void TIMER_ENC_INST_IRQHandler() {
    if (DL_TimerG_getPendingInterrupt(TIMER_ENC_INST) == DL_TIMER_IIDX_ZERO) {
        encoder.update();
    }
    DL_TimerG_clearInterruptStatus(TIMER_ENC_INST, DL_TIMER_IIDX_ZERO);
}

}
```
