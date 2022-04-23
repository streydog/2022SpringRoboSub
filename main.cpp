

/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */


#include <csetjmp>
#include <cstring>
#include <stdio.h>

#include "mbed.h"
#include "mpu6050.h"
#include "motor_controller.h"

#include <MQTTClientMbedOs.h>

#include "EthernetInterface.h"

#include "test.h"


int main()
{
    run_test();
    

    return 0;

}


