#include "motor.hpp"
#include "stm32f4xx.h"

Motor::Motor(){};
Motor::~Motor() {};

void Motor::Init() {
    m1 = {
            .htim = &htim2,
            .Channel = TIM_CHANNEL_3,
            MOTOR1_A_PORT,
            MOTOR1_A_PIN,
            MOTOR1_B_PORT,
            MOTOR1_B_PIN
    };
    m2 = {
            .htim = &htim2,
            .Channel = TIM_CHANNEL_2,
            MOTOR2_A_PORT,
            MOTOR2_A_PIN,
            MOTOR2_B_PORT,
            MOTOR2_B_PIN
    };
    m3 = {
            .htim = &htim5,
            .Channel = TIM_CHANNEL_3,
            MOTOR3_A_PORT,
            MOTOR3_A_PIN,
            MOTOR3_B_PORT,
            MOTOR3_B_PIN
    };
    m4 = {
            .htim = &htim5,
            .Channel = TIM_CHANNEL_2,
            MOTOR4_A_PORT,
            MOTOR4_A_PIN,
            MOTOR4_B_PORT,
            MOTOR4_B_PIN
    };
    // TODO m1,m2,m3的初始化
    HAL_TIM_PWM_Start(m1.htim, m1.Channel);
    HAL_TIM_PWM_Start(m2.htim, m2.Channel);
    HAL_TIM_PWM_Start(m3.htim, m3.Channel);
    HAL_TIM_PWM_Start(m4.htim, m4.Channel);
}

// TODO
void Motor::Move() {
            __HAL_TIM_SetCompare(m1.htim, m1.Channel, 0.4 * 1000);
    setDirection(this->m1,FORWARD);
            __HAL_TIM_SetCompare(m2.htim, m2.Channel, 0.4 * 1000);
    setDirection(this->m2,FORWARD);
            __HAL_TIM_SetCompare(m3.htim, m3.Channel, 0.4 * 1000);
    setDirection(this->m3,FORWARD);
            __HAL_TIM_SetCompare(m4.htim, m4.Channel, 0.4 * 1000);
    setDirection(this->m4,FORWARD);
}

void Motor::setDirection(Motor_t &m,Direction_t d) {
    switch (d) {
        case FORWARD:
            HAL_GPIO_WritePin(m.PORT_A, m.PIN_A, GPIO_PIN_SET);
            HAL_GPIO_WritePin(m.PORT_B, m.PIN_B, GPIO_PIN_RESET);
            break;
        case BACKWARD:
            HAL_GPIO_WritePin(m.PORT_A, m.PIN_A, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(m.PORT_B, m.PIN_B, GPIO_PIN_SET);
            break;
        case STOP:
            HAL_GPIO_WritePin(m.PORT_A, m.PIN_A, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(m.PORT_B, m.PIN_B, GPIO_PIN_RESET);
            break;
    }
}
