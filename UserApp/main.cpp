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
Rx_Data_t rx_cmd;
Controller *LocationPIDController = new Controller(10.0,0.5,0.1);

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
    servo.SetAngle(120); // 79
    ikun::stop();
    // 开启定时器
    while (1) {
//        osMutexWait(carStatusMutexHandle,HAL_MAX_DELAY);

//        osMutexRelease(carStatusMutexHandle);
        osDelay(1);
    }
}

void timerTask(void const *argument){
    osMutexWait(carStatusMutexHandle,HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)"offset reset\r\n",14,100);
    for(auto& motor:motors){
        motor.driver->SetDirection(Direction_t::FORWARD);
        motor.driver->SetOffset(0);
    }
    LocationPIDController->pid.vErrorLast = 0;
    LocationPIDController->pid.vError2 = 0;
    osMutexRelease(carStatusMutexHandle);
}
/**
 * @def 舵机控制任务
 * @param argument
 */

void ServoHandleTask(void const *argument) {
    while (1) {
        osDelay(1);
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
        HAL_UARTEx_ReceiveToIdle_IT(&huart2,rx_cmd.data,255);
        if(rx_data.isReceived){
            DeserializationError error = deserializeJson(jsonDocument, rx_data.data);
            if (!error) {
                int blockType = jsonDocument["id"];
                double xBias = jsonDocument["Tx"];
                double distance = jsonDocument["Tz"];
                std::string InfoStr = "blockType:"+std::to_string(blockType)+"\t"
                                        +"xBias:"+std::to_string(xBias)+"\t"
                                        +"distance:"+std::to_string(distance)+"\r\n";
                HAL_UART_Transmit_IT(&huart2, (uint8_t*)InfoStr.c_str(), InfoStr.length());

                osMutexWait(carStatusMutexHandle,HAL_MAX_DELAY);
                if(blockType == 0){
                    // 改进：依据zBias的值进行s
//                     判断为炸弹
                    if(xBias > -5 && xBias < 0 && distance > -6 ){
                        // 左转 -》 右轮01前进左轮后退23
                        for(int i = 0 ; i < 2 ; i++){
                            motors[i].driver->SetDirection(Direction_t::BACKWARD);
                        }
                        osDelay(500);
                        for(int i = 0 ; i < 2 ; i++){
                            motors[i].driver->SetDirection(Direction_t::FORWARD);
                        }
                    }else if(xBias < 5 && xBias > 0 && distance > -6 ){
                        // 右转 -》 右轮01后退 左轮23前进
                        // DELAY 1s正好是掉头，掉头会导致行走路劲重复
                        for(int i = 0 ; i < 2 ; i++){
                            motors[i+2].driver->SetDirection(Direction_t::BACKWARD);
                        }
                        osDelay(500);
                        for(int i = 0 ; i < 2 ; i++){
                            motors[i+2].driver->SetDirection(Direction_t::FORWARD);
                        }
                    }
                }else if(blockType == 1){
                    LocationPIDController->LocationPIDController(0,xBias);
                    float output = LocationPIDController->pid.output2;
                    std::string outputStr = "output:"+std::to_string(output)+"\r\n";
                    HAL_UART_Transmit_IT(&huart2, (uint8_t*)outputStr.c_str(), outputStr.length());
                    //判断为能量块
                    motors[0].driver->SetOffset(int64_t(-output*2.5));
                    motors[1].driver->SetOffset(int64_t(-output*2.5));
                    motors[1].driver->SetOffset(int64_t(output*2.5));
                    motors[1].driver->SetOffset(int64_t(output*2.5));
                    osTimerStop(timerId);
                    osTimerStart(timerId,300);
                }
                osMutexRelease(carStatusMutexHandle);
            }
            memset(rx_data.data,0,sizeof(rx_data.data));
            rx_data.isReceived = false;
        }
        if(rx_cmd.isReceived){
            if(rx_cmd.data[0] == '0'){
                ikun::stop();
            }else if(rx_cmd.data[0] == '1'){
                ikun::move();
            }
            memset(rx_cmd.data,0,sizeof(rx_cmd.data));
            rx_cmd.isReceived = false;
        }
        osDelay(10);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if(huart->Instance == USART1){rx_data.isReceived = true;}
    else if(huart->Instance == USART2){rx_cmd.isReceived = true;}
}

/**
 * @name 光电传感器任务
 * @param 触发的IO号
 * @details IO在上升沿和下降沿会触发这个函数
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_3) {
        delay_ms(10);
        if(HAL_GPIO_ReadPin(GPIOC,GPIO_Pin) == GPIO_PIN_RESET) return;
        // 2左红外检测
        // 先后退，再右转
        for(auto& motor:motors){
            motor.driver->SetDirection(Direction_t::BACKWARD);
        }
        delay_ms(500);
        // 右转，左轮前进右轮后退
        for(int i = 0 ; i < 2 ; i++){
            motors[i+2].driver->SetDirection(Direction_t::FORWARD);
        }

    } else if(GPIO_Pin == GPIO_PIN_2 ) {
        delay_ms(10);
        if(HAL_GPIO_ReadPin(GPIOC,GPIO_Pin) == GPIO_PIN_RESET) return;

        // 3右红外检测
        for(auto& motor:motors){
            motor.driver->SetDirection(Direction_t::BACKWARD);
        }
        delay_ms(500);
        // 左转，右轮前进左轮后退
        for(int i = 0 ; i < 2 ; i++){
            motors[i].driver->SetDirection(Direction_t::FORWARD);
        }
    }
    delay_ms(500);

    for(auto& motor:motors){
        motor.driver->SetCNT(400);
        motor.driver->SetDirection(FORWARD);
    }

}

/* 主函数 */
void Main() {
//    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
//    HAL_NVIC_DisableIRQ(EXTI3_IRQn);
    Controller::Config_PID_t config_pid = {
            .kp = 1.00,
            .ki = 0.29,
            .kd = 0.12,
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
    HAL_UART_Transmit_IT(&huart2, (uint8_t *)"hello\n", 6);

    osTimerDef(timer1,timerTask);
    timerId = osTimerCreate(osTimer(timer1),osTimerOnce,NULL);

    osMutexDef(RxDataMutex);
    carStatusMutexHandle = osMutexCreate(osMutex(RxDataMutex));

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
