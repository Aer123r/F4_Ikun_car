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


    Motor();
    ~Motor();



};

#endif // !MOTOR_HPP_
#define MOTOR_H_
