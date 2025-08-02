# TI MSPM0 MPU6050 DMP库

本库为TI MSPM0微控制器提供MPU6050六轴运动传感器的数字运动处理器(DMP)功能支持。

## 功能特性

- 完整支持InvenSense MPU6050 DMP功能
- 实时运动处理，CPU占用低
- 支持以下功能：
  - 点击检测(单次和多次点击)
  - 屏幕方向检测(竖屏/横屏)
  - 计步器(步数统计)
  - 低功耗四元数计算
  - 陀螺仪校准
- 简洁易用的API接口

## 硬件要求

- TI MSPM0系列微控制器
- MPU6050六轴运动传感器
- I2C接口连接

## 快速开始
本实例程展示了如何使用MPU6050 DMP库，实现MPU6050的陀螺仪数据实时处理，并获取运动数据。
```c
#include "mpu6050.h"
#include "inv_mpu.h"

float pitch, roll, yaw;
float delta_yaw = 0.0;

float yaw_stable;
bool yaw_stablized = false;

int main() {
    MPU6050_Init();
    mpu_dmp_init();

	// ... 使用 pitch, roll, yaw 变量 ...
	// Other codes...
}

void TIMER_General_INST_IRQHandler(void) {
    if (DL_TimerG_getPendingInterrupt(TIMER_General_INST) == DL_TIMER_IIDX_ZERO) {
        static float last_yaw = 999.9;
        mpu_dmp_get_data(&pitch, &roll, &yaw);

		if (!yaw_stablized) {
			if (fabs(last_yaw - yaw) < 0.01) {
				yaw_stablized = true;
				yaw_stable = yaw;
			} else {
				last_yaw = yaw;
			}
		}
    }
    DL_TimerG_clearInterruptStatus(TIMER_General_INST, DL_TIMER_IIDX_ZERO);
    NVIC_ClearPendingIRQ(TIMER_General_INST_INT_IRQN);
}
```

## API参考

### 基础MPU6050函数
- `uint8_t MPU6050_Init(void)` - 初始化MPU6050传感器
- `void MPU6050ReadGyro(short *gyroData)` - 读取原始陀螺仪数据
- `void MPU6050ReadAcc(short *accData)` - 读取原始加速度计数据
- `float MPU6050_GetTemp(void)` - 读取温度值(摄氏度)

### DMP函数
- `int dmp_load_motion_driver_firmware(void)` - 加载DMP固件
- `int dmp_enable_feature(unsigned short mask)` - 启用DMP功能
- `int dmp_read_fifo(...)` - 从FIFO读取处理后的传感器数据
- `int dmp_set_orientation(unsigned short orient)` - 设置屏幕方向
- `int dmp_get_pedometer_step_count(unsigned long *count)` - 获取步数统计

### 及其他函数，不作介绍