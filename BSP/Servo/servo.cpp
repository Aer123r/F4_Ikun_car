//
// Created by WuGaoyuan on 2023/7/9.
//

#include "servo.hpp"

Servo::Servo(TIM_HandleTypeDef *htim, uint32_t CHANNEL, const uint16_t RotationAngle):htim(htim),CHANNEL(CHANNEL),RotationAngle(RotationAngle) {}

Servo::~Servo() {}

void Servo::Init() {
    HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
}

void Servo::SetAngle(float angle) {
    uint32_t setCount = (500+angle*(2000.0/RotationAngle));
    __HAL_TIM_SET_COMPARE(htim,CHANNEL,setCount);
}