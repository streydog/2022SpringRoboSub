#pragma once

#include "PWMMotorController.h"
#include "Initializable.h"

class PWMDiamondDynamicsMotor : public PWMMotorController{
    public:
        PWMDiamondDynamicsMotor(PinName pin);
    protected:
        void initialization_sequence() override;
        void reset_sequence() override;
};