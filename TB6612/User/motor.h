#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "ti_msp_dl_config.h"

#define PWM_PERIOD_COUNT 1000

class Motor {
    public:
        enum class MOTOR_Dir_t {
            FORWARD,
            BACKWARD,
            STOP
        };

        Motor(GPIO_Regs* in_gpio_port, uint32_t in1_pin, uint32_t in2_pin,
              GPTIMER_Regs* gptimer, DL_TIMER_CC_INDEX cc_index);
        void begin();
        void setDuty(float dutyCycle);
        float getDuty();
        uint32_t __getCCR_for_debug();

    private:
        void setDir(MOTOR_Dir_t dir);

        GPIO_Regs* in_gpio_port_;
        uint32_t in1_pin_;
        uint32_t in2_pin_;
        GPTIMER_Regs* pwm_timer_;
        DL_TIMER_CC_INDEX cc_index_;

        float duty_cycle_;
    
};


#endif