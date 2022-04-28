#include "Timer.h"

class PIDController{
    public:
        PIDController(float Kp, float Ki, float Kd);
        void set_setpoint(double setpoint);
        void set_gains(float Kp, float Ki, float Kd);
        void set_p(float Kp);
        void set_i(float Ki);
        void set_d(float Kd);
        void reset();
        float calculate(double measurement);
    private:
        mbed::Timer timer;
        float Kp;
        float Ki;
        float Kd;
        double setpoint;
        double total_error;
        int last_calculate_time;
        double last_error;
};