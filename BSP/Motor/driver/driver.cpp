//
// Created by WuGaoyuan on 2023/7/9.
//

#include "driver.hpp"
Driver::Driver(Driver_t driver,int64_t cnt): driver(driver),cnt(cnt) {}

Driver::~Driver() {}

void Driver::Init() {
    assert_param(driver != nullptr);
    HAL_TIM_PWM_Start(driver.htim,driver.Channel);
            __HAL_TIM_SetCompare(this->driver.htim, this->driver.Channel, this->cnt);
}

void Driver::SetDirection(Direction_t d) {
    GPIO_PinState A_pinStatus;
    GPIO_PinState B_pinStatus;
    switch (d) {
        case FORWARD:
           A_pinStatus = GPIO_PIN_SET;
           B_pinStatus = GPIO_PIN_RESET;
            break;
        case BACKWARD:
            A_pinStatus = GPIO_PIN_RESET;
            B_pinStatus = GPIO_PIN_SET;
            break;
        case STOP:
            A_pinStatus = GPIO_PIN_RESET;
            B_pinStatus = GPIO_PIN_RESET;
            break;
    }
    HAL_GPIO_WritePin(driver.PORT_A, driver.PIN_A, A_pinStatus);
    HAL_GPIO_WritePin(driver.PORT_B, driver.PIN_B, B_pinStatus);
}

void Driver::SetCNT(int64_t cnt) {
    this->cnt = cnt;
            __HAL_TIM_SetCompare(this->driver.htim, this->driver.Channel, this->cnt);
}

int64_t Driver::GetCNT() {
    return this->cnt;
}