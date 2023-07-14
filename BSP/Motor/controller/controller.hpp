//
// Created by WuGaoyuan on 2023/7/10.
//

#ifndef F4_IKUN_CAR_CONTROLLER_HPP
#define F4_IKUN_CAR_CONTROLLER_HPP
#include "common_inc.h"
#include "cmath"



class Motor;
class Controller {
public:
    friend Motor;
    typedef struct {
        float kp;
        float ki;
        float kd;
    } Config_PID_t;

    typedef struct
    {
        float kp, ki, kd;
        float vError, vErrorLast;
        float integral;
        int32_t output;
    } PID_t;
    PID_t  pid{0};

    explicit Controller(float kp,float ki,float kd);
    explicit Controller(Config_PID_t *config_pid);
    void PIDController(int32_t setpoint, int32_t processVariable);


};


#endif //F4_IKUN_CAR_CONTROLLER_HPP
