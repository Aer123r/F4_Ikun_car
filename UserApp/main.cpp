#include "common_inc.h"
#include "freertos_inc.h"

#include "ArduinoJson.h"
#include "string"
#include "ikunCar.hpp"

Motor motors[4];
Servo servo(&htim9,TIM_CHANNEL_1);

StaticJsonDocument<200> jsonDocument;

uint8_t rx_data[255];
bool isReceived = false;
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
void CarControllerTask(void const *argument){
    servo.Init();
//    servo.SetAngle(135);
//    ikun::backMove(motors);
//    osDelay(3000);
//    osDelay(2000);
//    ikun::backMove(motors);
    while(1){
        osDelay(10);
    }
}
/**
 * @def 舵机控制任务
 * @param argument
 */

void ServoHandleTask(void const *argument) {

    while(1){

        osDelay(200);
    }
}


/**
 * TODO 处理编码器数据和更加PWM
 * @def 电机控制任务
 */

void MotorSpeedUpdateHandleTask(void const *argument) {
    for(auto& motor:motors){
        motor.encoder->Start();
    }
    while (1) {
        for (auto& motor : motors) {
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
    while(1) {
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,rx_data,255);
        osDelay(10);
    }
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    HAL_UART_Transmit(&huart1,rx_data,Size,100);
    DeserializationError error = deserializeJson(jsonDocument, rx_data);
    if (!error) {
        int blockType = jsonDocument["id"];
        float xBias = jsonDocument["Tx"];
        float distance = jsonDocument["Rz"];
        HAL_UART_Transmit(&huart1, (uint8_t *)std::to_string(blockType).append("\n").c_str(),1,100);
        HAL_UART_Transmit(&huart1, (uint8_t *)std::to_string(xBias).append("\n").c_str(),std::to_string(xBias).length(),100);
        HAL_UART_Transmit(&huart1, (uint8_t *)std::to_string(distance).append("\n").c_str(),std::to_string(distance).length(),100);
    }
}
/**
 * @name 光电传感器任务
 * @param 触发的IO号
 * @details IO在上升沿和下降沿会触发这个函数
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_PIN_2){
    }else{}
}

/* 主函数 */
void Main() {
    motors[0].driver = new Driver(Config_Driver_1);
    motors[1].driver = new Driver(Config_Driver_2);
    motors[2].driver = new Driver(Config_Driver_3);
    motors[3].driver = new Driver(Config_Driver_4);

    motors[0].encoder = new Encoder(&htim1);
    motors[1].encoder = new Encoder(&htim3);
    motors[2].encoder = new Encoder(&htim4);
    motors[3].encoder = new Encoder(&htim8);

    Controller *controller = new Controller(0,0,0);

    for(uint8_t i=0;i<4;i++){
        motors[i].driver->Init();
//        motors[i].controller = controller;
    }


    HAL_UART_Transmit_IT(&huart1,(uint8_t *)"hello",5);

    osMutexDef(RxDataMutex);
    rxDataMutexHandle = osMutexCreate(osMutex(RxDataMutex));

    osThreadDef(LedBlinkyTask_, LedBlinkyTask, osPriorityNormal, 0, 256);
    ledBlinkyTaskHandle = osThreadCreate(osThread(LedBlinkyTask_), NULL);

    osThreadDef(MotorUpdateTask, MotorSpeedUpdateHandleTask, osPriorityNormal, 0, 512);
    motorSpeedUpdateTaskHandle = osThreadCreate(osThread(MotorUpdateTask), NULL);

    osThreadDef(ServoTask, ServoHandleTask, osPriorityAboveNormal, 0, 512);
    servoTaskHandle = osThreadCreate(osThread(ServoTask), NULL);

    osThreadDef(ObstacleDetectionTask,obstacleDetectionAndProcessingTask,osPriorityHigh,0,512);
    obstacleTaskHandle = osThreadCreate(osThread(ObstacleDetectionTask), NULL);

    osThreadDef(CarControllerTask,CarControllerTask,osPriorityHigh,0,512);
    carControllerTaskHandle = osThreadCreate(osThread(CarControllerTask),NULL);
}
