#include "Robot.h"

#include "mbed.h"

#include "HardwareRegistry.h"
#include "PWMDiamondDynamicsMotor.h"
#include "Controller.h"

#define FRONT_LEFT_MOTOR_PORT PF_9
#define FRONT_RIGHT_MOTOR_PORT PF_8
#define BACK_LEFT_MOTOR_PORT PF_7
#define BACK_RIGHT_MOTOR_PORT PF_6

HardwareRegistry hardware;
PWMDiamondDynamicsMotor frontLeftMotor(FRONT_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor frontRightMotor(FRONT_RIGHT_MOTOR_PORT);
PWMDiamondDynamicsMotor backLeftMotor(BACK_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor backRightMotor(BACK_RIGHT_MOTOR_PORT);

Controller controller;

void initialize(){
    hardware.add(frontLeftMotor);
    hardware.add(frontRightMotor);
    hardware.add(backLeftMotor);
    hardware.add(backRightMotor);

    hardware.initializeAll();
}

void loop(){
    float speed = controller.getSpeed();
    frontLeftMotor.set(speed);
}