#include "common_inc.h"
#include "freertos_inc.h"

#include "ArduinoJson.h"
#include "string"
#include "ikunCar.hpp"
Motor motors[4];
Servo servo(&htim9,TIM_CHANNEL_1);

StaticJsonDocument<200> jsonDocument;
uint8_t rx_data[100];

int64_t pwm;

/* LED Blinking Task */
void LedBlinkyTask(void const *argument) {

    while (1) {
        osMutexWait(pwmMutexHandle,10);
//        s = std::to_string(pwm).append("\n");
        osMutexRelease(pwmMutexHandle);
        HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
        osDelay(1000);
    }
}
/**
 * @def 舵机控制任务
 * @param argument
 */

void ServoHandleTask(void const *argument) {
    servo.Init();
    servo.SetAngle(135);
    while(1){
        osDelay(200);
    }
}


/**
 * TODO 处理编码器数据和更加PWM
 * @def 电机控制任务
 */

void MotorHandleTask(void const *argument) {
    for(uint8_t i=0;i<4;i++){
        motors[i].encoder->Start();
    }
    ikun::move(motors);
    while (1) {
        osMutexWait(pwmMutexHandle,10);
        for(uint8_t i=0;i<4;i++){
            motors[i].correctCount();
        }

        osMutexRelease(pwmMutexHandle);
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
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,rx_data,100);
        osDelay(50);
    }
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart->Instance == USART1){
        //解析数据
        DeserializationError error = deserializeJson(jsonDocument, rx_data);
        if (!error) {
            const char *myC = jsonDocument["myChar"];
            HAL_UART_Transmit_IT(&huart1,(uint8_t*)myC,strlen(myC));
        }
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
    osMutexDef(pwmMutex);
    pwmMutexHandle = osMutexCreate(osMutex(pwmMutex));

    osThreadDef(LedBlinkyTask_, LedBlinkyTask, osPriorityNormal, 0, 256);
    ledBlinkyTaskHandle = osThreadCreate(osThread(LedBlinkyTask_), NULL);

    osThreadDef(MotorTask, MotorHandleTask, osPriorityNormal, 0, 512);
    motorTaskHandle = osThreadCreate(osThread(MotorTask), NULL);

    osThreadDef(ServoTask, ServoHandleTask, osPriorityAboveNormal, 0, 512);
    servoTaskHandle = osThreadCreate(osThread(ServoTask), NULL);

    osThreadDef(ObstacleDetectionTask,obstacleDetectionAndProcessingTask,osPriorityHigh,0,512);
    obstacleTaskHandle = osThreadCreate(osThread(ObstacleDetectionTask), NULL);
}
