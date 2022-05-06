/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "Robot.h"
#include <chrono>

mbed::Timer loop_timer;

int main()
{   
    initialize();
    loop_timer.start();
    while(true){
        std::chrono::microseconds start_time = loop_timer.elapsed_time();
        OCI::update_values();
        loop();
        std::chrono::microseconds end_time = loop_timer.elapsed_time();
        if(end_time>start_time+LOOP_PERIOD_MS){
            printf("WARNING: Loop time overrun");
        }
        else{
            wait_us(start_time.count()+INT_LOOP_PERIOD_US-loop_timer.elapsed_time().count());
        }
    }
    return 0;

}
