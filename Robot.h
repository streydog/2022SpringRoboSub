#pragma once

#include "ThisThread.h"
#include "mbed.h"

#include "HardwareRegistry.h"
#include "PWMDiamondDynamicsMotor.h"
#include "OffboardComputerInterface.h"
#include "Initializable.h"
#include <MQTTClientMbedOs.h>

#include "OCI.h"

#define FRONT_LEFT_MOTOR_PORT PF_9
#define FRONT_RIGHT_MOTOR_PORT PF_8
#define BACK_LEFT_MOTOR_PORT PF_7
#define BACK_RIGHT_MOTOR_PORT PF_6

extern OffboardComputerInterface offboardComputerInterface;

void initialize();
void loop();