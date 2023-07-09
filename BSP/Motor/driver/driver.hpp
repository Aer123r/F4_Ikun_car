//
// Created by WuGaoyuan on 2023/7/9.
//

#ifndef F4_IKUN_CAR_DRIVER_H
#define F4_IKUN_CAR_DRIVER_H
#include "common_inc.h"

struct Driver_t {
    TIM_HandleTypeDef *htim;
    uint32_t Channel;
    GPIO_TypeDef *PORT_A;
    uint32_t PIN_A;
    GPIO_TypeDef *PORT_B;
    uint32_t PIN_B;
};

class Driver {
private:
    Driver_t driver = {0};

public:
    Driver();
    ~Driver();
    void setDirection(Motor_t &m,Direction_t d);
};


#endif //F4_IKUN_CAR_DRIVER_H
