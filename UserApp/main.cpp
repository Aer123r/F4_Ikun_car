#include "common_inc.h"
#include "freertos_inc.h"
#include <cstdio>
#include <stdlib.h>

#include "ArduinoJson.h"
#include "ikunCar.hpp"
#include "string"


Motor motors[4];
Servo servo(&htim9, TIM_CHANNEL_1);
StaticJsonDocument<200> jsonDocument;
Rx_Data_t rx_data;

int64_t pwm;

/* LED Blinking Task */
void LedBlinkyTask(void const *argument) {
    while (1) {
        HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
        osDelay(1000);
    }
}
/**
 * @def IKUN战车控制任务
 * @param argument
 */
void CarControllerTask(void const *argument) {
    servo.Init();
    servo.SetAngle(79);
        ikun::move(motors);
    //    osDelay(3000);
    //    osDelay(2000);
    //    ikun::backMove(motors);
    while (1) {
        osDelay(10);
    }
}
/**
 * @def 舵机控制任务
 * @param argument
 */

void ServoHandleTask(void const *argument) {

    while (1) {

        osDelay(200);
    }
}

/**
 * TODO 处理编码器数据和更加PWM
 * @def 电机控制任务
 */

void MotorSpeedUpdateHandleTask(void const *argument) {
    for (auto &motor : motors) {
        motor.encoder->Start();
    }
    while (1) {
        for (auto &motor : motors) {
            motor.correctCount();
        }

        osDelay(50);
    }
}

/**
 * @name 障碍物识别处理任务
 * @param argument
 * @interface UART
 */
void obstacleDetectionAndProcessingTask(void const *argument) {
    while (1) {
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_data.data, 255);
        if(rx_data.isReceived){
            DeserializationError error = deserializeJson(jsonDocument, rx_data.data);
            if (!error) {
                int blockType = jsonDocument["id"];
                double xBias = jsonDocument["Tx"];
                double distance = jsonDocument["Tz"];
//                HAL_UART_Transmit(&huart2, (uint8_t *)std::to_string(xBias).c_str(), std::to_string(xBias).length(),100);
                if(blockType == 0){
                    // 改进：依据zBias的值进行s
//                     判断为炸弹
                    if(xBias > -4 && xBias < 0 && distance > -6 ){
                        // object is on the right
                        // lift转
                        motors[0].driver->SetCNT(motors[0].driver->cnt + xBias*30); // 左轮
                        motors[1].driver->SetCNT(motors[1].driver->cnt + xBias*30);
                        motors[2].driver->SetCNT(motors[2].driver->cnt - xBias*30);
                        motors[3].driver->SetCNT(motors[3].driver->cnt - xBias*30);
                    }else if(xBias < 4 && xBias > 0 && distance > -6 ){
                        // 右转
                        motors[0].driver->SetCNT(motors[0].driver->cnt - xBias*30); // 左轮
                        motors[1].driver->SetCNT(motors[1].driver->cnt - xBias*30);
                        motors[2].driver->SetCNT(motors[2].driver->cnt + xBias*30);
                        motors[3].driver->SetCNT(motors[3].driver->cnt + xBias*30);
                    }
                    osDelay(1000);
                    for(auto& motor:motors){
                        motor.driver->SetCNT(400);
                    }
                }else if(blockType == 1){
                    //判断为能量块
                    if(xBias > 0.5 && distance > -10){
                        // 右转
                        motors[0].driver->SetCNT(motors[0].driver->cnt - xBias*10); // 左轮
                        motors[1].driver->SetCNT(motors[1].driver->cnt - xBias*10);
                        motors[2].driver->SetCNT(motors[2].driver->cnt + xBias*10);
                        motors[3].driver->SetCNT(motors[3].driver->cnt + xBias*10);
                    }else if(xBias < -0.5  && distance > -10){
                        // 左转
                        motors[0].driver->SetCNT(motors[0].driver->cnt + xBias*10); // 左轮
                        motors[1].driver->SetCNT(motors[1].driver->cnt + xBias*10);
                        motors[2].driver->SetCNT(motors[2].driver->cnt - xBias*10);
                        motors[3].driver->SetCNT(motors[3].driver->cnt - xBias*10);
                    }
                }
            }

            memset(rx_data.data,0,sizeof(rx_data.data));
            rx_data.isReceived = false;
        }
        osDelay(10);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    HAL_UART_Transmit(&huart1, rx_data.data, Size,100);
    rx_data.isReceived = true;

}

/**
 * @name 光电传感器任务
 * @param 触发的IO号
 * @details IO在上升沿和下降沿会触发这个函数
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
    HAL_NVIC_DisableIRQ(EXTI3_IRQn);
    if (GPIO_Pin == GPIO_PIN_3) {
        delay_ms(10);
        if(HAL_GPIO_ReadPin(GPIOC,GPIO_Pin) == GPIO_PIN_RESET) return;
        // 2左红外检测
        // 先后退，再右转
        for(auto& motor:motors){
            motor.driver->SetDirection(Direction_t::BACKWARD);
        }
        delay_ms(1000);
        // 右转，左轮前进右轮后退
        for(int i = 0 ; i < 2 ; i++){
            motors[i+2].driver->SetDirection(Direction_t::FORWARD);
        }
        delay_ms(700);
        for(auto& motor:motors){
            motor.driver->SetDirection(Direction_t::FORWARD);
        }


    } else if(GPIO_Pin == GPIO_PIN_2 ) {
        delay_ms(10);
        if(HAL_GPIO_ReadPin(GPIOC,GPIO_Pin) == GPIO_PIN_RESET) return;
        // 3右红外检测
        for(auto& motor:motors){
            motor.driver->SetDirection(Direction_t::BACKWARD);
        }
        delay_ms(1000);
        // 左转，右轮前进左轮后退
        for(int i = 0 ; i < 2 ; i++){
            motors[i].driver->SetDirection(Direction_t::FORWARD);
        }
        delay_ms(700);
        for(auto& motor:motors){
            motor.driver->SetDirection(Direction_t::FORWARD);
        }
    }
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

/* 主函数 */
void Main() {
    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
    HAL_NVIC_DisableIRQ(EXTI3_IRQn);

    Controller::Config_PID_t config_pid = {
            .kp = 2.00,
            .ki = 0.09,
            .kd = 0.02,
    };
    motors[0].driver = new Driver(Config_Driver_1, 400);
    motors[1].driver = new Driver(Config_Driver_2, 400);
    motors[2].driver = new Driver(Config_Driver_3, 400);
    motors[3].driver = new Driver(Config_Driver_4, 400);

    motors[0].encoder = new Encoder(&htim1);
    motors[1].encoder = new Encoder(&htim3);
    motors[2].encoder = new Encoder(&htim4);
    motors[3].encoder = new Encoder(&htim8);

    motors[0].controller = new Controller(&config_pid);
    motors[1].controller = new Controller(&config_pid);
    motors[2].controller = new Controller(&config_pid);
    motors[3].controller = new Controller(&config_pid);

    for (uint8_t i = 0; i < 4; i++) {
        motors[i].driver->Init();
    }

//    HAL_UART_Transmit_IT(&huart1, (uint8_t *)"hello", 5);
    HAL_UART_Transmit_IT(&huart2, (uint8_t *)"hello", 5);

    osMutexDef(RxDataMutex);
    rxDataMutexHandle = osMutexCreate(osMutex(RxDataMutex));

    osThreadDef(LedBlinkyTask_, LedBlinkyTask, osPriorityNormal, 0, 256);
    ledBlinkyTaskHandle = osThreadCreate(osThread(LedBlinkyTask_), NULL);

    osThreadDef(MotorUpdateTask, MotorSpeedUpdateHandleTask, osPriorityNormal, 0,
                512);
    motorSpeedUpdateTaskHandle = osThreadCreate(osThread(MotorUpdateTask), NULL);

    osThreadDef(ServoTask, ServoHandleTask, osPriorityAboveNormal, 0, 512);
    servoTaskHandle = osThreadCreate(osThread(ServoTask), NULL);

    osThreadDef(ObstacleDetectionTask, obstacleDetectionAndProcessingTask,
                osPriorityHigh, 0, 512);
    obstacleTaskHandle = osThreadCreate(osThread(ObstacleDetectionTask), NULL);

    osThreadDef(CarControllerTask, CarControllerTask, osPriorityHigh, 0, 512);
    carControllerTaskHandle = osThreadCreate(osThread(CarControllerTask), NULL);
}
