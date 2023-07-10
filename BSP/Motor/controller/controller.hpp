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

    typedef struct
    {
        bool kpValid, kiValid, kdValid;
        int32_t kp, ki, kd;
        int32_t vError, vErrorLast;
        int32_t outputKp, outputKi, outputKd;
        int32_t integralRound;
        int32_t integralRemainder;
        int32_t output;
    } PID_t;


};


#endif //F4_IKUN_CAR_CONTROLLER_HPP
