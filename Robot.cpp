#include "Robot.h"

HardwareRegistry hardware;
PWMDiamondDynamicsMotor frontLeftMotor(FRONT_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor frontRightMotor(FRONT_RIGHT_MOTOR_PORT);
PWMDiamondDynamicsMotor backLeftMotor(BACK_LEFT_MOTOR_PORT);
PWMDiamondDynamicsMotor backRightMotor(BACK_RIGHT_MOTOR_PORT);

MPU6050 mpu(IMU_SDA_PIN, IMU_SCL_PIN);

OffboardComputerInterface offboardComputerInterface;


void initialize(){

    
    hardware.add(&frontLeftMotor);
    hardware.add(&frontRightMotor);
    hardware.add(&backLeftMotor);
    hardware.add(&backRightMotor);

    hardware.add(&mpu);
    
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

    int16_t accel_data[3];
    mpu.readAccelData(accel_data);
    OCI::publish_message("robot/data/accel/X", &accel_data[0]);
}