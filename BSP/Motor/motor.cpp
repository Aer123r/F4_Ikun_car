#include "motor.hpp"
#include "string"
//Motor::Motor(Driver *driver,Encoder *encoder){}
Motor::Motor(){}

Motor::~Motor(){}

int64_t Motor::correctCount() {
    assert_param(this->driver->status != Direction_t::STOP);
    assert_param(this->driver != nullptr);
    assert_param(this->controller != nullptr);
    assert_param(this->encoder != nullptr);
    if(var.firstTime) {
        var.prev_count = encoder->GetCount();
        var.firstTime = false;
        return 0;
    }
    int64_t count = encoder->GetCount();
    int64_t cnt;
    if(encoder->htim->Instance == TIM3 || encoder->htim->Instance == TIM8){
        // 递减
        if((var.prev_count-count) > 0 ){
            cnt = var.prev_count-count;
        }else{
            cnt = var.prev_count-count + 65536;
        }
    }else{
        // 递增
        if((var.prev_count-count) < 0 ){
            cnt = count - var.prev_count;
        }else{
            cnt = count - var.prev_count + 65536;
        }
    }
//    if(var.epoch++ == 20){
//        var.epoch=0;
//        std::string s;
//        s = std::to_string(cnt).append("\n");
//        HAL_UART_Transmit(&huart1,(uint8_t *)s.c_str(),s.size(),100);
//    }
    var.prev_count = count;

    controller->PIDController(driver->cnt,cnt);

    // 限幅
    if(controller->pid.output > 20){
        controller->pid.output = 20;
    }else if(controller->pid.output < -20) {
        controller->pid.output = -20;
    }

    driver->SetERR(controller->pid.output);
    return cnt;
}

