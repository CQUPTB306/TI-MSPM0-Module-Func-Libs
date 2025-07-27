#include "jy901s.h"

JY901S::JY901S(IRQn_Type JY901S_IRQn) : _JY901S_IRQn(JY901S_IRQn) {}

void JY901S::begin() {
    NVIC_ClearPendingIRQ(_JY901S_IRQn);
    NVIC_EnableIRQ(_JY901S_IRQn);
    // Ensure UART RX interrupt is enabled
    DL_UART_Main_enableInterrupt(UART_JY901S_INST, DL_UART_MAIN_INTERRUPT_RX);
}

bool JY901S::decode() {
    if (jy901s_frame_ready) {
        uint8_t sum = 0;
        for (int i = 0; i < JY901S_FRAME_LEN - 1; i++) {
            sum += jy901s_rx_data[i];
        }
        if (sum != jy901s_rx_data[JY901S_FRAME_LEN - 1]) {
            return false;
        }

        int16_t roll_raw  = (jy901s_rx_data[3] << 8) | jy901s_rx_data[2];
        int16_t pitch_raw = (jy901s_rx_data[5] << 8) | jy901s_rx_data[4];
        int16_t yaw_raw   = (jy901s_rx_data[7] << 8) | jy901s_rx_data[6];
        _angles.roll  = (float)roll_raw  / 32768.0f * 180.0f;
        _angles.pitch = (float)pitch_raw / 32768.0f * 180.0f;
        _angles.yaw   = (float)yaw_raw   / 32768.0f * 180.0f;

        jy901s_frame_ready = false;
        
        return true;
    } else {
        return false;
    }
}

float JY901S::getPitch() {
    return _angles.pitch;
}

float JY901S::getRoll() {
    return _angles.roll;
}

float JY901S::getYaw() {
    return _angles.yaw;
}
