#include "encoder.h"
#include <stdlib.h>

void ENCODER_Init(ENCODER_t* enc, IRQn_Type EXTI_IRQn, IRQn_Type TIMER_IRQn) {
    enc->_EXTI_IRQn = EXTI_IRQn;
    enc->_TIMER_IRQn = TIMER_IRQn;
    enc->_cnt = 0;
    enc->_cnt_IRQ = 0;
    enc->_cnt_integral = 0;
    enc->_spd = 0.0;
    enc->_dist = 0.0;
    enc->_dir = ENCODER_DIR_FORWARD;
}

void ENCODER_Begin(ENCODER_t* enc) {
    NVIC_ClearPendingIRQ(enc->_EXTI_IRQn);
    NVIC_EnableIRQ(enc->_EXTI_IRQn);
}

int32_t ENCODER_GetCount(const ENCODER_t* enc) {
    return enc->_cnt;
}

void ENCODER_Update(ENCODER_t* enc) {
    enc->_cnt = abs(enc->_cnt_IRQ);
    enc->_cnt_integral += abs(enc->_cnt_IRQ);

    enc->_spd = (float)enc->_cnt * CNT2CM_PARAM / SAMPLE_INTERVAL_SEC;
    enc->_dist = (float)enc->_cnt_integral * CNT2CM_PARAM;

    enc->_dir = (enc->_cnt_IRQ >= 0) ? ENCODER_DIR_FORWARD : ENCODER_DIR_BACKWARD;

    enc->_cnt_IRQ = 0;
}

float ENCODER_GetSpeed(const ENCODER_t* enc) {
    return enc->_spd;
}

float ENCODER_GetDistance(const ENCODER_t* enc) {
    return enc->_dist;
}

void ENCODER_ClearDistance(ENCODER_t* enc) {
    enc->_cnt_integral = 0;
    enc->_dist = 0;
}

ENCODER_Dir_t ENCODER_GetDirection(const ENCODER_t* enc) {
    return enc->_dir;
}
