#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <math.h>

#define MPU6050_ADDR 0x68 // 注意一定要是真的MPU6050。现在有很多模块用的芯片其实是MPU6500, 地址为0x70

#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG       0x1A
#define MPU6050_GYRO_CONFIG  0x1B
#define MPU6050_ACCEL_CONFIG 0x1C

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_TEMP_OUT_H   0x41
#define MPU6050_GYRO_XOUT_H  0x43

#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_PWR_MGMT_2   0x6C
#define MPU6050_WHO_AM_I     0x75

#define MPU6050_INT_ENABLE   0x38
#define MPU6050_INT_PIN_CFG  0x37
#define MPU6050_INT_STATUS   0x3A

#define dt 0.001f
#define YAW_COMPENSATION_FACTOR 0.036f

typedef struct {
    int16_t AccelX;
    int16_t AccelY;
    int16_t AccelZ;
    int16_t GyroX;
    int16_t GyroY;
    int16_t GyroZ;
} MPU6050_Raw;

class MPU6050 {
    public:
        MPU6050(I2C_Regs* I2C_PORT = I2C_0_INST);
        void begin(uint8_t samplerate_div, uint8_t gyro_config, uint8_t accel_config, bool useint = false);

        void clrInt();
        MPU6050_Raw getRawData();
        void rawConv(const MPU6050_Raw& raw, float* data);
        int16_t getTemp();
        uint8_t getID();

        void calibrate(uint16_t samples);
        void updateEuler();
        void updateYaw();

        float getPitch();
        float getRoll();
        float getYaw();

    private:
        uint8_t accel_cfg_ = 0, gyro_cfg_ = 0;
        float gyroX_offset_ = 0, gyroY_offset_ = 0, gyroZ_offset_ = 0;
        float yaw_ = 0, pitch_ = 0, roll_ = 0;
        I2C_Regs* I2C_PORT_;
};

#endif