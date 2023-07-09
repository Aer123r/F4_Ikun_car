//
// Created by WuGaoyuan on 2023/7/9.
//

#ifndef F4_IKUN_CAR_SERVO_HPP
#define F4_IKUN_CAR_SERVO_HPP
#include "common_inc.h"

class Servo {
private:
    TIM_HandleTypeDef *htim;
    uint32_t CHANNEL;
    const uint16_t RotationAngle;
    uint32_t CNT;
public:
    Servo(TIM_HandleTypeDef *htim,uint32_t CHANNEL,const uint16_t RotationAngle=270);
    ~Servo();
    void Init();
    void SetAngle(float angle);
};


#endif //F4_IKUN_CAR_SERVO_HPP
