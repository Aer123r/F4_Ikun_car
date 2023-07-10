//
// Created by WuGaoyuan on 2023/7/9.
//

#ifndef F4_IKUN_CAR_DRIVER_H
#define F4_IKUN_CAR_DRIVER_H
#include "common_inc.h"

#define Config_Driver_1     {\
                                &htim2,\
                                TIM_CHANNEL_3,\
                                MOTOR1_A_PORT,\
                                MOTOR1_A_PIN,\
                                MOTOR1_B_PORT,\
                                MOTOR1_B_PIN \
                             }

#define Config_Driver_2     {\
                                &htim2,\
                                TIM_CHANNEL_2,\
                                MOTOR2_A_PORT,\
                                MOTOR2_A_PIN,\
                                MOTOR2_B_PORT,\
                                MOTOR2_B_PIN\
                            }

#define Config_Driver_3     {\
                                &htim5,\
                                TIM_CHANNEL_3,\
                                MOTOR3_A_PORT,\
                                MOTOR3_A_PIN,\
                                MOTOR3_B_PORT,\
                                MOTOR3_B_PIN\
                            }

#define Config_Driver_4     {\
                                &htim5,\
                                TIM_CHANNEL_2,\
                                MOTOR4_A_PORT,\
                                MOTOR4_A_PIN,\
                                MOTOR4_B_PORT,\
                                MOTOR4_B_PIN\
                            }
enum Direction_t {
    FORWARD,
    BACKWARD,
    STOP,
};


class Motor;
class Driver {
public:
    friend Motor;

    typedef struct {
        TIM_HandleTypeDef *htim;
        uint32_t Channel;
        GPIO_TypeDef *PORT_A;
        uint32_t PIN_A;
        GPIO_TypeDef *PORT_B;
        uint32_t PIN_B;
    } Driver_t;
    Driver_t driver{0};

    int64_t cnt;
    explicit Driver(Driver_t driver,int64_t cnt = 200);
    ~Driver();

    void Init();
    void SetDirection(Direction_t d);
    void SetCNT(int64_t cnt);
    int64_t GetCNT();
};


#endif //F4_IKUN_CAR_DRIVER_H
