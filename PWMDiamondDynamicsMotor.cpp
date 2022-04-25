#include "PWMDiamondDynamicsMotor.h"
#include "Initializable.h"

PWMDiamondDynamicsMotor::PWMDiamondDynamicsMotor(PinName pin) : PWMMotorController(pin, 2000, 1000){}

void PWMDiamondDynamicsMotor::initialization_sequence(){
    set_microseconds(1500);
    ThisThread::sleep_for(2s);
}

void PWMDiamondDynamicsMotor::reset_sequence(){
    set_microseconds(1500); // Just stop motor I guess
    // setMicroseconds(0); maybe could also do this
}