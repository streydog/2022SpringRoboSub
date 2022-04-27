#pragma once

#include "mbed.h"
#include "Initializable.h"

class PWMMotorController : public virtual Initializable {
    public:
        PWMMotorController(PinName pin, int boundMax, int boundMin);
        void set(float speed);
        void set_microseconds(int us);
    protected:
        virtual void initialization_sequence();
        virtual void reset_sequence();
    private:
        PwmOut pwm;
        PinName pin;
        int boundMax;
        int boundMin;
};