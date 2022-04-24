#pragma once

#include "mbed.h"

class Initializable {
    public:
        Initializable();
        void initialize();
        bool isInitialized() const;
        void reset();
    protected:
        virtual void initializationSequence();
        virtual void resetSequence();
    private:
        bool initialized;
        bool sequenceStarted;
};