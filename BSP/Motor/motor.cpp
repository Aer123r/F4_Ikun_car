#include "motor.hpp"

//Motor::Motor(Driver *driver,Encoder *encoder){}
Motor::Motor(){}

Motor::~Motor(){}

void Motor::correctCount() {
    assert_param(this->driver != nullptr);
    assert_param(this->controller != nullptr);
    assert_param(this->encoder != nullptr);
    static bool firstTime = true;
    static int64_t prev_count = 0;
    if(firstTime) {
        prev_count = encoder->GetCount();
        firstTime = false;
        return;
    }


    int64_t setPointCount = this->driver->cnt;
    int32_t processVariable = this->encoder->GetCount();

    this->controller->PIDController(setPointCount,processVariable);

}

