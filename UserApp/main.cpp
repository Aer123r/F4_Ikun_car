#include "common_inc.h"
#include "freertos_inc.h"
#include "motor.hpp"

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
        osDelay(50);
    }
}

void Main() {
    motor.Init();

    osThreadDef(LedBlinkyTask_, LedBlinkyTask, osPriorityNormal, 0, 128);
    ledBlinkyTaskHandle = osThreadCreate(osThread(LedBlinkyTask_), NULL);

    osThreadDef(MotorTask, MotorHandleTask, osPriorityNormal, 0, 512);
    motorTaskHandle = osThreadCreate(osThread(MotorTask), NULL);

}
