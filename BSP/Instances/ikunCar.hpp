//
// Created by WuGaoyuan on 2023/7/10.
//

#ifndef F4_IKUN_CAR_IKUNCAR_HPP
#define F4_IKUN_CAR_IKUNCAR_HPP
#include "common_inc.h"
namespace ikun {
    void move(Motor *motors){
        motors[0].driver->SetDirection(FORWARD);
        motors[1].driver->SetDirection(FORWARD);
        motors[2].driver->SetDirection(FORWARD);
        motors[3].driver->SetDirection(FORWARD);
    }
    void stop(Motor *motors){
        motors[0].driver->SetDirection(STOP);
        motors[1].driver->SetDirection(STOP);
        motors[2].driver->SetDirection(STOP);
        motors[3].driver->SetDirection(STOP);
    }
    void DEBUG_PRINT(std::string s){

    }
};
#endif //F4_IKUN_CAR_IKUNCAR_HPP
