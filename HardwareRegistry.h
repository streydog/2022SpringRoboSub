#pragma once

#include "Initializable.h"
#include <vector>

class HardwareRegistry {
    public:
        HardwareRegistry();
        void initialize_all();
        void add(Initializable periphery);
    private:
        vector<Initializable> hardware;
};