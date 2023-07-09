#ifndef MOTOR_H_
#define MOTOR_H_
#include "common_inc.h"
#include "stm32f407xx.h"
#include "stm32f4xx_hal_gpio.h"

enum Direction_t {
    FORWARD,
    BACKWARD,
    STOP,
};

struct Motor_t {
  TIM_HandleTypeDef *htim;
  uint32_t Channel;
  GPIO_TypeDef *PORT_A;
  uint32_t PIN_A;
  GPIO_TypeDef *PORT_B;
  uint32_t PIN_B;
};

class Motor {
private:
  Motor_t m1 = {0};
  Motor_t m2 = {0};
  Motor_t m3 = {0};
  Motor_t m4 = {0};
  void setDirection(Motor_t &m,Direction_t d);
public:
  Motor();
  ~Motor();
  void Init();
  void Move();
  void Stop();
};

#endif // !MOTOR_H_
#define MOTOR_H_
