# JY901S Lib for TI MSPM0

本库是为电赛小车设计的JY901S姿态传感器驱动库，目前功能是获取欧拉角，其他功能将在后续有空时补充。

## 功能特点

- 已实现功能：
  - 欧拉角获取
  - UART中断接收数据帧
  - 数据校验和解析

- 待实现功能：
  - 加速度和角速度数据获取
  - 温度数据获取

## 使用方法

1. 将本库添加到您的基于cpp的工程中
2. 包含头文件：`#include "jy901s.h"`
3. 初始化UART接口(自行配置，波特率记得检查和上位机配置一致)
4. 在中断服务函数中调用数据接收逻辑

## APIs

### JY901S 类

```cpp
class JY901S {
public:
    JY901S(IRQn_Type JY901S_IRQn);  // 构造函数，传入UART中断号
    void begin();                    // 初始化传感器
    bool decode();                   // 解码接收到的数据帧
    float getYaw();                  // 获取偏航角(当前主要功能)
    float getPitch();                // 获取俯仰角(待完善)
    float getRoll();                 // 获取横滚角(待完善)
};
```

## 使用示例

```cpp
#include "ti_msp_dl_config.h"
#include "jy901s.h"

float yaw = 0.0;  // 存储偏航角

// 创建JY901S实例，传入UART中断号
JY901S jy(UART_JY901S_INST_INT_IRQN);

int main() {
    // 系统初始化
    SYSCFG_DL_init();

    // 初始化JY901S传感器
    jy.begin();

    while (1) {
        // 主循环中解码数据
        if (jy.decode()) {
            // 获取偏航角(当前主要使用功能)
            yaw = jy.getYaw();
        }
    }
}

// UART中断服务函数
extern "C" void UART_JY901S_INST_IRQHandler() {
    switch (DL_UART_getPendingInterrupt(UART_JY901S_INST)) {
        case DL_UART_IIDX_RX: {
            // 接收数据字节
            uint8_t rxByte = DL_UART_Main_receiveData(UART_JY901S_INST);
            
            // 数据帧解析逻辑
            if (jy.jy901s_rx_index == 0) {
                if (rxByte == JY901S_FRAME_HEAD1) {
                    jy.jy901s_rx_data[jy.jy901s_rx_index++] = rxByte;
                }
            } 
            else if (jy.jy901s_rx_index == 1) {
                if (rxByte == JY901S_FRAME_HEAD2) {
                    jy.jy901s_rx_data[jy.jy901s_rx_index++] = rxByte;
                } else {
                    jy.jy901s_rx_index = 0;
                }
            } 
            else {
                jy.jy901s_rx_data[jy.jy901s_rx_index++] = rxByte;
                if (jy.jy901s_rx_index >= JY901S_FRAME_LEN) {
                    jy.jy901s_frame_ready = true;
                    jy.jy901s_rx_index = 0;
                }
            }
            
            break;
        }
        default: break;
    }

    // 清除中断标志
    DL_UART_clearInterruptStatus(UART_JY901S_INST, DL_UART_IIDX_RX);
}
```

## 注意事项

0. !!请确保中断函数位于extern "C" 块中!!
1. 本库当前主要针对电赛小车需要的欧拉角功能开发
2. 使用时请确保正确配置UART，UART的rxBuffer使用单字节即可
3. 数据帧头为0x55和0x53，帧长度为11字节
4. 偏航角范围为-180°到+180°

## 计划

if (打完电赛 && 有时间 && 有兴趣) { 会来更新其余功能 }
- 添加传感器校准功能
- 增加更多传感器数据支持
