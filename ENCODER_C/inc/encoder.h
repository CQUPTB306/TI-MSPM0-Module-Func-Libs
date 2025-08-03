#pragma once

#include "ti_msp_dl_config.h"

#define HALL_ENC_PULSE_PER_TURN 26.0f
#define MOTOR_REDUCTION_RATIO 28.0f
#define WHEEL_PERIMETER 21.6f

#define CNT2CM_PARAM (WHEEL_PERIMETER / HALL_ENC_PULSE_PER_TURN / MOTOR_REDUCTION_RATIO)
#define SAMPLE_INTERVAL_SEC 0.02f

// 方向枚举定义
typedef enum {
    ENCODER_DIR_BACKWARD,
    ENCODER_DIR_FORWARD
} ENCODER_Dir_t;

// ENCODER结构体定义
typedef struct {
    IRQn_Type _EXTI_IRQn;
    IRQn_Type _TIMER_IRQn;
    int16_t _cnt;
    int16_t _cnt_IRQ;
    uint32_t _cnt_integral;
    float _spd;
    float _dist;
    ENCODER_Dir_t _dir;
} ENCODER_t;

// 初始化函数
void ENCODER_Init(ENCODER_t* enc, IRQn_Type EXTI_IRQn, IRQn_Type TIMER_IRQn);
void ENCODER_Begin(ENCODER_t* enc);
int32_t ENCODER_GetCount(const ENCODER_t* enc);
float ENCODER_GetSpeed(const ENCODER_t* enc);
float ENCODER_GetDistance(const ENCODER_t* enc);
void ENCODER_ClearDistance(ENCODER_t* enc);
ENCODER_Dir_t ENCODER_GetDirection(const ENCODER_t* enc);
void ENCODER_Update(ENCODER_t* enc);
