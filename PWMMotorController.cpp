#include "PWMMotorController.h"

PWMMotorController::PWMMotorController(PinName pin, int boundMax, int boundMin) : pwm(pin){
    this->pin = pin;
    pwm.period_ms(20);
    this->boundMax = boundMax;
    this->boundMin = boundMin;
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

void PWMMotorController::setMicroseconds(int us){
    pwm.pulsewidth_us(us);
}
