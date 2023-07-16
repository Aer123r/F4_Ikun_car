//
// Created by WuGaoyuan on 2023/7/10.
//

#include "controller.hpp"
#include "string"
//位置式
void Controller::LocationPIDController(float setpoint, float processVariable) {
//    static int epoch=0;
    pid.vError = setpoint - processVariable;
    pid.integral += pid.vError;
    float derivative = pid.vError - pid.vErrorLast;
    pid.output2 = pid.kp * pid.vError
                  + pid.integral * pid.ki
                  +pid.kd * derivative;
    pid.vErrorLast = pid.vError;

//    if(epoch++ == 20){
//        epoch = 0;
//        std::string s;
//        s = std::to_string(pid.output).append("\n");
//        HAL_UART_Transmit(&huart1,(uint8_t *)s.c_str(),s.size(),100);
//    }
    return;
}

void Controller::PIDController(int32_t setpoint, int32_t processVariable) {
    pid.vError = setpoint-processVariable;
    pid.output = (pid.kp * pid.vError) - (pid.ki*pid.vErrorLast)+(pid.kd * pid.vError2);
    pid.vError2 = pid.vErrorLast;
    pid.vErrorLast = pid.vError;
}
Controller::Controller(float kp, float ki, float kd): pid({
    .kp = kp,
    .ki = ki,
    .kd = kd,
}) {}

Controller::Controller(Config_PID_t *config_pid): pid({
    .kp = config_pid->kp,
    .ki = config_pid->ki,
    .kd = config_pid->kd,
}) {}