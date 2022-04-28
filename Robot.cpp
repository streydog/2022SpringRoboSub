#include "Robot.h"

#include "ThisThread.h"
#include "mbed.h"

#include "HardwareRegistry.h"
#include "PWMDiamondDynamicsMotor.h"
#include "OffboardComputerInterface.h"
#include "Initializable.h"
#include <MQTTClientMbedOs.h>

#include "OCI.h"

HardwareRegistry hardware;
PWMDiamondDynamicsMotor frontLeftMotor(FRONT_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor frontRightMotor(FRONT_RIGHT_MOTOR_PORT);
PWMDiamondDynamicsMotor backLeftMotor(BACK_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor backRightMotor(BACK_RIGHT_MOTOR_PORT);

OffboardComputerInterface offboardComputerInterface;


void initialize(){

    
    hardware.add(&frontLeftMotor);
    
    hardware.initialize_all();
    

    //offboardComputerInterface.initialize();

    OCI::start();
}

void loop(){
    //float speed = offboardComputerInterface.get_speed();

    // TODO: this is for debug purposes, remove this later. 
    //speed *= 1000000;

    //printf("speed: %d\n", (int)speed);

    //ThisThread::sleep_for(500ms);

    frontLeftMotor.set(OCI::get_speed());

    // TODO: I commented this out because of the issue I described in initialize()
    //frontLeftMotor.set(speed);
}