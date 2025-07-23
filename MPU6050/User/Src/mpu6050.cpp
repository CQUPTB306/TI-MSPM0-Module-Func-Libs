#include "mpu6050.h"

MPU6050::MPU6050(I2C_Regs* I2C_PORT) : I2C_PORT_(I2C_PORT) {}

void MPU6050::begin(uint8_t samplerate_div, uint8_t gyro_config, uint8_t accel_config, bool useint) {
    accel_cfg_ = accel_config;
    gyro_cfg_ = gyro_config;

    uint8_t data_buf[2];

    // PWR_MGMT_1
    data_buf[0] = MPU6050_PWR_MGMT_1;
    data_buf[1] = 0x01;
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, data_buf, 2);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);

    // PWR_MGMT_2
    data_buf[0] = MPU6050_PWR_MGMT_2;
    data_buf[1] = 0x00;
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, data_buf, 2);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);

    // SMPLRT_DIV
    data_buf[0] = MPU6050_SMPLRT_DIV;
    data_buf[1] = samplerate_div;
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, data_buf, 2);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);

    // CONFIG
    data_buf[0] = MPU6050_CONFIG;
    data_buf[1] = 0x04;
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, data_buf, 2);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);

    // GYRO_CONFIG
    data_buf[0] = MPU6050_GYRO_CONFIG;
    data_buf[1] = gyro_config << 3;
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, data_buf, 2);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);

    // ACCEL_CONFIG
    data_buf[0] = MPU6050_ACCEL_CONFIG;
    data_buf[1] = accel_config << 3;
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, data_buf, 2);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);

    if (useint) {
        data_buf[0] = MPU6050_INT_PIN_CFG;
        data_buf[1] = 0x02;
        DL_I2C_fillControllerTXFIFO(I2C_PORT_, data_buf, 2);
        while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
        DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
        while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
        while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
        DL_I2C_flushControllerTXFIFO(I2C_PORT_);

        data_buf[0] = MPU6050_INT_ENABLE;
        data_buf[1] = 0x01;
        DL_I2C_fillControllerTXFIFO(I2C_PORT_, data_buf, 2);
        while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
        DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
        while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
        while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
        DL_I2C_flushControllerTXFIFO(I2C_PORT_);

        clrInt();
    }
}

void MPU6050::clrInt() {
    uint8_t reg = MPU6050_INT_STATUS;
    uint8_t dummy;
    // 读1字节
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, &reg, 1);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));

    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_RX, 1);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    dummy = DL_I2C_receiveControllerData(I2C_PORT_);
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);
}

MPU6050_Raw MPU6050::getRawData() {
    MPU6050_Raw data;
    uint8_t reg = MPU6050_ACCEL_XOUT_H;
    uint8_t buf[14]; // [AccelX,Y,Z,Temp,GyroX,Y,Z]
    // 读取14字节
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, &reg, 1);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_RX, 14);
    for (int i = 0; i < 14; i++) {
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_PORT_));
        buf[i] = DL_I2C_receiveControllerData(I2C_PORT_);
    }
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);

    data.AccelX = (buf[0] << 8) | buf[1];
    data.AccelY = (buf[2] << 8) | buf[3];
    data.AccelZ = (buf[4] << 8) | buf[5];
    data.GyroX  = (buf[8] << 8) | buf[9];
    data.GyroY  = (buf[10] << 8) | buf[11];
    data.GyroZ  = (buf[12] << 8) | buf[13];

    return data;
}

void MPU6050::rawConv(const MPU6050_Raw& raw, float* data) {
    float accel_sensitivity;
    switch (accel_cfg_) {
        case 0: accel_sensitivity = 16384.0f; break; // ±2g
        case 1: accel_sensitivity = 8192.0f; break;  // ±4g
        case 2: accel_sensitivity = 4096.0f; break;  // ±8g
        case 3: accel_sensitivity = 2048.0f; break;  // ±16g
        default: accel_sensitivity = 16384.0f; break;
    }

    float gyro_sensitivity;
    switch (gyro_cfg_) {
        case 0: gyro_sensitivity = 131.0f; break; // ±250dps
        case 1: gyro_sensitivity = 65.5f; break;  // ±500dps
        case 2: gyro_sensitivity = 32.8f; break;  // ±1000dps
        case 3: gyro_sensitivity = 16.4f; break;  // ±2000dps
        default: gyro_sensitivity = 131.0f; break;
    }

    data[0] = raw.AccelX / accel_sensitivity;
    data[1] = raw.AccelY / accel_sensitivity;
    data[2] = raw.AccelZ / accel_sensitivity;

    data[3] = (raw.GyroX - gyroX_offset_) / gyro_sensitivity;
    data[4] = (raw.GyroY - gyroY_offset_) / gyro_sensitivity;
    data[5] = (raw.GyroZ - gyroZ_offset_) / gyro_sensitivity;
}

int16_t MPU6050::getTemp() {
    uint8_t reg = MPU6050_TEMP_OUT_H;
    uint8_t temp_buf[2];
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, &reg, 1);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_RX, 2);
    for (int i = 0; i < 2; i++) {
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_PORT_));
        temp_buf[i] = DL_I2C_receiveControllerData(I2C_PORT_);
    }
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);
    int16_t temp = (temp_buf[0] << 8) | temp_buf[1];
    return temp;
}

uint8_t MPU6050::getID() {
    uint8_t reg = MPU6050_WHO_AM_I;
    uint8_t id;
    DL_I2C_fillControllerTXFIFO(I2C_PORT_, &reg, 1);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_PORT_, MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_RX, 1);
    while (DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_PORT_) & DL_I2C_CONTROLLER_STATUS_IDLE));
    id = DL_I2C_receiveControllerData(I2C_PORT_);
    DL_I2C_flushControllerTXFIFO(I2C_PORT_);
    return id;
}

void MPU6050::calibrate(uint16_t samples) {
    MPU6050_Raw data;
    gyroX_offset_ = gyroY_offset_ = gyroZ_offset_ = 0;
    for (uint16_t i = 0; i < samples; i++) {
        data = getRawData();
        gyroX_offset_ += data.GyroX;
        gyroY_offset_ += data.GyroY;
        gyroZ_offset_ += data.GyroZ;
        // 建议在外部加延时
    }
    gyroX_offset_ /= samples;
    gyroY_offset_ /= samples;
    gyroZ_offset_ /= samples;
}

void MPU6050::updateEuler() {
    float data_conv[6];
    MPU6050_Raw data = getRawData();
    rawConv(data, data_conv);

    float accelX = data_conv[0];
    float accelY = data_conv[1];
    float accelZ = data_conv[2];

    float gyroX = data_conv[3];
    float gyroY = data_conv[4];
    float gyroZ = data_conv[5];

    float pitch_acc = atan2f(accelY, sqrtf(accelX * accelX + accelZ * accelZ));
    float roll_acc  = atan2f(accelX, sqrtf(accelY * accelY + accelZ * accelZ));
    pitch_acc *= 180.0f / 3.1415926536f;
    roll_acc  *= 180.0f / 3.1415926536f;

    yaw_ += gyroZ * YAW_COMPENSATION_FACTOR;

    static uint8_t is_first_run = 1;
    if (is_first_run) {
        pitch_ = pitch_acc;
        roll_ = roll_acc;
        is_first_run = 0;
    } else {
        pitch_ += gyroY * dt;
        roll_  += gyroX * dt;
    }

    float accelMagnitude = sqrtf(
        data.AccelX * data.AccelX +
        data.AccelY * data.AccelY +
        data.AccelZ * data.AccelZ
    );
    if (fabs(accelMagnitude - 1.0f) < 0.1f) {
        // 可根据需要重置yaw_等，略
    }

    float alpha = 0.96f;
    pitch_ = alpha * pitch_ + (1 - alpha) * pitch_acc;
    roll_  = alpha * roll_  + (1 - alpha) * roll_acc;
}

void MPU6050::updateYaw() {
    float gyro_sensitivity;
    switch (gyro_cfg_) {
        case 0: gyro_sensitivity = 131.0f; break;
        case 1: gyro_sensitivity = 65.5f; break;
        case 2: gyro_sensitivity = 32.8f; break;
        case 3: gyro_sensitivity = 16.4f; break;
        default: gyro_sensitivity = 131.0f; break;
    }
    MPU6050_Raw data = getRawData();
    float gyroZ = (data.GyroZ - gyroZ_offset_) / gyro_sensitivity;
    yaw_ += gyroZ * YAW_COMPENSATION_FACTOR;
}

float MPU6050::getPitch() { return pitch_; }
float MPU6050::getRoll()  { return roll_; }
float MPU6050::getYaw()   { return yaw_; }
