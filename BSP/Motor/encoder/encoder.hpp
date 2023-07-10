//
// Created by WuGaoyuan on 2023/7/8.
//

#ifndef F4_IKUN_CAR_ENCODER_HPP
#define F4_IKUN_CAR_ENCODER_HPP
#include "common_inc.h"
class Motor;
class Encoder
{
private:
    const float RAD_TO_DEG = 57.295777754771045f;

    TIM_HandleTypeDef *htim;

public:
    friend Motor;
    explicit Encoder(TIM_HandleTypeDef *_htim, uint16_t _cpr = 4096, bool _inverse = false);

    void Start();

    int64_t GetCount();

    float GetAngle(bool _useRAD = false);


    struct Config_t
    {
        uint16_t cpr;

        bool inverse;
    };

    Config_t config;
};

#endif //F4_IKUN_CAR_ENCODER_HPP
