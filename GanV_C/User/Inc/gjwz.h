#ifndef GJWZ_H
#define GJWZ_H

#include "ti_msp_dl_config.h"

/* I2C 地址 - 尝试不同的地址 */
#define GJWZ_I2C_ADDRESS_7BIT    0x4C  /* 0x98 >> 1 (标准7位地址) */
#define GJWZ_I2C_ADDRESS_8BIT    0x98  /* 原始8位地址 */

/* 默认使用7位地址 */
#define GJWZ_I2C_ADDRESS         GJWZ_I2C_ADDRESS_7BIT

/* 命令定义 - 根据循迹模块手册 */
#define GJWZ_CMD_READ_DIGITAL    0xDD  /* 读取数字信号命令 */

/* 寄存器地址定义 */
#define GJWZ_REG_OUTPUT          0x00
#define GJWZ_REG_CONFIG          0x01
#define GJWZ_REG_ID              0x02

/* 函数声明 */
void    GJWZ_Init(I2C_Regs *i2cInst);
void    GJWZ_WriteReg(I2C_Regs *i2cInst, uint8_t address, uint8_t byte);
uint8_t GJWZ_ReadReg(I2C_Regs *i2cInst, uint8_t address);
uint8_t GJWZ_GetDigital(I2C_Regs *i2cInst, uint8_t *byte);
uint8_t GJWZ_GetDigitalWithCmd(I2C_Regs *i2cInst, uint8_t cmd, uint8_t *byte);
void    GJWZ_ScanBus(I2C_Regs *i2cInst);

#endif /* GJWZ_H */