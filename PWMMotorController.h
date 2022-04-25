#pragma once

#include "mbed.h"
#include "Initializable.h"

class PWMMotorController : public Initializable {
    public:
        PWMMotorController(PinName pin, int boundMax, int boundMin);
        void set(float speed);
        void set_microseconds(int us);
    private:
        PwmOut pwm;
        PinName pin;
        int boundMax;
        int boundMin;
};