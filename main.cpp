/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "Robot.h"


int main()
{   
    initialize();
    while(true){
        OCI::update_values();
        loop();
    }
    return 0;

}


