#include <Arduino.h>

#define led_pin_1 13
#define led_pin_2 21
#define tap_pin_1 12
#define tap_pin_2 14

// The frequency of the PWM signal
#define PWM_FREQUENCY 500

// The resolution of the PWM signal in bits
#define PWM_RESOLUTION 8

void setup_pwm_duty_cycle(int pin, float duty_cycle) {
    // Clamp the duty cycle value between 0.0 and 1.0
    if (duty_cycle < 0.0) {
        duty_cycle = 0.0;
    } else if (duty_cycle > 1.0) {
        duty_cycle = 1.0;
    }

    // Convert the duty cycle to a value between 0 and the maximum PWM value
    uint32_t pwm_value = (uint32_t)(duty_cycle * (pow(2, PWM_RESOLUTION) - 1));

    // Configure the PWM pin
    ledcSetup(pin, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(pin, pin);

    // Set the duty cycle for the PWM pin
    ledcWrite(pin, pwm_value);
}

void set_pwm_duty_cycle(int pin, float duty_cycle){
    // Clamp the duty cycle value between 0.0 and 1.0
    if (duty_cycle < 0.0) {
        duty_cycle = 0.0;
    } else if (duty_cycle > 1.0) {
        duty_cycle = 1.0;
    }

    // Convert the duty cycle to a value between 0 and the maximum PWM value
    uint32_t pwm_value = (uint32_t)(duty_cycle * (pow(2, PWM_RESOLUTION) - 1));

    // Set the duty cycle for the PWM pin
    ledcWrite(pin, pwm_value);
}
