#include "PWMMotorController.h"

class PWMDiamondDynamicsMotor : public PWMMotorController{
    public:
        PWMDiamondDynamicsMotor(PinName pin);
    protected:
        void initializationSequence() override;
        void resetSequence() override;
};