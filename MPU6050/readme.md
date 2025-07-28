# MPU6050 Library for TI MSPM0 Series

## 简介
本库为TI MSPM0系列微控制器提供的MPU6050六轴传感器(加速度计+陀螺仪)驱动库，支持：
- 传感器初始化配置
- 原始数据读取
- 自动校准
- 姿态解算(俯仰角、滚转角、偏航角)
- 温度读取

## 硬件连接
将MPU6050模块与MSPM0开发板通过I2C接口连接：
- SCL - 连接到MSPM0的I2C时钟线
- SDA - 连接到MSPM0的I2C数据线
- VCC - 3.3V电源
- GND - 共地

注意：确保模块上I2C地址正确(默认0x68)

## 快速开始

```cpp
#include "ti_msp_dl_config.h"
#include "mpu6050.h"

uint8_t id;
MPU6050_Raw raw;
float yaw;

MPU6050 mpu(I2C_1_INST); // 传入你的I2C外设实例指针

int main(void) {
    SYSCFG_DL_init();

    mpu.begin(0x06, 0x01, 0x00, false);
    delay(10);
    mpu.calibrate(2000);

    // ...

    while (1) {
        // ...
        id = mpu.getID();
        raw = mpu.getRawData();

        mpu.updateEuler(); //获取欧拉角前需要先使用此方法计算欧拉角
        yaw = mpu.getYaw();
        // ...
    }
}
```

## API

### 构造函数
```cpp
MPU6050(I2C_Regs* I2C_PORT = I2C_0_INST)
```
- `I2C_PORT`: 指定使用的I2C外设实例指针，默认为I2C_0_INST

### 初始化
```cpp
void begin(uint8_t samplerate_div, uint8_t gyro_config, uint8_t accel_config, bool useint = false)
```
- `samplerate_div`: 采样率分频系数(0-255)
- `gyro_config`: 陀螺仪量程配置(MPU6050_GYRO_FS_250/500/1000/2000)
- `accel_config`: 加速度计量程配置(MPU6050_ACCEL_FS_2/4/8/16)
- `useint`: 是否使用中断模式

### 数据获取
```cpp
MPU6050_Raw getRawData()
```
返回包含原始传感器数据的结构体

```cpp
void rawConv(const MPU6050_Raw& raw, float* data)
```
将原始数据转换为物理量(m/s²和°/s)

### 校准
```cpp
void calibrate(uint16_t samples)
```
- `samples`: 校准采样次数(建议至少1000次以上)

### 姿态更新
```cpp
void updateEuler()
```
更新所有姿态角(俯仰、滚转、偏航)

```cpp
void updateYaw()
```
仅更新偏航角(适用于仅需航向的应用)

### 姿态获取
```cpp
float getPitch()
float getRoll() 
float getYaw()
```
返回对应的姿态角(度)，get之前需要调用updateEuler() 或 updateYaw()(如果只需要偏航角)

### 其他功能
```cpp
int16_t getTemp()
```
返回温度值(原始数据)

```cpp
uint8_t getID()
```
返回设备ID(用于验证连接)

## 注意事项
1. 校准时请确保传感器保持静止，校准样本越多，数据越不容易飘移
2. mpu6050.h中的`YAW_COMPENSATION_FACTOR`宏需要根据你实际得到的偏航角数据偏差来更改。比如，你转过90°，但偏航角改变了130°，则你需要将此值设置为 当前宏的值*90/130
3. 偏航角会随时间漂移，需要定期校准
4. 有些模块使用的芯片可能是MPU6500(地址0x70)，请注意区分
