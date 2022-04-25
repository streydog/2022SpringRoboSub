#pragma once

#include "mbed.h"

class Initializable {
    public:
        Initializable();
        void initialize();
        bool is_initialized() const;
        void reset();
    protected:
        virtual void initialization_sequence();
        virtual void reset_sequence();
    private:
        bool initialized;
        bool sequenceStarted;
};