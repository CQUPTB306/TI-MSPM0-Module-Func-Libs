/******************************************************************************
 - jy901s.h
 - Designed by WilliTourt
 - 2025.07.23
 - More info at:
 - https://github.com/CQUPTB306/TI-MSPM0-Module-Func-Libs/tree/main/MPU6050
******************************************************************************/

#pragma once

#include "ti_msp_dl_config.h"

#define JY901S_FRAME_LEN 11

#define JY901S_FRAME_HEAD1 0x55
#define JY901S_FRAME_HEAD2 0x53

class JY901S {
    public:
        struct Angles {
            float roll;
            float pitch;
            float yaw;
        };

        JY901S(IRQn_Type JY901S_IRQn, UART_Regs *UART_INST);
        void begin();
        bool decode();
        float getPitch();
        float getRoll();
        float getYaw();

        uint8_t jy901s_rx_index = 0;
        bool jy901s_frame_ready = false;
        uint8_t jy901s_rx_data[JY901S_FRAME_LEN];

    private:
        IRQn_Type _JY901S_IRQn;
        UART_Regs *_JY901S_INST;
        Angles _angles;
};
