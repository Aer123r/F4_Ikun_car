#ifndef MOTOR_HPP_
#define MOTOR_HPP_
#include "common_inc.h"
class Driver;
class Encoder;
class Controller;
class Motor {

public:
    Driver *driver = nullptr;
    Encoder *encoder = nullptr;
    Controller *controller = nullptr;

    typedef struct {
      bool firstTime;
      int32_t epoch;
      int64_t prev_count;
    } Var;
    Var var = {
            .firstTime = true,
            .epoch = 0,
            .prev_count = 0,
    };
    Motor();
    ~Motor();

    int64_t correctCount();

};

#endif // !MOTOR_HPP_
#define MOTOR_H_
