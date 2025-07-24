/******************************************************************************
 - motor.cpp
 - Designed by WilliTourt
 - 2025.07.23
 - More info at:
 - https://github.com/CQUPTB306/TI-MSPM0-Module-Func-Libs/blob/main/TB6612
******************************************************************************/

#include "motor.h"

/**
 * @brief  Construct a new Motor object
 * @param  in_gpio_port GPIO port for direction control
 * @param  in1_pin IN1 pin number for direction control
 * @param  in2_pin IN2 pin number for direction control
 * @param  gptimer Timer used for PWM generation
 * @param  pwm_pin PWM output pin
 * @param  cc_index Capture/Compare channel index
 * @retval None
 */
Motor::Motor(GPIO_Regs* in_gpio_port, uint32_t in1_pin, uint32_t in2_pin, GPTIMER_Regs *gptimer, DL_TIMER_CC_INDEX cc_index)
    : in_gpio_port_(in_gpio_port), in1_pin_(in1_pin), in2_pin_(in2_pin),
      pwm_timer_(gptimer), cc_index_(cc_index), 
      ccr_val_(0), duty_cycle_(0.0) {}

/**
 * @brief  Initialize the motor controller
 * @param  None
 * @retval None
 */
void Motor::begin() {
    DL_TimerG_startCounter(pwm_timer_);
}

/**
 * @brief  Set motor duty cycle
 * @param  dutyCycle Duty cycle in percentage (-100% to 100%).
 *         Negative values for reverse direction
 * @retval None
 */
void Motor::setDuty(float dutyCycle) {
    duty_cycle_ = dutyCycle;
    if (dutyCycle > 0.0) {
        setDir(MOTOR_Dir_t::FORWARD);
    } else if (dutyCycle == 0.0) {
        setDir(MOTOR_Dir_t::STOP);
    } else {
        setDir(MOTOR_Dir_t::BACKWARD);
        dutyCycle = -dutyCycle;
    }
    if (dutyCycle > 100.0) { dutyCycle = 100.0; }
    
    uint16_t ccr = (uint16_t)((dutyCycle / 100.0) * PWM_PERIOD_COUNT);

    DL_TimerA_setCaptureCompareValue(pwm_timer_, ccr, cc_index_);
}

/**
 * @brief  Set motor direction
 * @param  dir Motor direction (FORWARD/BACKWARD/STOP)
 * @retval None
 */
void Motor::setDir(MOTOR_Dir_t dir) {
    switch (dir) {
        case MOTOR_Dir_t::FORWARD:
            DL_GPIO_setPins(in_gpio_port_, in1_pin_);
            DL_GPIO_clearPins(in_gpio_port_, in2_pin_);
            break;
        case MOTOR_Dir_t::BACKWARD:
            DL_GPIO_setPins(in_gpio_port_, in2_pin_);
            DL_GPIO_clearPins(in_gpio_port_, in1_pin_);
            break;
        case MOTOR_Dir_t::STOP:
            DL_GPIO_clearPins(in_gpio_port_, in1_pin_);
            DL_GPIO_clearPins(in_gpio_port_, in2_pin_);
            break;
    }
}

/**
 * @brief  Get current duty cycle
 * @param  None
 * @retval float Current duty cycle (-100% to 100%)
 */
float Motor::getDuty() {
    return duty_cycle_;
}

/**
 * @brief  Get current CCR value (for debug purposes)
 * @param  None
 * @retval uint32_t Current CCR register value
 */
uint32_t Motor::__getCCR_for_debug() {
    return DL_Timer_getCaptureCompareValue(pwm_timer_, cc_index_);
}
