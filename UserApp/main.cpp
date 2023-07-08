#include "common_inc.h"
#include "freertos_inc.h"
#include "motor.hpp"
#include "MPU6050.hpp"
#include "usart.h"
#include "ArduinoJson.h"


Motor motor;
MPU6050 mpu6050(&hi2c1);
StaticJsonDocument<200> jsonDocument;
uint8_t rx_data[100];

/* LED Blinking Task */
void LedBlinkyTask(void const *argument) {

    while (1) {
        HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
        osDelay(1000);
    }
}

// TODO 处理编码器数据和更加PWM
void MotorHandleTask(void const *argument) {
    motor.Move();
    while (1) {
        osDelay(20);
    }
}

/**
 * @name 障碍物识别处理任务
 * @param argument
 * @interface UART
 */
void obstacleDetectionAndProcessingTask(void const *argument) {
    printf("hello world\n");
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
        // TODO
    }
}

/* 主函数 */
void Main() {

//    do
//    {
//        mpu6050.Init();
//        osDelay(100);
//    } while (!mpu6050.testConnection());
//    mpu6050.InitFilter(200, 100, 50);
    /* 初始化 */
    motor.Init();

    osThreadDef(LedBlinkyTask_, LedBlinkyTask, osPriorityNormal, 0, 128);
    ledBlinkyTaskHandle = osThreadCreate(osThread(LedBlinkyTask_), NULL);

    osThreadDef(MotorTask, MotorHandleTask, osPriorityNormal, 0, 512);
    motorTaskHandle = osThreadCreate(osThread(MotorTask), NULL);

    osThreadDef(ObstacleDetectionTask,obstacleDetectionAndProcessingTask,osPriorityHigh,0,512);
    obstacleTaskHandle = osThreadCreate(osThread(ObstacleDetectionTask), NULL);
}
