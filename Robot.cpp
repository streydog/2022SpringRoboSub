#include "Robot.h"

HardwareRegistry hardware;
PWMDiamondDynamicsMotor frontLeftMotor(FRONT_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor frontRightMotor(FRONT_RIGHT_MOTOR_PORT);
PWMDiamondDynamicsMotor backLeftMotor(BACK_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor backRightMotor(BACK_RIGHT_MOTOR_PORT);

OffboardComputerInterface offboardComputerInterface;


void initialize(){

    
    hardware.add(&frontLeftMotor);
    hardware.add(&frontRightMotor);
    hardware.add(&backLeftMotor);
    hardware.add(&backRightMotor);
    
    hardware.initialize_all();
    

    //offboardComputerInterface.initialize();

    OCI::start();
}

void loop(){
    //float speed = offboardComputerInterface.get_speed();

    float speed = OCI::get_speed();
    float strafe = OCI::get_strafe();
    float rotation = OCI::get_rotation();

    frontLeftMotor.set(speed+strafe+rotation);
    frontRightMotor.set(speed-strafe-rotation);
    backLeftMotor.set(-speed+strafe-rotation);
    backRightMotor.set(-speed-strafe+rotation);
}