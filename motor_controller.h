#pragma once
#include "mbed.h"

PwmOut m1(PF_9);

void setup_m1(){
    m1.period(0.02f);
    m1.write(0.075f);
}