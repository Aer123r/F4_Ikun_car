//
// Created by WuGaoyuan on 2023/7/10.
//

#include "controller.hpp"
#include "string"
void Controller::PIDController(int32_t setpoint, int32_t processVariable)
{
    static int epoch=0;
    pid.vError = setpoint - processVariable;
    pid.integral += pid.vError;
    float derivative = pid.vError-pid.vErrorLast;
    pid.output = int32_t (pid.kp*pid.vError + pid.integral*pid.ki+pid.kd*derivative);
    pid.vErrorLast = pid.vError;

//    if(epoch++ == 20){
//        epoch = 0;
//        std::string s;
//        s = std::to_string(pid.output).append("\n");
//        HAL_UART_Transmit(&huart1,(uint8_t *)s.c_str(),s.size(),100);
//    }
    return;
}

Controller::Controller(int32_t kp, int32_t ki, int32_t kd): pid({
    .kp = kp,
    .ki = ki,
    .kd = kd,
}) {}