#pragma once

#include "ti_msp_dl_config.h"

#define HALL_ENC_PULSE_PER_TURN 52.0f
#define MOTOR_REDUCTION_RATIO 34.014f
#define WHEEL_PERIMETER 18.84955592f

#define CNT2CM_PARAM (WHEEL_PERIMETER / HALL_ENC_PULSE_PER_TURN / MOTOR_REDUCTION_RATIO)
#define SAMPLE_INTERVAL_SEC 0.02f

class Encoder {
    public:
        enum class Direction {
            BACKWARD,
            FORWARD
        };

        Encoder(IRQn_Type EXTI_IRQn, IRQn_Type TIMER_IRQn);

        void begin();
        int32_t getCount() const;
        uint16_t getSpeed() const;
        uint32_t getDistance() const;
        void clearDistance();
        Direction getDirection() const;

        void update();

        volatile int32_t cnt_IRQ = 0;

    private:
        IRQn_Type _EXTI_IRQn;
        IRQn_Type _TIMER_IRQn;
        
        int32_t cnt = 0;
        uint32_t cnt_integral = 0;
        uint16_t spd = 0;
        uint32_t dist = 0;
        Direction dir = Direction::FORWARD;
};
