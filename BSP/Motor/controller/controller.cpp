//
// Created by WuGaoyuan on 2023/7/10.
//

#include "controller.hpp"

void Controller::PIDController(int32_t setpoint, int32_t processVariable)
{
    int32_t error = setpoint - processVariable;

    // Proportional term
    int32_t proportional = pid.kp * error;

    // Integral term
    pid.integralRemainder += (pid.ki * error) + pid.integralRound;
    int32_t integral = pid.integralRemainder / 1000;
    pid.integralRemainder = pid.integralRemainder % 1000;

    // Derivative term
    int32_t derivative = pid.kd * (error - pid.vErrorLast);

    // Calculate the output
    pid.output = proportional + integral + derivative;

    // Update variables for the next iteration
    pid.vErrorLast = error;
    pid.vError = error;
}

Controller::Controller(int32_t kp, int32_t ki, int32_t kd): pid({
    .kp = kp,
    .ki = ki,
    .kd = kd,
}) {}