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


};
#endif //F4_IKUN_CAR_IKUNCAR_HPP
