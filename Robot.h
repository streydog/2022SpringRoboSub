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
#define IMU_SCL_PIN D15
#define IMU_SDA_PIN D14

#define INT_LOOP_PERIOD_US 20000
#define LOOP_PERIOD_MS 20ms

extern OffboardComputerInterface offboardComputerInterface;

extern void initialize();
extern void loop();