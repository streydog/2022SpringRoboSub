#pragma once

#include <string>
#include <functional>
#include "Robot.h"

namespace OCI {
    extern float get_speed();
    extern float get_strafe();
    extern float get_rotation();
    extern float get_depth();
    extern void start();
    extern void update_values();
    extern void publish_message(const char* topic_name, void* data);
}