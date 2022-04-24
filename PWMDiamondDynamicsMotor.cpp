#include "PWMDiamondDynamicsMotor.h"

PWMDiamondDynamicsMotor::PWMDiamondDynamicsMotor(PinName pin) : PWMMotorController(pin, 2000, 1000){}

void PWMDiamondDynamicsMotor::initializationSequence(){
    setMicroseconds(1500);
    ThisThread::sleep_for(2s);
}

void PWMDiamondDynamicsMotor::resetSequence(){
    setMicroseconds(1500); // Just stop motor I guess
    // setMicroseconds(0); maybe could also do this
}