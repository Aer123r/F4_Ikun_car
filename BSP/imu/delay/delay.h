//
// Created by WuGaoyuan on 2023/3/3.
//

#ifndef EE297_V1_DELAY_H
#define EE297_V1_DELAY_H
#include "main.h"
void delay_init(uint8_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
static uint32_t fac_us=0;
#define delay(x) delay_ms(x)
#endif //EE297_V1_DELAY_H
