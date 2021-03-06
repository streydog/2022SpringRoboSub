#include "PWMMotorController.h"

PWMMotorController::PWMMotorController(PinName pin, int boundMax, int boundMin) : pwm(pin){
    this->pin = pin;
    pwm.period_ms(20);
    this->boundMax = boundMax;
    this->boundMin = boundMin;
}

void PWMMotorController::initialization_sequence(){
    printf("default PWM pls override");
}

void PWMMotorController::reset_sequence(){
    // nothing
}

void PWMMotorController::set(float speed){
    if(speed > 1.0){
        speed = 1.0;
    }
    if(speed < -1.0){
        speed = 1.0;
    }
    pwm.pulsewidth_us(boundMin + (float(boundMax - boundMin) / 2.0) * (speed + 1.0));
}

void PWMMotorController::set_microseconds(int us){
    pwm.pulsewidth_us(us);
}
