# TB6612电机控制库 (TI MSPM0)

本库提供了基于TI MSPM0微控制器的TB6612电机驱动芯片控制功能，支持直流电机的PWM速度控制和方向控制。

## 功能特性

- 支持直流电机正反转控制
- PWM速度控制（-100%到100%占空比，浮点数）
- 简单的API接口

### 连接方式

1. 将TB6612的IN1和IN2引脚连接到MSPM0的GPIO，IN1和IN2需位于同一GPIO端口
2. 将PWM引脚连接到定时器PWM输出
3. 确保共地和电源连接正确

### API

#### `Motor::Motor(GPIO_Regs*, uint32_t, uint32_t, GPTIMER_Regs*, DL_TIMER_CC_INDEX)`

构造函数，创建电机控制对象。

参数：
- `in_gpio_port`: GPIO端口寄存器指针
- `in1_pin`: IN1控制引脚号
- `in2_pin`: IN2控制引脚号
- `gptimer`: 定时器寄存器指针
- `cc_index`: 定时器捕获/比较通道索引

#### `void Motor::begin()`

初始化电机控制器，启动PWM定时器。

#### `void Motor::setDuty(float dutyCycle)`

设置电机占空比和方向。

参数：
- `dutyCycle`: 占空比（-100.0到100.0）
  - 正值: 正转
  - 负值: 反转
  - 0: 停止

#### `float Motor::getDuty()`

获取当前占空比。

返回值：
- 当前占空比（-100.0到100.0）

#### `uint32_t Motor::__getCCR_for_debug()`

获取当前CCR寄存器值（用于调试）。

### 示例代码

```cpp
#include "ti_msp_dl_config.h"
#include "motor.h"

Motor motor(MOTOR_GPIO_PORT, MOTOR_GPIO_IN1_PIN, MOTOR_GPIO_IN2_PIN, MOTOR_PWM_INST, DL_TIMER_CC_0_INDEX); // 请根据自己设置的GPIO来初始化

int main() {
    motor.begin();

    while (1) {
        // 正转50%速度
        motor.setDuty(50.0);
        delay(1000);
        
        // 反转75.5%速度
        motor.setDuty(-75.5);
        delay(1000);
        
        // 停止
        motor.setDuty(0);
        delay(1000);
    }
}
```

## 注意事项

1. 需根据你在syscfg文件中设置的PWM_PERIOD_COUNT修改头文件中的`PWM_PERIOD_COUNT`宏，建议的值为1000。
2. 电机电源和控制电源需共地
3. 反转时只需设置负值占空比即可
