#include "gjwz.h"
#include "ti_msp_dl_config.h"
#include <stdio.h>

/* I2C 超时时间 (ms) */
#define I2C_TIMEOUT_MS  200

/* 延时函数 */
static void delayMs(uint32_t ms) {
    uint32_t t = 0;
    while (t++<ms) {
        delay_cycles(32000);
    }
}

/* I2C 错误检查和清除 */
static void checkI2CStatus(I2C_Regs *i2cInst) {
    uint32_t status = DL_I2C_getControllerStatus(i2cInst);
    if (status & DL_I2C_CONTROLLER_STATUS_ERROR) {
        /* 清除错误状态 - 通过复位I2C模块 */
        DL_I2C_disableController(i2cInst);
        delayMs(1);
        DL_I2C_enableController(i2cInst);
    }
}

/* 等待I2C空闲 */
static int waitI2CIdle(I2C_Regs *i2cInst, uint32_t timeout_ms) {
    for (uint32_t i = 0; i < timeout_ms; i++) {
        if (!(DL_I2C_getControllerStatus(i2cInst) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS)) {
            return 1;
        }
        delayMs(1);  /* ~1ms */
    }
    return 0;
}

/* 重置I2C控制器 (使用启用/禁用) */
static void resetI2C(I2C_Regs *i2cInst) {
    DL_I2C_disableController(i2cInst);
    delayMs(2);
    DL_I2C_enableController(i2cInst);
    delayMs(2);
}

/* 扫描I2C总线 - 用于调试 */
void GJWZ_ScanBus(I2C_Regs *i2cInst) {
    uint8_t found = 0;
    extern void UART_SendString(const char *str);
    extern void UART_SendHex(uint8_t value);
    
    UART_SendString("Scanning I2C bus...\r\n");
    
    for (uint8_t addr = 0x01; addr < 0x7F; addr++) {
        /* 重置I2C */
        resetI2C(i2cInst);
        
        /* 尝试发送1字节来检测设备 */
        DL_I2C_startControllerTransfer(i2cInst, addr,
                                       DL_I2C_CONTROLLER_DIRECTION_TX, 1);
        
        /* 等待传输完成或超时 */
        for (uint32_t i = 0; i < 50; i++) {
            if (DL_I2C_getControllerStatus(i2cInst) & DL_I2C_CONTROLLER_STATUS_IDLE) break;
            delayMs(1);  /* ~1ms */
        }
        
        /* 检查是否有错误 (NACK表示设备不存在) */
        uint32_t status = DL_I2C_getControllerStatus(i2cInst);
        if (!(status & DL_I2C_CONTROLLER_STATUS_ERROR)) {
            UART_SendString("Found device at 0x");
            UART_SendHex(addr);
            UART_SendString("\r\n");
            found++;
        }
        
        /* 重置I2C为下次扫描 */
        resetI2C(i2cInst);
        delayMs(5);
    }
    
    if (found == 0) {
        UART_SendString("No devices found!\r\n");
    } else {
        UART_SendString("Scan complete. Found ");
        char buf[8];
        sprintf(buf, "%d", found);
        UART_SendString(buf);
        UART_SendString(" devices.\r\n");
    }
}

/* 初始化循迹模块 */
void GJWZ_Init(I2C_Regs *i2cInst) {
    (void)i2cInst;
    /* 模块上电后可能需要延时 */
    delayMs(100);
}

/* 写寄存器 */
void GJWZ_WriteReg(I2C_Regs *i2cInst, uint8_t address, uint8_t byte) {
    uint8_t txData[2] = {address, byte};
    
    /* 重置I2C控制器确保干净状态 */
    resetI2C(i2cInst);
    
    /* 等待I2C空闲 */
    if (!waitI2CIdle(i2cInst, I2C_TIMEOUT_MS)) return;
    
    /* 清除错误状态 */
    checkI2CStatus(i2cInst);
    
    /* 发送数据: 先发寄存器地址，再发数据 */
    DL_I2C_startControllerTransfer(i2cInst, GJWZ_I2C_ADDRESS, 
                                   DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    
    /* 填充TX FIFO */
    DL_I2C_fillControllerTXFIFO(i2cInst, txData, 2);
    
    /* 等待传输完成 */
    if (!waitI2CIdle(i2cInst, I2C_TIMEOUT_MS)) return;
    
    /* 检查错误 */
    checkI2CStatus(i2cInst);
}

/* 读寄存器 */
uint8_t GJWZ_ReadReg(I2C_Regs *i2cInst, uint8_t address) {
    uint8_t rxData = 0;
    
    /* 重置I2C控制器 */
    resetI2C(i2cInst);
    
    /* 等待I2C空闲 */
    if (!waitI2CIdle(i2cInst, I2C_TIMEOUT_MS)) return 0;
    
    /* 清除错误状态 */
    checkI2CStatus(i2cInst);
    
    /* 1. 先写寄存器地址 (写操作) */
    DL_I2C_startControllerTransfer(i2cInst, GJWZ_I2C_ADDRESS,
                                   DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    DL_I2C_fillControllerTXFIFO(i2cInst, &address, 1);
    
    if (!waitI2CIdle(i2cInst, I2C_TIMEOUT_MS)) return 0;
    
    /* 检查写操作是否成功 */
    if (DL_I2C_getControllerStatus(i2cInst) & DL_I2C_CONTROLLER_STATUS_ERROR) {
        checkI2CStatus(i2cInst);
        return 0;
    }
    
    /* 短暂延时确保设备准备数据 */
    delayMs(1);
    
    /* 2. 再读数据 (读操作) */
    DL_I2C_startControllerTransfer(i2cInst, GJWZ_I2C_ADDRESS,
                                   DL_I2C_CONTROLLER_DIRECTION_RX, 1);

    /* 等待数据到达 */
    uint32_t i;
    for (i = 0; i < I2C_TIMEOUT_MS; i++) {
        if (!DL_I2C_isControllerRXFIFOEmpty(i2cInst)) break;
        delayMs(1);  /* ~1ms */
    }
    if (DL_I2C_isControllerRXFIFOEmpty(i2cInst)) return 0;

    rxData = DL_I2C_receiveControllerData(i2cInst);
    
    /* 等待传输完成 */
    waitI2CIdle(i2cInst, I2C_TIMEOUT_MS);
    
    return rxData;
}

/* 直接读取数字信号 */
uint8_t GJWZ_GetDigital(I2C_Regs *i2cInst, uint8_t *byte) {
    /* 重置I2C控制器 */
    resetI2C(i2cInst);
    
    /* 等待I2C空闲 */
    if (!waitI2CIdle(i2cInst, I2C_TIMEOUT_MS)) return 0;
    
    /* 清除错误状态 */
    checkI2CStatus(i2cInst);
    
    /* 直接读取1字节数据 */
    DL_I2C_startControllerTransfer(i2cInst, GJWZ_I2C_ADDRESS,
                                   DL_I2C_CONTROLLER_DIRECTION_RX, 1);
    
    /* 等待数据到达 */
    for (uint32_t i = 0; i < I2C_TIMEOUT_MS; i++) {
        if (!DL_I2C_isControllerRXFIFOEmpty(i2cInst)) break;
        delayMs(1);  /* ~1ms */
    }
    if (DL_I2C_isControllerRXFIFOEmpty(i2cInst)) return 0;

    *byte = DL_I2C_receiveControllerData(i2cInst);
    
    /* 等待传输完成 */
    waitI2CIdle(i2cInst, I2C_TIMEOUT_MS);
    
    return 1;
}

/* 带命令字节的读取 */
uint8_t GJWZ_GetDigitalWithCmd(I2C_Regs *i2cInst, uint8_t cmd, uint8_t *byte) {
    /* 重置I2C控制器 */
    resetI2C(i2cInst);
    
    /* 等待I2C空闲 */
    if (!waitI2CIdle(i2cInst, I2C_TIMEOUT_MS)) return 0;
    
    /* 清除错误状态 */
    checkI2CStatus(i2cInst);
    
    /* 1. 先发送命令 */
    DL_I2C_startControllerTransfer(i2cInst, GJWZ_I2C_ADDRESS,
                                   DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    DL_I2C_fillControllerTXFIFO(i2cInst, &cmd, 1);
    
    if (!waitI2CIdle(i2cInst, I2C_TIMEOUT_MS)) return 0;
    
    /* 检查写操作是否成功 */
    if (DL_I2C_getControllerStatus(i2cInst) & DL_I2C_CONTROLLER_STATUS_ERROR) {
        checkI2CStatus(i2cInst);
        return 0;
    }
    
    /* 短暂延时 */
    delayMs(1);
    
    /* 2. 再读数据 */
    DL_I2C_startControllerTransfer(i2cInst, GJWZ_I2C_ADDRESS,
                                   DL_I2C_CONTROLLER_DIRECTION_RX, 1);

    for (uint32_t i = 0; i < I2C_TIMEOUT_MS; i++) {
        if (!DL_I2C_isControllerRXFIFOEmpty(i2cInst)) break;
        delayMs(1);  /* ~1ms */
    }
    if (DL_I2C_isControllerRXFIFOEmpty(i2cInst)) return 0;

    *byte = DL_I2C_receiveControllerData(i2cInst);
    
    waitI2CIdle(i2cInst, I2C_TIMEOUT_MS);
    
    return 1;
}