//
// Created by WuGaoyuan on 2023/7/10.
//

#ifndef F4_IKUN_CAR_DELAY_H
#define F4_IKUN_CAR_DELAY_H

#include "main.h"
void delay_init(uint8_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
static uint32_t fac_us=0;


#endif //F4_IKUN_CAR_DELAY_H
