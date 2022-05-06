#pragma once

#include "ThisThread.h"
#include "mbed.h"
#include <MQTTClientMbedOs.h>

#include "HardwareRegistry.h"
#include "PWMDiamondDynamicsMotor.h"
#include "OffboardComputerInterface.h"
#include "Initializable.h"
#include "OCI.h"
#include "PIDController.h"
#include "MPU6050IMU.h"

#define FRONT_LEFT_MOTOR_PORT PF_9
#define FRONT_RIGHT_MOTOR_PORT PF_8
#define BACK_LEFT_MOTOR_PORT PF_7
#define BACK_RIGHT_MOTOR_PORT PE_6
// TODO: Add actual pin names
#define IMU_SCL_PIN PE_4
#define IMU_SDA_PIN PF_4

extern OffboardComputerInterface offboardComputerInterface;

extern void initialize();
extern void loop();