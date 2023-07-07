#include "common_inc.h"
#include "freertos_inc.h"
#include "motor.hpp"
#include "usart.h"

Motor motor;

/* 心跳任务 */
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
    while(1) {
        osDelay(30);
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

    motor.Init();

    osThreadDef(LedBlinkyTask_, LedBlinkyTask, osPriorityNormal, 0, 128);
    ledBlinkyTaskHandle = osThreadCreate(osThread(LedBlinkyTask_), NULL);

    osThreadDef(MotorTask, MotorHandleTask, osPriorityNormal, 0, 512);
    motorTaskHandle = osThreadCreate(osThread(MotorTask), NULL);

    osThreadDef(ObstacleDetectionTask,obstacleDetectionAndProcessingTask,osPriorityHigh,0,512);
    obstacleTaskHandle = osThreadCreate(osThread(ObstacleDetectionTask), NULL);
}
