#include "encoder.h"
#include "math.h"

Encoder::Encoder(IRQn_Type EXTI_IRQn, IRQn_Type TIMER_IRQn) : 
    _EXTI_IRQn(EXTI_IRQn), _TIMER_IRQn(TIMER_IRQn) {}

void Encoder::begin() {
    NVIC_ClearPendingIRQ(_EXTI_IRQn);
    NVIC_EnableIRQ(_EXTI_IRQn);

    // Timer for speed counting, 50Hz
    NVIC_ClearPendingIRQ(_TIMER_IRQn);
    NVIC_EnableIRQ(_TIMER_IRQn);
    DL_TimerG_startCounter(TIMER_ENC_INST);
}

int32_t Encoder::getCount() const {
    return cnt;
}

void Encoder::update() {
    cnt = abs(cnt_IRQ);
    cnt_integral += abs(cnt_IRQ);

    spd = (float)cnt * CNT2CM_PARAM / SAMPLE_INTERVAL_SEC;
    dist = (float)cnt_integral * CNT2CM_PARAM;

    dir = (cnt_IRQ >= 0) ? Direction::FORWARD : Direction::BACKWARD;

    cnt_IRQ = 0;
}

uint16_t Encoder::getSpeed() const {
    return spd;
}

uint32_t Encoder::getDistance() const {
    return dist;
}

void Encoder::clearDistance() {
    cnt_integral = 0;
    dist = 0;
}

Encoder::Direction Encoder::getDirection() const {
    return dir;
}
