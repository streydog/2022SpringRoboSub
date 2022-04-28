#include "PIDController.h"
#include <ctime>

PIDController::PIDController(float Kp, float Ki, float Kd){
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->setpoint = 0.0;
    this->last_calculate_time = 0;
    this->last_error = 0.0;
    this->total_error = 0.0;
    timer.start();
}

void PIDController::set_setpoint(double setpoint){
    this->setpoint = setpoint;
}

void PIDController::set_gains(float Kp, float Ki, float Kd){
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

void PIDController::set_p(float Kp){
    this->Kp = Kp;
}

void PIDController::set_i(float Ki){
    this->Ki = Ki;
}

void PIDController::set_d(float Kd){
    this->Kd = Kd;
}

void PIDController::reset(){
    this->last_calculate_time = 0;
    this->last_error = 0.0;
    this->total_error = 0.0;
}

float PIDController::calculate(double measurement){
    if(last_calculate_time == 0){
        last_calculate_time = timer.elapsed_time().count();
    }
    int curr_time = timer.elapsed_time().count();
    int time_since_last = curr_time - last_calculate_time;

    double error = setpoint - measurement;
    total_error += error * time_since_last;
    double derivative_error = (error-last_error)/time_since_last;

    last_calculate_time = curr_time;
    last_error = error;

    return (Kp*error) + (Ki*total_error) + (Kd*derivative_error);
}