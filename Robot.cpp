#include "Robot.h"

#include "ThisThread.h"
#include "mbed.h"

#include "HardwareRegistry.h"
#include "PWMDiamondDynamicsMotor.h"
#include "OffboardComputerInterface.h"
#include "Initializable.h"
#include <MQTTClientMbedOs.h>

HardwareRegistry hardware;
PWMDiamondDynamicsMotor frontLeftMotor(FRONT_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor frontRightMotor(FRONT_RIGHT_MOTOR_PORT);
PWMDiamondDynamicsMotor backLeftMotor(BACK_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor backRightMotor(BACK_RIGHT_MOTOR_PORT);

OffboardComputerInterface offboardComputerInterface;


void initialize(){
    /*
    David, 
    
    The following lines of code work initially, but including it makes the OS throw a
    HardFault error and crash after the mqtt_callback function is called. If I comment the following code out,
    the callback function works properly and I get to call it multiple times. 

    I imagine it has something to do with your comment in HardwareRegister::add() that says, "I have no clue if this will work".

    Thanks,
    Nolan
    */

    /*
    hardware.add(frontLeftMotor);
    hardware.add(frontRightMotor);
    hardware.add(backLeftMotor);
    hardware.add(backRightMotor);
    
    hardware.initializeAll();
    */

    offboardComputerInterface.initialize();
}

void loop(){
    float speed = offboardComputerInterface.get_speed();

    // TODO: this is for debug purposes, remove this later. 
    speed *= 1000000;

    printf("speed: %d\n", (int)speed);

    ThisThread::sleep_for(500ms);

    // TODO: I commented this out because of the issue I described in initialize()
    //frontLeftMotor.set(speed);
}