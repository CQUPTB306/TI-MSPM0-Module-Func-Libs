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

        JY901S(IRQn_Type JY901S_IRQn);
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
        Angles _angles;
};