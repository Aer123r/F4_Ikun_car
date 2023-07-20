//
// Created by WuGaoyuan on 2023/7/10.
//

#ifndef F4_IKUN_CAR_IKUNCAR_HPP
#define F4_IKUN_CAR_IKUNCAR_HPP
#include "common_inc.h"
enum CarStatus_t {
    Normal,
    Stop,
    EnergyBlockEncountered,
    BombBlockEncounter,
    OrdinaryBlockEncounter,
    LeftBoundaryDetection,
    RightBoundaryDetection,
};

typedef struct {
    int blockType;
    double xBias;
    double distance;
}BlockInfo_t;

extern Motor motors[4];
extern CarStatus_t car_status;

typedef struct {
    uint8_t data[255];
    bool isReceived;

} Rx_Data_t;

namespace ikun {
    void move(){
        motors[0].driver->SetDirection(FORWARD);
        motors[1].driver->SetDirection(FORWARD);
        motors[2].driver->SetDirection(FORWARD);
        motors[3].driver->SetDirection(FORWARD);
    }
    void backMove(){
        motors[0].driver->SetDirection(BACKWARD);
        motors[1].driver->SetDirection(BACKWARD);
        motors[2].driver->SetDirection(BACKWARD);
        motors[3].driver->SetDirection(BACKWARD);
    }

    void stop(){
        motors[0].driver->SetDirection(STOP);
        motors[1].driver->SetDirection(STOP);
        motors[2].driver->SetDirection(STOP);
        motors[3].driver->SetDirection(STOP);
    }
    void setCarStatus(CarStatus_t status){
        osMutexWait(carStatusMutexHandle,200);
        car_status = status;
        osMutexRelease(carStatusMutexHandle);
    }
    void doAction(CarStatus_t status){
        switch (status) {
            case CarStatus_t::Stop :
                ikun::stop();
                break;
            case CarStatus_t::Normal :
                for(auto& motor:motors){
                    motor.driver->SetCNT(400);
                }
                ikun::move();
                break;
            case CarStatus_t::BombBlockEncounter :

                break;
            case CarStatus_t::LeftBoundaryDetection :
                // 2左红外检测
                // 先后退，再右转
                ikun::backMove();
                osDelay(700);
                // 右转，左轮前进右轮后退
                for(int i = 0 ; i < 2 ; i++){
                    motors[i].driver->SetDirection(Direction_t::BACKWARD);
                }
                osDelay(700);
                ikun::setCarStatus(CarStatus_t::Normal);
                break;
            case CarStatus_t::RightBoundaryDetection :
                ikun::backMove();
                osDelay(700);
                // 左转，右轮前进左轮后退
                for(int i = 0 ; i < 2 ; i++){
                    motors[i+2].driver->SetDirection(Direction_t::BACKWARD);
                }
                osDelay(700);
                ikun::setCarStatus(CarStatus_t::Normal);
                break;
            default:
                break;
        }
    }
};


#endif //F4_IKUN_CAR_IKUNCAR_HPP
